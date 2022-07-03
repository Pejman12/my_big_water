#include <iostream>
#include <err.h>

#include "obj_raw.hh"
#include "program.hh"
#include "camera.hh"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

#define MAX_DX 5.0f
#define MAX_DY 5.0f
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

std::map<std::string, shared_program> progMap;

Camera camera( 5.0f, 30.0f, 5.0f, 0.0f, 1.0f, 0.0f, 225.0f, 0.0f);
constexpr glm::vec3 lightPos(0.0f, 40.0f, 0.0f);

unsigned int uboMatrix;

void update_view() noexcept {
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrix);TEST_OPENGL_ERROR();
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                    glm::value_ptr(camera.getViewMatrix()));
    TEST_OPENGL_ERROR();
    glBindBuffer(GL_UNIFORM_BUFFER, 0);TEST_OPENGL_ERROR();
}

void update_projection(float fov, float aspect, float near, float far) noexcept {
    const auto &projection = glm::perspective(fov, aspect, near, far);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrix);TEST_OPENGL_ERROR();
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    TEST_OPENGL_ERROR();
    glBindBuffer(GL_UNIFORM_BUFFER, 0);TEST_OPENGL_ERROR();
}

void update_light() noexcept {
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrix);TEST_OPENGL_ERROR();
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(lightPos));
    TEST_OPENGL_ERROR();
    glBindBuffer(GL_UNIFORM_BUFFER, 0);TEST_OPENGL_ERROR();
}

void window_resize(int width, int height) noexcept {
    update_projection(glm::radians(30.0f), (float)width / (float)height, 0.1f, 200.0f);
    glViewport(0,0, width, height);
    TEST_OPENGL_ERROR();
}

void display() noexcept
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto &[name, prog]: progMap) {
        prog->use();
        prog->update_materials();
        for (const auto &[objName, obj]: prog->get_objects()) {
            TEST_OPENGL_ERROR();
            glBindVertexArray(obj->get_vao_id());
            TEST_OPENGL_ERROR();

            glDrawArrays(GL_TRIANGLES, 0,
                         obj->vbo_data_map.at(obj->vbo_ids_map.at("position")).size());
            TEST_OPENGL_ERROR();
        }
    }

    glutSwapBuffers();
    TEST_OPENGL_ERROR();
}

void processSpecialKeys(int key, int, int) noexcept {
    camera.processKeyboard(key);
    update_view();
}

void processMotion(int x, int y) noexcept {
    float dx = x - lastX;
    float dy = y - lastY;
    lastX = x;
    lastY = y;
    if (abs(dx) > MAX_DX || abs(dy) > MAX_DY)
        return;

    camera.processMouseMovement(dx, dy, true);
    update_view();
}

void processMouseScroll(int btn, int, int, int) noexcept {
    camera.processMouseScroll(btn);
    update_view();
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

void initObjects(shared_program prog, const std::vector<obj_raw::objRawPtr> &vaos)
{
    for (const auto vao : vaos)
    {
        prog->add_object(vao->name, 2);
        prog->add_object_vbo(vao->name, "position", vao->vecs.at("position"), 3);
        prog->add_object_vbo(vao->name, "normal", vao->vecs.at("normal"), 3);
    }
}

void initUBO()
{
    glGenBuffers(1, &uboMatrix);TEST_OPENGL_ERROR();
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrix);TEST_OPENGL_ERROR();
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec3), NULL, GL_STATIC_DRAW);TEST_OPENGL_ERROR();
    glBindBuffer(GL_UNIFORM_BUFFER, 0);TEST_OPENGL_ERROR();
    // define the range of the buffer that links to a uniform binding point
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrix, 0, 2 * sizeof(glm::mat4) + sizeof(glm::vec3));TEST_OPENGL_ERROR();

    update_view();
    update_projection(glm::radians(30.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
    update_light();
}

void update(int value) noexcept
{
    value = value % 300;
    glutTimerFunc(1000/60, update, ++value);

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

    for (auto &[mat, meshes] : matMap) {
        progMap[mat->name] = program::make_program("shaders/vertex.vert", "shaders/fragment.frag",
                                                   "Matrix", mat);
        initObjects(progMap[mat->name], meshes);
    }
    initUBO();
    glutTimerFunc(1000/60, update, 0);
    glutMainLoop();

    return 0;
}