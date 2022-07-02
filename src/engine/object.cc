#include "object.hh"

object::object(int nb_vbo_)
{
    glGenVertexArrays(1, &vao_id);
    TEST_OPENGL_ERROR();
    this->nb_vbo_added = 0;
    this->nb_vbo = nb_vbo_;
    this->vbo_ids.reserve(nb_vbo_);
    glGenBuffers(nb_vbo_, this->vbo_ids.data());
    TEST_OPENGL_ERROR();
}

object::~object()
{
    glDeleteVertexArrays(1, &vao_id);
    TEST_OPENGL_ERROR();
    glDeleteBuffers(nb_vbo, this->vbo_ids.data());
    TEST_OPENGL_ERROR();
}

void object::add_vbo(const std::string &name, const std::vector<float> &data, GLuint program_id, GLint unit_size)
{
    glBindVertexArray(vao_id);
    TEST_OPENGL_ERROR();

    GLint vbo_loc = glGetAttribLocation(program_id, name.c_str());
    TEST_OPENGL_ERROR();

    if (vbo_loc == -1)
        return;

    vbo_ids_map.insert({name, vbo_loc});
    vbo_data_map.insert({vbo_loc, data});

    glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[nb_vbo_added++]);
    TEST_OPENGL_ERROR();

    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    TEST_OPENGL_ERROR();

    glVertexAttribPointer(vbo_loc, unit_size, GL_FLOAT, GL_FALSE, 0, 0);
    TEST_OPENGL_ERROR();

    glEnableVertexAttribArray(vbo_loc);
    TEST_OPENGL_ERROR();

    glBindVertexArray(0);
    TEST_OPENGL_ERROR();
}
