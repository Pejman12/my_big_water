#include "texture.hh"
#include "io_png.hh"

static unsigned char *flatten(unsigned char **buffer_, const int width, const int height) noexcept {
    auto result = (unsigned char *)malloc(sizeof(unsigned char) * width * height);
#pragma omp parallel for schedule(dynamic) shared(buffer_, result, width, height) default(none) collapse(2)
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            result[i * width + j] = buffer_[i][j];
    return result;
}

texture::texture()
{}

texture::texture(const std::string &fileName, bool hasTransparency)
    : fileName(fileName)
{
    unsigned char **pixels_ = read_png(fileName.c_str(), &width, &height);
    auto pixels = flatten(pixels_, width * 3, height);
    for (int i = 0; i < height; free(pixels_[i++]));
    free(pixels_);
    this->transparency = hasTransparency;

    glGenTextures(1, &textureID);TEST_OPENGL_ERROR();
    bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);TEST_OPENGL_ERROR();

    glGenerateMipmap(GL_TEXTURE_2D);TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);TEST_OPENGL_ERROR();
    free(pixels);

    unbind();
}

texture::~texture() {
    glDeleteTextures(1, &textureID);TEST_OPENGL_ERROR();
}

bool texture::hasTransparency() {
    return transparency;
}

void texture::bind(int index) const noexcept{
    glActiveTexture(GL_TEXTURE0 + index);TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, textureID);TEST_OPENGL_ERROR();
}

void texture::unbind() const noexcept {
    glBindTexture(GL_TEXTURE_2D, 0);TEST_OPENGL_ERROR();
}

shared_texture texture::createTextureAttachment(int width, int height) {
    auto Texture = std::make_shared<texture>();
    Texture->width = width;
    Texture->height = height;
    Texture->transparency = false;

    glGenTextures(1, &(Texture->textureID));TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, Texture->textureID);TEST_OPENGL_ERROR();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);TEST_OPENGL_ERROR();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture->textureID, 0);TEST_OPENGL_ERROR();

    return Texture;
}

