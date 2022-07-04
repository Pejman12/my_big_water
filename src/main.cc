#include <iostream>
#include <err.h>
#include <GL/glew.h>

#include "engine/camera.hh"
#include "engine/scene.hh"
#include "water/water.hh"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

#define LIGHT_POS 0.0f, 40.0f, 0.0f
#define CAM_POS 5.0f, 30.0f, 5.0f
#define CAM_UP 0.0f, 1.0f, 0.0f
#define CAM_ANGLE 225.0f, 0.0f
#define FOV 30.0f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 200.0f

#define MAX_DX 10.0f
#define MAX_DY 10.0f

int width;
int height;
int lastX = SCR_WIDTH / 2.0f;
int lastY = SCR_HEIGHT / 2.0f;
shared_scene Scene = nullptr;
shared_water Water = nullptr;
shared_camera camera = std::make_shared<Camera>(CAM_POS, CAM_UP, CAM_ANGLE);
glm::vec3 lightPos;

unsigned int uboMatrix;

void update_view() noexcept {
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrix);TEST_OPENGL_ERROR();
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                    glm::value_ptr(camera->getViewMatrix()));
    TEST_OPENGL_ERROR();
    glBindBuffer(GL_UNIFORM_BUFFER, 0);TEST_OPENGL_ERROR();
}
void update_projection(int width_, int height_) noexcept {
    width = width_ < 1 ? 1 : width_;
    height = height_ < 1 ? 1 : height_;

    const auto &projection = glm::perspective(glm::radians(FOV), (float)width / (float)height, NEAR_PLANE, FAR_PLANE);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrix);TEST_OPENGL_ERROR();
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    TEST_OPENGL_ERROR();
    glBindBuffer(GL_UNIFORM_BUFFER, 0);TEST_OPENGL_ERROR();
}

void update_light(const glm::vec3 &lightPos_) noexcept {
    lightPos = lightPos_;

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrix);TEST_OPENGL_ERROR();
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(lightPos));
    TEST_OPENGL_ERROR();
    glBindBuffer(GL_UNIFORM_BUFFER, 0);TEST_OPENGL_ERROR();
}

void window_resize(int width, int height) noexcept {
    update_projection(width, height);
    glViewport(0,0, width, height);
    TEST_OPENGL_ERROR();
}

void processSpecialKeys(int key, int, int) noexcept {
    camera->processKeyboard(key);
    update_view();
}

void processMotion(int x, int y) noexcept {
    float dx = x - lastX;
    float dy = y - lastY;
    lastX = x;
    lastY = y;
    if (abs(dx) > MAX_DX || abs(dy) > MAX_DY)
        return;

    camera->processMouseMovement(dx, dy, true);
    update_view();
}

void processMouseScroll(int btn, int, int, int) noexcept {
    camera->processMouseScroll(btn);
    update_view();
}

void draw() noexcept {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Scene->draw();
    Water->draw();

    glutSwapBuffers();
    TEST_OPENGL_ERROR();
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
    glutDisplayFunc(draw);
    glutReshapeFunc(window_resize);
    glutSpecialFunc(processSpecialKeys);
    glutMotionFunc(processMotion);
    glutMouseFunc(processMouseScroll);
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

    //glEnable(GL_CULL_FACE);
    TEST_OPENGL_ERROR();

    glClearColor(0.15,0.15,0.15,1.0);
    TEST_OPENGL_ERROR();

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    TEST_OPENGL_ERROR();
}

void update(int value) noexcept
{
    value = value % 300;
    glutTimerFunc(1000/60, update, ++value);

    glutPostRedisplay();
}

void initUBO() {
    //init ubo
    glGenBuffers(1, &uboMatrix);TEST_OPENGL_ERROR();
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrix);TEST_OPENGL_ERROR();
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec3), NULL, GL_STATIC_DRAW);TEST_OPENGL_ERROR();
    glBindBuffer(GL_UNIFORM_BUFFER, 0);TEST_OPENGL_ERROR();
    // define the range of the buffer that links to a uniform binding point
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrix, 0, 2 * sizeof(glm::mat4) + sizeof(glm::vec3));TEST_OPENGL_ERROR();
    update_view();
    update_projection(SCR_WIDTH, SCR_HEIGHT);
    update_light(glm::vec3(LIGHT_POS));
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
    Scene = std::make_shared<scene>(matMap);
    Water = std::make_shared<water>(matMap);
    initUBO();
    glutTimerFunc(1000/60, update, 0);
    glutMainLoop();

    return 0;
}