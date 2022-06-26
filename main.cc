#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <err.h>

#include "matrix.hh"
#include "object_vbo.hh"
#include "program.hh"
#include "obj_raw.hh"
#include "camera.hh"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

std::map<std::string, shared_program> progMap;

glm::vec3 cam_pos = glm::vec3(10.0f, 0.0f, 10.0f);
glm::vec3 cam_target = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cam_dir = glm::normalize(cam_target - cam_pos);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cam_right = glm::normalize(glm::cross(cam_dir, up));
glm::vec3 cam_up = glm::normalize(glm::cross(cam_right, cam_dir));

Camera camera(cam_pos, cam_up, 225.0f, 0.0f, SPEED, SENSITIVITY, ZOOM); //TODO: Revoir alphaY, alphaX

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

#define MAX_DX 5.0f
#define MAX_DY 5.0f

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

void processSpecialKeys(int key, int x, int y) {

    camera.ProcessKeyboard(key);

    for (const auto &[name, prog]: progMap) {
        GLint model_view_matrix =
                glGetUniformLocation(prog->get_program_id(), "model_view_matrix");
        TEST_OPENGL_ERROR();
        if (model_view_matrix == -1)
            errx(1, "Could not find uniform model_view_matrix");

        glUniformMatrix4fv(model_view_matrix, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        TEST_OPENGL_ERROR();
    }
}

void processMotion(int x, int y) {
    float dx = x - lastX;
    float dy = y - lastY;
    lastX = x;
    lastY = y;

    if (abs(dx) > MAX_DX || abs(dy) > MAX_DY)
        return;

    camera.ProcessMouseMovement(dx, dy, false);

    for (const auto &[name, prog]: progMap) {
        GLint model_view_matrix =
                glGetUniformLocation(prog->get_program_id(), "model_view_matrix");
        TEST_OPENGL_ERROR();
        if (model_view_matrix == -1)
            errx(1, "Could not find uniform model_view_matrix");

        glUniformMatrix4fv(model_view_matrix, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        TEST_OPENGL_ERROR();
    }
}

void init_glut(int &argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("MY BIG WATER");
    glutDisplayFunc(display);
    glutReshapeFunc(window_resize);
    glutSpecialFunc(processSpecialKeys);
    glutMotionFunc(processMotion);
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
    // init camera
    GLint model_view_matrix =
        glGetUniformLocation(prog->get_program_id(), "model_view_matrix");
    TEST_OPENGL_ERROR();
    if (model_view_matrix == -1)
        errx(1, "Could not find uniform model_view_matrix");

    glUniformMatrix4fv(model_view_matrix, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
    TEST_OPENGL_ERROR();

    glm::mat4 projection = glm::perspective(glm::radians(30.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 400.0f);
    GLint projection_matrix =
        glGetUniformLocation(prog->get_program_id(), "projection_matrix");
    TEST_OPENGL_ERROR();
    if (projection_matrix == -1)
        errx(1, "Could not find uniform projection_matrix");

    glUniformMatrix4fv(projection_matrix, 1, GL_FALSE,
                       glm::value_ptr(projection));
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

/*
    float scale = (unsigned int)value / 100.0;
    float r = scale < 1.0 ? scale : 0.0;
    float g = scale >= 1.0 && scale < 2.0 ? scale - 1.0 : 0.0;
    float b = scale >= 2.0 ? scale - 2.0 : 0.0;

    progMap.begin()->second->use();
    glm::vec3 color_vec(r, g, b);
    GLuint color_location = glGetUniformLocation(progMap.begin()->second->get_program_id(), "Kd");
    TEST_OPENGL_ERROR();
    glUniform3fv(color_location, 1, glm::value_ptr(color_vec));
    TEST_OPENGL_ERROR();
*/

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