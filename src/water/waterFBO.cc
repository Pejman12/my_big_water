#include "waterFBO.hh"

waterFBO::waterFBO(int width, int height)
    : displayWidth(width)
    , displayHeight(height)
    , REFRACTION_WIDTH(width)
    , REFRACTION_HEIGHT(height)
    , REFLECTION_WIDTH(width/4)
    , REFLECTION_HEIGHT(height/4)
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
    bindFrameBuffer(reflectionFBO,REFLECTION_WIDTH,REFLECTION_HEIGHT);
}

void waterFBO::bindRefractionFrameBuffer() noexcept {
    bindFrameBuffer(refractionFBO,REFRACTION_WIDTH,REFRACTION_HEIGHT);
}

void waterFBO::unbindCurrentFrameBuffer() noexcept {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, displayWidth, displayHeight);
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
    reflectionTexture = createTextureAttachment(REFLECTION_WIDTH,REFLECTION_HEIGHT);
    reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH,REFLECTION_HEIGHT);
    unbindCurrentFrameBuffer();
}

void waterFBO::initialiseRefractionFrameBuffer() {
    refractionFBO = createFrameBuffer();
    refractionTexture = createTextureAttachment(REFRACTION_WIDTH,REFRACTION_HEIGHT);
    refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH,REFRACTION_HEIGHT);
    unbindCurrentFrameBuffer();
}

void waterFBO::bindFrameBuffer(unsigned int frameBuffer, int width, int height) {
    glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, width, height);
}

unsigned int waterFBO::createFrameBuffer() {
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    return fbo;
}

unsigned int waterFBO::createTextureAttachment(int width, int height) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
    return texture;
}

unsigned int waterFBO::createDepthTextureAttachment(int width, int height) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
    return texture;
}

unsigned int waterFBO::createDepthBufferAttachment(int width, int height) {
    unsigned int depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    return depthBuffer;
}