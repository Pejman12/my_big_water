#include "waterFBO.hh"

waterFBO::waterFBO(int width_, int height_)
    : width(width_)
    , height(height_)
{
    initialiseReflectionFrameBuffer();
    initialiseRefractionFrameBuffer();
}

waterFBO::~waterFBO() {
    glDeleteFramebuffers(1, &reflectionFBO);
    glDeleteTextures(1, &reflectionTexture);
    glDeleteRenderbuffers(1, &reflectionDepthBuffer);
    glDeleteFramebuffers(1, &refractionFBO);
    glDeleteTextures(1, &refractionTexture);
    glDeleteTextures(1, &refractionDepthTexture);
}

void waterFBO::bindReflectionFrameBuffer() noexcept {
    bindFrameBuffer(reflectionFBO, width, height);
}

void waterFBO::bindRefractionFrameBuffer() noexcept {
    bindFrameBuffer(refractionFBO, width, height);
}

void waterFBO::unbindCurrentFrameBuffer() noexcept {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);TEST_OPENGL_ERROR();
    glViewport(0, 0, width, height);TEST_OPENGL_ERROR();
}

unsigned int waterFBO::getReflectionTexture() {
    return reflectionTexture;
}

unsigned int waterFBO::getRefractionTexture() {
    return refractionTexture;
}

unsigned int waterFBO::getRefractionDepthTexture() {
    return refractionDepthTexture;
}

void waterFBO::initialiseReflectionFrameBuffer() {
    reflectionFBO = createFrameBuffer();
    reflectionTexture = createTextureAttachment(width, height);
    reflectionDepthBuffer = createDepthBufferAttachment(width, height);
    unbindCurrentFrameBuffer();
}

void waterFBO::initialiseRefractionFrameBuffer() {
    refractionFBO = createFrameBuffer();
    refractionTexture = createTextureAttachment(width, height);
    refractionDepthTexture = createDepthTextureAttachment(width, height);
    unbindCurrentFrameBuffer();
}

void waterFBO::bindFrameBuffer(unsigned int frameBuffer, int width, int height) {
    glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
    TEST_OPENGL_ERROR();
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);TEST_OPENGL_ERROR();
    glViewport(0, 0, width, height);TEST_OPENGL_ERROR();
}

unsigned int waterFBO::createFrameBuffer() {
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);TEST_OPENGL_ERROR();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);TEST_OPENGL_ERROR();
    glDrawBuffer(GL_COLOR_ATTACHMENT0);TEST_OPENGL_ERROR();
    return fbo;
}

unsigned int waterFBO::createTextureAttachment(int width, int height) {
    unsigned int texture;
    glGenTextures(1, &texture);TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texture);TEST_OPENGL_ERROR();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);TEST_OPENGL_ERROR();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);TEST_OPENGL_ERROR();
    return texture;
}

unsigned int waterFBO::createDepthTextureAttachment(int width, int height) {
    unsigned int texture;
    glGenTextures(1, &texture);TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texture);TEST_OPENGL_ERROR();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);TEST_OPENGL_ERROR();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);TEST_OPENGL_ERROR();
    return texture;
}

unsigned int waterFBO::createDepthBufferAttachment(int width, int height) {
    unsigned int depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);TEST_OPENGL_ERROR();
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);TEST_OPENGL_ERROR();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);TEST_OPENGL_ERROR();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    TEST_OPENGL_ERROR();
    return depthBuffer;
}