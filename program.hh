#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <memory>
#include <string>

#define TEST_OPENGL_ERROR()                                                    \
    do                                                                         \
    {                                                                          \
        GLenum err = glGetError();                                             \
        if (err != GL_NO_ERROR)                                                \
            std::cerr << "OpenGL ERROR!" << __LINE__ << std::endl;             \
    } while (0)

class program
{
public:
    program();
    ~program();
    char *getlog(GLint id, GLenum type);
    void use();

    void set_shader_id(GLuint shd_id, GLenum type);
    GLuint load_shader(const char *filename, GLenum type);
    void add_shader(const char *filename, GLenum type);

    void link_program();
    inline GLuint get_program_id()
    {
        return program_id_;
    }
    inline bool isready()
    {
        return ready_;
    }

    static std::shared_ptr<program> make_program(const char *vertex_shader_src, const char *fragment_shader);

private:
    GLuint program_id_;
    GLuint vertex_shd_id_;
    GLuint fragment_shd_id_;

    bool ready_ = false;
};

using shared_program = std::shared_ptr<program>;
