#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <err.h>

#include "matrix.hh"
#include "object_vbo.hh"
#include "program.hh"
#include "obj_raw.hh"

GLuint teapot_vao_id;

shared_program prog = nullptr;

void window_resize(int width, int height) {
    glViewport(0,0,width,height);TEST_OPENGL_ERROR();
}

void display()
{
    for (auto &obj : prog->get_objects()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        TEST_OPENGL_ERROR();
        glBindVertexArray(obj.second.get_vao_id());
        TEST_OPENGL_ERROR();

        glDrawArrays(GL_TRIANGLES, 0,
                     obj.second.vbo_data_map.at(obj.second.vbo_ids_map.at("position")).size());
        TEST_OPENGL_ERROR();
        glBindVertexArray(0);
        TEST_OPENGL_ERROR();
    }

    glutSwapBuffers();
    TEST_OPENGL_ERROR();
}

bool init_glut(int &argc, char *argv[])
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

    return true;
}

bool init_glew()
{
    return (glewInit() == GLEW_OK);
}

bool init_GL()
{
    glEnable(GL_DEPTH_TEST);
    TEST_OPENGL_ERROR();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    TEST_OPENGL_ERROR();

    glEnable(GL_CULL_FACE);
    TEST_OPENGL_ERROR();

    glClearColor(0.25,0.25,0.25,1.0);
    TEST_OPENGL_ERROR();

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glPixelStorei(GL_PACK_ALIGNMENT,1);

    return true;
}

bool init_object(shared_program prog, const std::string filename)
{
    const auto obj = addObjs(filename);
    prog->add_object("teapot", 3);
    prog->add_object_vbo("teapot", "position", vertex_buffer_data, 3);
    prog->add_object_vbo("teapot", "normalFlat", normal_flat_buffer_data, 3);
    prog->add_object_vbo("teapot", "normalSmooth", normal_smooth_buffer_data, 3);
    return true;
}

bool init_POV(shared_program prog)
{
    mygl::matrix4 mat = mygl::matrix4(
        0.57735, -0.33333, 0.57735, 0.00000, 0.00000, 0.66667, 0.57735, 0.00000,
        -0.57735, -0.33333, 0.57735, 0.00000, 0.00000, 0.00000, -17, 1.00000);

    GLuint model_view_matrix =
        glGetUniformLocation(prog->get_program_id(), "model_view_matrix");
    TEST_OPENGL_ERROR();
    glUniformMatrix4fv(model_view_matrix, 1, GL_FALSE, glm::value_ptr(mat.mat));
    TEST_OPENGL_ERROR();

    mygl::matrix4 mat_2 =
        mygl::matrix4(5.00000, 0.00000, 0.00000, 0.00000, 0.00000, 5.00000,
                      0.00000, 0.00000, 0.00000, 0.00000, -1.00020, -1.00000,
                      0.00000, 0.00000, -10.00100, 0.00000);

    GLuint projection_matrix =
        glGetUniformLocation(prog->get_program_id(), "projection_matrix");
    TEST_OPENGL_ERROR();
    glUniformMatrix4fv(projection_matrix, 1, GL_FALSE,
                       glm::value_ptr(mat_2.mat));
    TEST_OPENGL_ERROR();

    return true;
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
    GLuint color_location = glGetUniformLocation(prog->get_program_id(), "color");
    TEST_OPENGL_ERROR();
    glUniform3fv(color_location, 1, glm::value_ptr(color_vec));
    TEST_OPENGL_ERROR();

    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        errx(1, "Usage : %s [filename]", argv[0]);
    }
    
    if (!init_glut(argc, argv))
    {
        TEST_OPENGL_ERROR();
        std::exit(-1);
    }

    if (!init_glew())
    {
        TEST_OPENGL_ERROR();
        std::exit(-1);
    }

    if (!init_GL())
    {
        TEST_OPENGL_ERROR();
        std::exit(-1);
    }

    prog = program::make_program("shaders/vertex.vert", "shaders/fragment.frag");
    prog->use();

    if (!init_object(prog, argv[1]))
    {
        TEST_OPENGL_ERROR();
        std::exit(-1);
    }

    if (!init_POV(prog))
    {
        TEST_OPENGL_ERROR();
        std::exit(-1);
    }

    glutTimerFunc(1000/60, update, 0);
    glutMainLoop();

    return 0;
}