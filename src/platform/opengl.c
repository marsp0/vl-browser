/*
 * Notes
 * 
 */

/********************/
/*     includes     */
/********************/

#include "opengl.h"

#include <EGL/egl.h>
#include <assert.h>

/********************/
/*      defines     */
/********************/


/********************/
/* static variables */
/********************/

PFNGLENABLEPROC                   glEnable;
PFNGLDISABLEPROC                  glDisable;
PFNGLBLENDFUNCPROC                glBlendFunc;
PFNGLVIEWPORTPROC                 glViewport;
PFNGLCLEARCOLORPROC               glClearColor;
PFNGLCLEARPROC                    glClear;
PFNGLDRAWARRAYSPROC               glDrawArrays;
PFNGLCREATEBUFFERSPROC            glCreateBuffers;
PFNGLNAMEDBUFFERSTORAGEPROC       glNamedBufferStorage;
PFNGLBINDVERTEXARRAYPROC          glBindVertexArray;
PFNGLCREATEVERTEXARRAYSPROC       glCreateVertexArrays;
PFNGLVERTEXARRAYATTRIBBINDINGPROC glVertexArrayAttribBinding;
PFNGLVERTEXARRAYVERTEXBUFFERPROC  glVertexArrayVertexBuffer;
PFNGLVERTEXARRAYATTRIBFORMATPROC  glVertexArrayAttribFormat;
PFNGLENABLEVERTEXARRAYATTRIBPROC  glEnableVertexArrayAttrib;
PFNGLCREATESHADERPROGRAMVPROC     glCreateShaderProgramv;
PFNGLGETPROGRAMIVPROC             glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;
PFNGLGENPROGRAMPIPELINESPROC      glGenProgramPipelines;
PFNGLUSEPROGRAMSTAGESPROC         glUseProgramStages;
PFNGLBINDPROGRAMPIPELINEPROC      glBindProgramPipeline;
PFNGLPROGRAMUNIFORMMATRIX2FVPROC  glProgramUniformMatrix2fv;
PFNGLBINDTEXTUREUNITPROC          glBindTextureUnit;
PFNGLCREATETEXTURESPROC           glCreateTextures;
PFNGLTEXTUREPARAMETERIPROC        glTextureParameteri;
PFNGLTEXTURESTORAGE2DPROC         glTextureStorage2D;
PFNGLTEXTURESUBIMAGE2DPROC        glTextureSubImage2D;
PFNGLDEBUGMESSAGECALLBACKPROC     glDebugMessageCallback;

/********************/
/* static functions */
/********************/


/********************/
/* public functions */
/********************/

void platform_opengl_init()
{
    glEnable                            = (PFNGLENABLEPROC)eglGetProcAddress("glEnable");
    assert(glEnable);
    glDisable                           = (PFNGLDISABLEPROC)eglGetProcAddress("glDisable");
    assert(glDisable);
    glBlendFunc                         = (PFNGLBLENDFUNCPROC)eglGetProcAddress("glBlendFunc");
    assert(glBlendFunc);
    glViewport                          = (PFNGLVIEWPORTPROC)eglGetProcAddress("glViewport");
    assert(glViewport);
    glClearColor                        = (PFNGLCLEARCOLORPROC)eglGetProcAddress("glClearColor");
    assert(glClearColor);
    glClear                             = (PFNGLCLEARPROC)eglGetProcAddress("glClear");
    assert(glClear);
    glDrawArrays                        = (PFNGLDRAWARRAYSPROC)eglGetProcAddress("glDrawArrays");
    assert(glDrawArrays);
    glCreateBuffers                     = (PFNGLCREATEBUFFERSPROC)eglGetProcAddress("glCreateBuffers");
    assert(glCreateBuffers);
    glNamedBufferStorage                = (PFNGLNAMEDBUFFERSTORAGEPROC)eglGetProcAddress("glNamedBufferStorage");
    assert(glNamedBufferStorage);
    glBindVertexArray                   = (PFNGLBINDVERTEXARRAYPROC)eglGetProcAddress("glBindVertexArray");
    assert(glBindVertexArray);
    glCreateVertexArrays                = (PFNGLCREATEVERTEXARRAYSPROC)eglGetProcAddress("glCreateVertexArrays");
    assert(glCreateVertexArrays);
    glVertexArrayAttribBinding          = (PFNGLVERTEXARRAYATTRIBBINDINGPROC)eglGetProcAddress("glVertexArrayAttribBinding");
    assert(glVertexArrayAttribBinding);
    glVertexArrayVertexBuffer           = (PFNGLVERTEXARRAYVERTEXBUFFERPROC)eglGetProcAddress("glVertexArrayVertexBuffer");
    assert(glVertexArrayVertexBuffer);
    glVertexArrayAttribFormat           = (PFNGLVERTEXARRAYATTRIBFORMATPROC)eglGetProcAddress("glVertexArrayAttribFormat");
    assert(glVertexArrayAttribFormat);
    glEnableVertexArrayAttrib           = (PFNGLENABLEVERTEXARRAYATTRIBPROC)eglGetProcAddress("glEnableVertexArrayAttrib");
    assert(glEnableVertexArrayAttrib);
    glCreateShaderProgramv              = (PFNGLCREATESHADERPROGRAMVPROC)eglGetProcAddress("glCreateShaderProgramv");
    assert(glCreateShaderProgramv);
    glGetProgramiv                      = (PFNGLGETPROGRAMIVPROC)eglGetProcAddress("glGetProgramiv");
    assert(glGetProgramiv);
    glGetProgramInfoLog                 = (PFNGLGETPROGRAMINFOLOGPROC)eglGetProcAddress("glGetProgramInfoLog");
    assert(glGetProgramInfoLog);
    glGenProgramPipelines               = (PFNGLGENPROGRAMPIPELINESPROC)eglGetProcAddress("glGenProgramPipelines");
    assert(glGenProgramPipelines);
    glUseProgramStages                  = (PFNGLUSEPROGRAMSTAGESPROC)eglGetProcAddress("glUseProgramStages");
    assert(glUseProgramStages);
    glBindProgramPipeline               = (PFNGLBINDPROGRAMPIPELINEPROC)eglGetProcAddress("glBindProgramPipeline");
    assert(glBindProgramPipeline);
    glProgramUniformMatrix2fv           = (PFNGLPROGRAMUNIFORMMATRIX2FVPROC)eglGetProcAddress("glProgramUniformMatrix2fv");
    assert(glProgramUniformMatrix2fv);
    glBindTextureUnit                   = (PFNGLBINDTEXTUREUNITPROC)eglGetProcAddress("glBindTextureUnit");
    assert(glBindTextureUnit);
    glCreateTextures                    = (PFNGLCREATETEXTURESPROC)eglGetProcAddress("glCreateTextures");
    assert(glCreateTextures);
    glTextureParameteri                 = (PFNGLTEXTUREPARAMETERIPROC)eglGetProcAddress("glTextureParameteri");
    assert(glTextureParameteri);
    glTextureStorage2D                  = (PFNGLTEXTURESTORAGE2DPROC)eglGetProcAddress("glTextureStorage2D");
    assert(glTextureStorage2D);
    glTextureSubImage2D                 = (PFNGLTEXTURESUBIMAGE2DPROC)eglGetProcAddress("glTextureSubImage2D");
    assert(glTextureSubImage2D);
    glDebugMessageCallback              = (PFNGLDEBUGMESSAGECALLBACKPROC)eglGetProcAddress("glDebugMessageCallback");
    assert(glDebugMessageCallback);
}