#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <iostream>

#include "matrix.hh"
#include "object_vbo.hh"
#include "program.hh"

GLuint teapot_vao_id;

shared_program prog = nullptr;

void window_resize(int width, int height) {
    //std::cout << "glViewport(0,0,"<< width << "," << height << ");TEST_OPENGL_ERROR();" << std::endl;
    glViewport(0,0,width,height);TEST_OPENGL_ERROR();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    TEST_OPENGL_ERROR();
    glBindVertexArray(teapot_vao_id);
    TEST_OPENGL_ERROR();

    glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size());
    TEST_OPENGL_ERROR();
    glBindVertexArray(0);
    TEST_OPENGL_ERROR();

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

bool init_object(shared_program prog)
{
    constexpr int max_nb_vbo = 3;
    int nb_vbo = 0;
    GLuint vbo_ids[max_nb_vbo];

    GLint vertex_location = glGetAttribLocation(prog->get_program_id(),"position");TEST_OPENGL_ERROR();
    GLint normal_flat_location = glGetAttribLocation(prog->get_program_id(),"normalFlat");TEST_OPENGL_ERROR();
    GLint normal_smooth_location = glGetAttribLocation(prog->get_program_id(),"normalSmooth");TEST_OPENGL_ERROR();

    glGenVertexArrays(1, &teapot_vao_id);
    TEST_OPENGL_ERROR();
    glBindVertexArray(teapot_vao_id);
    TEST_OPENGL_ERROR();

    nb_vbo = vertex_location != -1 ? nb_vbo + 1 : nb_vbo;
    nb_vbo = normal_flat_location != -1 ? nb_vbo + 1 : nb_vbo;
    nb_vbo = normal_smooth_location != -1 ? nb_vbo + 1 : nb_vbo;
    glGenBuffers(nb_vbo, vbo_ids);TEST_OPENGL_ERROR();

    int id_vbo = 0;
    if (vertex_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[id_vbo++]);TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(float), vertex_buffer_data.data(), GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(vertex_location);TEST_OPENGL_ERROR();
    }
    if (normal_flat_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[id_vbo++]);TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, normal_flat_buffer_data.size() * sizeof(float), normal_flat_buffer_data.data(), GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(normal_flat_location, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(normal_flat_location);TEST_OPENGL_ERROR();
    }
    if (normal_smooth_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[id_vbo++]);TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, normal_smooth_buffer_data.size() * sizeof(float), normal_smooth_buffer_data.data(), GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(normal_smooth_location, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(normal_smooth_location);TEST_OPENGL_ERROR();
    }

    glBindVertexArray(0);
    TEST_OPENGL_ERROR();

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
    scale = scale > 3.0 ? scale - 3.0 : scale;
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

    if (!init_object(prog))
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