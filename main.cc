#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <err.h>

#include "matrix.hh"
#include "object_vbo.hh"
#include "program.hh"
#include "obj_raw.hh"

std::map<std::string, shared_program> progMap;

void window_resize(int width, int height) {
    glViewport(0,0,width,height);TEST_OPENGL_ERROR();
}

void display()
{
    for (auto &[name, prog]: progMap) {
        for (auto &[objName, obj]: prog->get_objects()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            TEST_OPENGL_ERROR();
            glBindVertexArray(obj.get_vao_id());
            TEST_OPENGL_ERROR();

            glDrawArrays(GL_TRIANGLES, 0,
                         obj.vbo_data_map.at(obj.vbo_ids_map.at("position")).size());
            TEST_OPENGL_ERROR();
            glBindVertexArray(0);
            TEST_OPENGL_ERROR();
        }
    }

    glutSwapBuffers();
    TEST_OPENGL_ERROR();
}

void init_glut(int &argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("MY BIG WATER");
    glutDisplayFunc(display);
    glutReshapeFunc(window_resize);
}

bool init_glew()
{
    return (glewInit() == GLEW_OK);
}

void init_GL()
{
    glEnable(GL_DEPTH_TEST);
    TEST_OPENGL_ERROR();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    TEST_OPENGL_ERROR();

    glEnable(GL_CULL_FACE);
    TEST_OPENGL_ERROR();

    glClearColor(0.15,0.15,0.15,1.0);
    TEST_OPENGL_ERROR();

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    TEST_OPENGL_ERROR();
}

void initObjects(shared_program prog, const std::vector<obj_raw::objRaw> &vaos)
{
    for (const auto &vao : vaos)
    {
        prog->add_object(vao.name, 2);
        prog->add_object_vbo(vao.name, "position", vao.vecs.at("position"), 3);
        prog->add_object_vbo(vao.name, "normal", vao.vecs.at("normal"), 3);
    }
}

void initUniforms(shared_program prog, const obj_raw::objRaw &material)
{
    //mygl::matrix4 mat = mygl::matrix4(
    //    0.57735, -0.33333, 0.57735, 0.00000, 0.00000, 0.66667, 0.57735, 0.00000,
    //    -0.57735, -0.33333, 0.57735, 0.00000, 0.00000, 0.00000, -17, 1.00000);
    const auto &mat = mygl::matrix4::lookat(68.0f, 0.0f, 32.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    GLint model_view_matrix =
        glGetUniformLocation(prog->get_program_id(), "model_view_matrix");
    TEST_OPENGL_ERROR();
    if (model_view_matrix == -1)
        errx(1, "Could not find uniform model_view_matrix");

    glUniformMatrix4fv(model_view_matrix, 1, GL_FALSE, glm::value_ptr(mat.mat));
    TEST_OPENGL_ERROR();

    //mygl::matrix4 mat_2 =
    //    mygl::matrix4(5.00000, 0.00000, 0.00000, 0.00000, 0.00000, 5.00000,
    //                  0.00000, 0.00000, 0.00000, 0.00000, -1.00020, -1.00000,
    //                  0.00000, 0.00000, -10.00100, 0.00000);
    const auto &mat_2 = mygl::matrix4::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 300.0f);
    GLint projection_matrix =
        glGetUniformLocation(prog->get_program_id(), "projection_matrix");
    TEST_OPENGL_ERROR();
    if (projection_matrix == -1)
        errx(1, "Could not find uniform projection_matrix");

    glUniformMatrix4fv(projection_matrix, 1, GL_FALSE,
                       glm::value_ptr(mat_2.mat));
    TEST_OPENGL_ERROR();

    for (const auto &[name, vec] : material.vecs) {
        GLint uniform_location =
            glGetUniformLocation(prog->get_program_id(), name.c_str());
        TEST_OPENGL_ERROR();
        if (uniform_location == -1)
            warnx("Could not find uniform %s", name.c_str());

        if (name != "Ns") {
            glUniform3fv(uniform_location, 1, vec.data());
            TEST_OPENGL_ERROR();
        } else {
            glUniform1f(uniform_location, vec[0]);
            TEST_OPENGL_ERROR();
        }
    }
}

void update(int value)
{
    value = value % 300;
    glutTimerFunc(1000/60, update, ++value);

    float scale = (unsigned int)value / 100.0;
    float r = scale < 1.0 ? scale : 0.0;
    float g = scale >= 1.0 && scale < 2.0 ? scale - 1.0 : 0.0;
    float b = scale >= 2.0 ? scale - 2.0 : 0.0;

    glm::vec3 color_vec(r, g, b);
    GLuint color_location = glGetUniformLocation(progMap.begin()->second->get_program_id(), "color");
    TEST_OPENGL_ERROR();
    glUniform3fv(color_location, 1, glm::value_ptr(color_vec));
    TEST_OPENGL_ERROR();

    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        errx(1, "Usage : %s [filename]", argv[0]);

    init_glut(argc, argv);

    if (!init_glew())
        errx(1, "Could not initialize glew");

    init_GL();

    const auto &matMap = obj_raw::getMap(argv[1]);

    for (const auto &[mat, meshes] : matMap) {
        progMap[mat.name] = program::make_program("shaders/vertex.vert", "shaders/fragment.frag");
        progMap[mat.name]->use();
        initUniforms(progMap[mat.name], mat);
        initObjects(progMap[mat.name], meshes);
    }
    glutTimerFunc(1000/60, update, 0);
    glutMainLoop();

    return 0;
}