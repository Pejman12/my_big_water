#include "program.hh"

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <fcntl.h>

#define BUFF_SIZE 4096

program::program(obj_raw::shared_objRaw mat)
    : vertex_shd_id(0)
    , fragment_shd_id(0)
    , UBO_id(0)
    , material(mat)
{
    program_id = glCreateProgram();
    TEST_OPENGL_ERROR();
    if (program_id == 0) {
        std::cerr << "Error: glCreateProgram() failed" << std::endl;
        std::exit(-1);
    }
}

program::~program() {
    glDeleteProgram(program_id);
    TEST_OPENGL_ERROR();

    glDeleteShader(vertex_shd_id);
    TEST_OPENGL_ERROR();

    glDeleteShader(fragment_shd_id);
    TEST_OPENGL_ERROR();

    program_id = 0;
}

char *program::getlog(GLint id, GLenum type)
{
    GLint log_size;
    char *log;
    if (type == GL_SHADER)
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_size);
    else if (type == GL_PROGRAM)
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_size);
    else
        return nullptr;

    log = (char *)malloc(log_size + 1);
    if (log != 0)
    {
        if (type == GL_SHADER)
            glGetShaderInfoLog(id, log_size, &log_size, log);
        else if (type == GL_PROGRAM)
            glGetProgramInfoLog(program_id, log_size, &log_size, log);
        return log;
    }

    return nullptr;
}

void program::set_shader_id(GLuint shd_id, GLenum type)
{
    if (type == GL_VERTEX_SHADER)
        vertex_shd_id = shd_id;

    if (type == GL_FRAGMENT_SHADER)
        fragment_shd_id = shd_id;
}

static char *load_file(const char *filename)
{
    int fd;
    if ((fd = open(filename, O_RDONLY)) == -1)
        errx(1, "%s: No such file or directory", filename);
    posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL | POSIX_FADV_NOREUSE |
                            POSIX_FADV_WILLNEED);

    char *str = nullptr;
    ssize_t len_rd = 0;
    size_t len_file = 0;
    do
    {
        len_file += len_rd;
        str = (char *)realloc(str, sizeof(char) * (len_file + BUFF_SIZE));
        str[len_file] = '\0';
    } while ((len_rd = read(fd, str + len_file, BUFF_SIZE - 1)) > 0);
    close(fd);
    str = (char *)realloc(str, sizeof(char) * (len_file + 1));

    return str;
}

GLuint program::load_shader(const char *filename, GLenum type)
{
    GLint compile_status = GL_TRUE;

    GLuint shader_id = glCreateShader(type);
    TEST_OPENGL_ERROR();

    char *src = load_file(filename);
    glShaderSource(shader_id, 1, (const GLchar **)&(src), 0);
    TEST_OPENGL_ERROR();
    free(src);

    glCompileShader(shader_id);
    TEST_OPENGL_ERROR();
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
    TEST_OPENGL_ERROR();

    if (compile_status != GL_TRUE)
    {
        char *log = getlog(shader_id, GL_SHADER);
        if (log)
        {
            auto str = type == GL_VERTEX_SHADER ? "vertex " : "fragment ";
            errx(-1, "Shader %s %u : %s\n", str, shader_id, log);
        }
    }

    return shader_id;
}

void program::add_shader(const char *filename, GLenum type) {
    GLuint shader_id = load_shader(filename, type);
    if (shader_id == 0)
        return;
    set_shader_id(shader_id, type);

    glAttachShader(program_id, shader_id);
    TEST_OPENGL_ERROR();
}

void program::link_program()
{
    GLint link_status = GL_TRUE;

    glLinkProgram(program_id);
    TEST_OPENGL_ERROR();

    glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE)
    {
        char *program_log = getlog(program_id, GL_PROGRAM);
        if (program_log)
            errx(-1, "Program %s\n", program_log);
    }

    ready = true;
}

