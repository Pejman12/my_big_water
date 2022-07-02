#ifndef OBJECT_HEADER
#define OBJECT_HEADER

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>

#define TEST_OPENGL_ERROR()                                                    \
    do                                                                         \
    {                                                                          \
        GLenum err = glGetError();                                             \
        if (err != GL_NO_ERROR)                                                \
            std::cerr << "OpenGL ERROR! : line " << __FUNCTION__ << __LINE__ << ", code : "    \
                      << err << std::endl;                                     \
    } while (0)


class object {
public:
    object(int nb_vbo_);

    void add_vbo(const std::string &name, const std::vector<float> &data, GLuint program_id, GLint unit_size);

    inline GLuint get_vao_id() const
    {
        return vao_id;
    }

    inline GLuint get_vbo_id(int i) const
    {
        return vbo_ids[i];
    }

    std::map<std::string, GLuint> vbo_ids_map;
    std::map<GLuint, std::vector<float>> vbo_data_map;

private:
    GLuint vao_id;
    int nb_vbo;
    int nb_vbo_added;
    std::vector<GLuint> vbo_ids;
};

#endif