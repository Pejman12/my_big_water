#pragma once

#include "object.hh"
#include "obj_raw.hh"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class program
{
public:
    program(obj_raw::shared_objRaw mat);
    ~program();
    char *getlog(GLint id, GLenum type);
    void add_shader(const char *filename, GLenum type);
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
                                                 const std::string &UBO_name, obj_raw::shared_objRaw mat);

    void use() noexcept;

    void init_objects(const std::vector<obj_raw::shared_objRaw> &vaos);
    const shared_object get_object(const std::string &name) const;
    inline std::map<std::string, shared_object> get_objects() const noexcept {
        return objects;
    }

    void update_material(const std::string &name, const std::vector<float> &vec) noexcept;
    void update_materials() noexcept;
    void setTexture(const std::string &name, int value) noexcept;
    void setPlane(const glm::vec4 &plane) noexcept;
    void setTime(const float time) noexcept;
    void setCamPos(const glm::vec3 &pos) noexcept;

    void draw() noexcept;

private:
    void set_shader_id(GLuint shd_id, GLenum type);
    GLuint load_shader(const char *filename, GLenum type);

    GLuint program_id;
    GLuint vertex_shd_id;
    GLuint fragment_shd_id;
    unsigned int UBO_id;
    std::map<std::string, shared_object> objects;
    obj_raw::shared_objRaw material;

    bool ready = false;
};

using shared_program = std::shared_ptr<program>;
