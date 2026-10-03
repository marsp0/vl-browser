#pragma once

#include <GL/glcorearb.h>

extern PFNGLENABLEPROC                   glEnable;
extern PFNGLDISABLEPROC                  glDisable;
extern PFNGLBLENDFUNCPROC                glBlendFunc;
extern PFNGLVIEWPORTPROC                 glViewport;
extern PFNGLCLEARCOLORPROC               glClearColor;
extern PFNGLCLEARPROC                    glClear;
extern PFNGLDRAWARRAYSPROC               glDrawArrays;
extern PFNGLCREATEBUFFERSPROC            glCreateBuffers;
extern PFNGLNAMEDBUFFERSTORAGEPROC       glNamedBufferStorage;
extern PFNGLBINDVERTEXARRAYPROC          glBindVertexArray;
extern PFNGLCREATEVERTEXARRAYSPROC       glCreateVertexArrays;
extern PFNGLVERTEXARRAYATTRIBBINDINGPROC glVertexArrayAttribBinding;
extern PFNGLVERTEXARRAYVERTEXBUFFERPROC  glVertexArrayVertexBuffer;
extern PFNGLVERTEXARRAYATTRIBFORMATPROC  glVertexArrayAttribFormat;
extern PFNGLENABLEVERTEXARRAYATTRIBPROC  glEnableVertexArrayAttrib;
extern PFNGLCREATESHADERPROGRAMVPROC     glCreateShaderProgramv;
extern PFNGLGETPROGRAMIVPROC             glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;
extern PFNGLGENPROGRAMPIPELINESPROC      glGenProgramPipelines;
extern PFNGLUSEPROGRAMSTAGESPROC         glUseProgramStages;
extern PFNGLBINDPROGRAMPIPELINEPROC      glBindProgramPipeline;
extern PFNGLPROGRAMUNIFORMMATRIX2FVPROC  glProgramUniformMatrix2fv;
extern PFNGLBINDTEXTUREUNITPROC          glBindTextureUnit;
extern PFNGLCREATETEXTURESPROC           glCreateTextures;
extern PFNGLTEXTUREPARAMETERIPROC        glTextureParameteri;
extern PFNGLTEXTURESTORAGE2DPROC         glTextureStorage2D;
extern PFNGLTEXTURESUBIMAGE2DPROC        glTextureSubImage2D;
extern PFNGLDEBUGMESSAGECALLBACKPROC     glDebugMessageCallback;

void platform_opengl_init();