/*
 * Notes
 * credits to Valentin Ignatev who wrote the reference code i used https://gist.github.com/valignatev/60fdd91fefabd131a0e53fe2e3ef0ec7
 */

/********************/
/*     includes     */
/********************/

#include "platform/platform.h"

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

#include <xcb/xcb.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "platform/opengl.h"

/********************/
/*      defines     */
/********************/

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

/********************/
/* static variables */
/********************/

static bool run                 = true;

static xcb_connection_t* conn   = NULL;
static xcb_generic_error_t* err = NULL;
static xcb_screen_t* screen     = NULL;
static xcb_void_cookie_t cookie = { 0 };
static xcb_window_t window      = { 0 };
static xcb_atom_t wm_close      = { 0 };

static EGLDisplay display       = { 0 };
static EGLContext context       = { 0 };
static EGLSurface surface       = { 0 };
static EGLConfig config         = { 0 };
static EGLBoolean ok            = { 0 };
static EGLint major             = 0;
static EGLint minor             = 0;
static EGLint count             = 0;

static EGLint conf_attr[] = { EGL_SURFACE_TYPE,         EGL_WINDOW_BIT,
                              EGL_CONFORMANT,           EGL_OPENGL_BIT,
                              EGL_RENDERABLE_TYPE,      EGL_OPENGL_BIT,
                              EGL_COLOR_BUFFER_TYPE,    EGL_RGB_BUFFER,
                              EGL_RED_SIZE,             8,
                              EGL_GREEN_SIZE,           8,
                              EGL_BLUE_SIZE,            8,
                              EGL_DEPTH_SIZE,           24,
                              EGL_STENCIL_SIZE,         8,
                              EGL_NONE };

static EGLint ctx_attr[] = {  EGL_CONTEXT_MAJOR_VERSION, 4,
                              EGL_CONTEXT_MINOR_VERSION, 5,
                              EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
                              EGL_NONE };

static EGLint surf_attr[] = { EGL_GL_COLORSPACE, EGL_GL_COLORSPACE_LINEAR, // or use EGL_GL_COLORSPACE_SRGB for sRGB framebuffer
                              EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
                              EGL_NONE };

/********************/
/* static functions */
/********************/

static void xcb_init()
{
    conn = xcb_connect(NULL, NULL);
    if (!conn || xcb_connection_has_error(conn))
    {
        fprintf(stderr, "Couldn't connect to X server: error %d\n", conn ? xcb_connection_has_error(conn) : -1);
        assert(false);
    }

    uint32_t attributes[3] = { 0xffffffff,
                               XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_STRUCTURE_NOTIFY,
                               XCB_GRAVITY_STATIC };
    screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
    window = xcb_generate_id(conn);
    cookie = xcb_create_window_checked(conn,
                                       XCB_COPY_FROM_PARENT,
                                       window,
                                       screen->root,
                                       0,
                                       0,
                                       WINDOW_WIDTH,
                                       WINDOW_HEIGHT,
                                       1,
                                       XCB_WINDOW_CLASS_INPUT_OUTPUT,
                                       screen->root_visual,
                                       XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK | XCB_CW_BIT_GRAVITY,
                                       attributes);

    err = xcb_request_check(conn, cookie);

    if (err)
    {
        fprintf(stderr, "Couldn't create X window: error\n");
        assert(false);
    }

    // change window name
    xcb_change_property(conn,
                        XCB_PROP_MODE_REPLACE,
                        window,
                        XCB_ATOM_WM_NAME,
                        XCB_ATOM_STRING,
                        8,
                        13,
                        "VL Browser");

    xcb_intern_atom_cookie_t protocols_cookie   = xcb_intern_atom(conn, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_cookie_t delete_cookie      = xcb_intern_atom(conn, 0, 16, "WM_DELETE_WINDOW");
    xcb_intern_atom_reply_t* wm_protocols       = xcb_intern_atom_reply(conn, protocols_cookie, 0);
    xcb_intern_atom_reply_t* delete_window      = xcb_intern_atom_reply(conn, delete_cookie, 0);
    wm_close = delete_window->atom;

    xcb_change_property(conn,
                        XCB_PROP_MODE_REPLACE,
                        window,
                        wm_protocols->atom,
                        XCB_ATOM_ATOM,
                        32,
                        1,
                        &(delete_window->atom));

    xcb_map_window_checked(conn, window);
}


static void egl_init()
{

    display = eglGetPlatformDisplay(EGL_PLATFORM_XCB_EXT, (void*)conn, NULL);

    assert(display != EGL_NO_DISPLAY);

    ok = eglInitialize(display, &major, &minor);

    if (!ok)
    {
        assert(false && "Cannot initialize EGL display");
    }

    if (major < 1 || (major == 1 && minor < 5))
    {
        assert(false && "EGL version 1.5 or higher required");
    }

    ok = eglBindAPI(EGL_OPENGL_API);

    if (!ok)
    {
        assert(false && "Failed to select OpenGL api");
    }

    ok = eglChooseConfig(display, conf_attr, &config, 1, &count);

    if (!ok || count != 1)
    {
        assert(false && "Cannot choose EGL config");
    }

    context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctx_attr); 

    if (context == EGL_NO_CONTEXT)
    {
        assert(false && "Cannot create EGL context, OpenGL 4.5 not supported?");
    }

    surface = eglCreateWindowSurface(display, config, window, surf_attr);

    if (surface == EGL_NO_SURFACE)
    {
        assert(false && "Cannot create EGL surface");
    }
}

/********************/
/* public functions */
/********************/


void platform_init()
{
    xcb_init();
    egl_init();
    run = true;
}


void platform_process_events()
{
    xcb_generic_event_t* event              = xcb_poll_for_event(conn);
    // xcb_expose_event_t* expose              = NULL;
    // xcb_key_press_event_t* press            = NULL;
    xcb_client_message_event_t* client_msg    = NULL;

    while (event)
    {
        uint8_t r_type = event->response_type & 0x7f;
        printf("got event - %u\n", r_type);
        switch (r_type)
        {
            case XCB_EXPOSE:
                // expose = (xcb_expose_event_t *)event;
                printf("should draw");
                break;

            case XCB_CLIENT_MESSAGE:

                client_msg                      = (xcb_client_message_event_t*)event;
                xcb_client_message_data_t data  = client_msg->data;
                if (data.data32[0] == wm_close)
                {
                    run = false;
                }
                break;

            case XCB_KEY_RELEASE:
                break;

            case 0:
                printf("X11 error: %d\n", err->error_code);
        }

        if (event) { free(event); }
        event = xcb_poll_for_event(conn);
    }
}


bool platform_should_run()
{
    return run;
}


void platform_paint()
{

}


void platform_free()
{

}
