#pragma once

#include "obj_raw.hh"
#include "object.hh"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class program
{
public:
    program(obj_raw::objRawPtr mat);
    ~program();
    char *getlog(GLint id, GLenum type);
    void use() noexcept;

    void add_shader(const char *filename, GLenum type);

    void add_object(const std::string &name, int nb_vbo);
    const objectPtr get_object(const std::string &name) const;
    const inline std::map<std::string, objectPtr> get_objects() const noexcept {
        return objects;
    }

    void add_object_vbo(const std::string &name, const std::string &vbo_name, const std::vector<float> &data, GLint unit_size);

    void update_view_matrix(const glm::mat4 &view) noexcept;
    void update_projection_matrix(float fov, float aspect, float near, float far) noexcept;
    void update_material(const std::string &name, const std::vector<float> &vec) noexcept;
    void update_materials() noexcept;

    void link_program();
    inline GLuint get_program_id() const
    {
        return program_id;
    }
    inline bool isready() const
    {
        return ready;
    }

    static std::shared_ptr<program> make_program(const char *vertex_shader_src, const char *fragment_shader,
                                                 obj_raw::objRawPtr mat);

private:
    void set_shader_id(GLuint shd_id, GLenum type);
    GLuint load_shader(const char *filename, GLenum type);

    GLuint program_id;
    GLuint vertex_shd_id;
    GLuint fragment_shd_id;
    std::map<std::string, objectPtr> objects;
    obj_raw::objRawPtr material;

    bool ready = false;
};

using shared_program = std::shared_ptr<program>;
