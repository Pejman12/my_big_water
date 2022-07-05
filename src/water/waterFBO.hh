#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <memory>

class waterFBO
{
public:
    waterFBO(int width, int height);
    ~waterFBO();
    unsigned int createFrameBuffer();
    unsigned int createTextureAttachment(int width, int height);
    unsigned int createDepthTextureAttachment(int width, int height);
    unsigned int createDepthBufferAttachment(int width, int height);
    void bindFrameBuffer(unsigned int frameBuffer, int width, int height);
    void initialiseRefractionFrameBuffer();
    void initialiseReflectionFrameBuffer();
    unsigned int getRefractionDepthTexture();
    unsigned int getRefractionTexture();
    unsigned int getReflectionTexture();
    void unbindCurrentFrameBuffer() noexcept;
    void bindRefractionFrameBuffer() noexcept;
    void bindReflectionFrameBuffer() noexcept;

private:
    int displayWidth;
    int displayHeight;
    const int REFRACTION_WIDTH;
    const int REFRACTION_HEIGHT;
    const int REFLECTION_WIDTH;
    const int REFLECTION_HEIGHT;

    unsigned int reflectionFBO;
    unsigned int reflectionTexture;
    unsigned int reflectionDepthBuffer;

    unsigned int refractionFBO;
    unsigned int refractionTexture;
    unsigned int refractionDepthTexture;
};

using shared_waterFBO = std::shared_ptr<waterFBO>;