std::shared_ptr<program> program::make_program(const char *vertex_shader_src, const char *fragment_shader_src,
                                               const std::string &UBO_name, obj_raw::shared_objRaw mat)
{
    shared_program prog = std::make_shared<program>(mat);

    prog->add_shader(vertex_shader_src, GL_VERTEX_SHADER);
    prog->add_shader(fragment_shader_src, GL_FRAGMENT_SHADER);

    prog->link_program();

    prog->UBO_id = glGetUniformBlockIndex(prog->program_id, UBO_name.c_str());
    TEST_OPENGL_ERROR();
    glUniformBlockBinding(prog->program_id, prog->UBO_id, 0);
    TEST_OPENGL_ERROR();

    return prog;
}

void program::use() noexcept {
    if (ready) {
        glUseProgram(program_id);
        TEST_OPENGL_ERROR();
    } else
        errx(-1, "Program is not ready");
}

const shared_object program::get_object(const std::string &name) const {
    return objects.at(name);
}

void program::init_objects(const std::vector<obj_raw::shared_objRaw> &vaos) {
    for (const auto &vao : vaos)
    {
        auto obj = std::make_shared<object>(2);
        objects.insert({vao->name, obj});
        objects.at(vao->name)->add_vbo("position", vao->vecs.at("position"), program_id, 3);
        objects.at(vao->name)->add_vbo("normal", vao->vecs.at("normal"), program_id, 3);
    }
}

static inline void update_vec(const GLuint prog_id, const std::string &name, const std::vector<float> &vec) noexcept {
    GLint uniform_location = glGetUniformLocation(prog_id, name.c_str());
    TEST_OPENGL_ERROR();
#ifdef DEBUG
    if (uniform_location == -1)
        warnx("Could not find uniform %s", name.c_str());
#endif
    if (name != "Ns") {
        glUniform3fv(uniform_location, 1, vec.data());
        TEST_OPENGL_ERROR();
    } else {
        glUniform1f(uniform_location, vec[0]);
        TEST_OPENGL_ERROR();
    }
}

void program::update_material(const std::string &name, const std::vector<float> &vec) noexcept
{
    material->vecs[name] = vec;
    update_vec(program_id, name, vec);
}

void program::update_materials() noexcept {
    use();
    for (const auto &[name, vec] : material->vecs) {
        update_vec(program_id, name, vec);
    }
    glUseProgram(0);
}

void program::draw() noexcept {
    use();
    for (const auto &[objName, obj]: objects)
        obj->draw();
    glUseProgram(0);
}

void program::setTexture(const std::string &name, int value) noexcept {
    use();
    GLint location = glGetUniformLocation(program_id, name.c_str());
    TEST_OPENGL_ERROR();
#ifdef DEBUG
    if (location == -1)
        warnx("Could not find uniform %s", name.c_str());
#endif
    glUniform1i(location, value);TEST_OPENGL_ERROR();
    glUseProgram(0);
}

void program::setPlane(const glm::vec4 &plane) noexcept {
    use();
    GLint location = glGetUniformLocation(program_id, "plane");
    TEST_OPENGL_ERROR();
#ifdef DEBUG
    if (location == -1)
        warnx("Could not find uniform plane");
#endif
    glUniform4fv(location, 1, glm::value_ptr(plane));
    TEST_OPENGL_ERROR();
    glUseProgram(0);
}

void program::setTime(const float time) noexcept {
    use();
    GLint location = glGetUniformLocation(program_id, "time");
    TEST_OPENGL_ERROR();
#ifdef DEBUG
    if (location == -1)
        warnx("Could not find uniform time");
#endif
    glUniform1f(location, time);
    TEST_OPENGL_ERROR();
    glUseProgram(0);
}

void program::setCamPos(const glm::vec3 &pos) noexcept {
    use();
    GLint location = glGetUniformLocation(program_id, "camPos");
    TEST_OPENGL_ERROR();
#ifdef DEBUG
    if (location == -1)
        warnx("Could not find uniform camPos");
#endif
    glUniform3fv(location, 1, glm::value_ptr(pos));
    TEST_OPENGL_ERROR();
    glUseProgram(0);
}
