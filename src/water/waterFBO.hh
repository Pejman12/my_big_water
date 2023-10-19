#pragma once

#include "texture.hh"
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <memory>

#define TEST_OPENGL_ERROR()                                                     \
    do                                                                          \
    {                                                                           \
        GLenum err = glGetError();                                              \
        if (err != GL_NO_ERROR)                                                 \
            std::cerr << "OpenGL ERROR! : in function " << __FUNCTION__         \
                      << " line " << __LINE__ << ", code : " << err << std::endl;\
    } while (0)


class waterFBO
{
public:
    waterFBO(int width, int height);
    ~waterFBO();
    unsigned int createFrameBuffer();
    unsigned int createDepthTextureAttachment(int width, int height);
    unsigned int createDepthBufferAttachment(int width, int height);
    void bindFrameBuffer(unsigned int frameBuffer, int width, int height);
    void initialiseRefractionFrameBuffer();
    void initialiseReflectionFrameBuffer();
    unsigned int getRefractionDepthTexture();
    shared_texture getRefractionTexture();
    shared_texture getReflectionTexture();
    void unbindCurrentFrameBuffer() noexcept;
    void bindRefractionFrameBuffer() noexcept;
    void bindReflectionFrameBuffer() noexcept;

    int width;
    int height;
private:

    unsigned int reflectionFBO;
    shared_texture reflectionTexture;
    unsigned int reflectionDepthBuffer;

    unsigned int refractionFBO;
    shared_texture refractionTexture;
    unsigned int refractionDepthTexture;
};

using shared_waterFBO = std::shared_ptr<waterFBO>;