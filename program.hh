#pragma once

#include "object.hh"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class program
{
public:
    program();
    ~program();
    char *getlog(GLint id, GLenum type);
    void use();

    void add_shader(const char *filename, GLenum type);

    void add_object(const std::string &name, int nb_vbo);
    const object &get_object(const std::string &name) const;
    const inline std::map<std::string, object> get_objects() const {
        return objects;
    }

    void add_object_vbo(const std::string &name, const std::string &vbo_name, const std::vector<float> &data, GLint unit_size);

    void update_view_matrix(const glm::mat4 &view);
    void update_projection_matrix(float fov, float aspect, float near, float far);
    void update_material(const std::string &name, const std::vector<float> &vec);

    void link_program();
    inline GLuint get_program_id() const
    {
        return program_id;
    }
    inline bool isready() const
    {
        return ready;
    }

    static std::shared_ptr<program> make_program(const char *vertex_shader_src, const char *fragment_shader);

private:
    void set_shader_id(GLuint shd_id, GLenum type);
    GLuint load_shader(const char *filename, GLenum type);

    GLuint program_id;
    GLuint vertex_shd_id;
    GLuint fragment_shd_id;
    std::map<std::string, object> objects;

    bool ready = false;
};

using shared_program = std::shared_ptr<program>;
