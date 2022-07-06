#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <memory>

#define TEST_OPENGL_ERROR()                                                    \
    do                                                                         \
    {                                                                          \
        GLenum err = glGetError();                                             \
        if (err != GL_NO_ERROR)                                                \
            std::cerr << "OpenGL ERROR! : in function "                         \
            << __FUNCTION__  << " line "<< __LINE__ << ", code : " << err << std::endl; \
    } while (0)                                                                \

class texture {
public:
    texture();
    texture(const std::string &fileName, bool hasTransparency = false);
    ~texture();

    bool hasTransparency();

    void bind(int index = 0) const noexcept;
    void unbind() const noexcept;

    static std::shared_ptr<texture> createTextureAttachment(int width, int height);

private:
    std::string fileName;
    GLuint textureID;
    GLsizei width, height;
    bool transparency;
};

using shared_texture = std::shared_ptr<texture>;
