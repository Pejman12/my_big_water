#include <iostream>
#include <err.h>

#include "obj_raw.hh"
#include "program.hh"
#include "camera.hh"

#include "scene.hh"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

#define LIGHT_POS 0.0f, 40.0f, 0.0f
#define CAM_POS 5.0f, 30.0f, 5.0f
#define CAM_UP 0.0f, 1.0f, 0.0f
#define CAM_ANGLE 225.0f, 0.0f

shared_scene Scene = nullptr;

void init_glut(int &argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("MY BIG WATER");
    glutDisplayFunc([](){Scene->draw();});
    glutReshapeFunc([](int width, int height){Scene->window_resize(width, height);});
    glutSpecialFunc([](int key, int, int){Scene->processSpecialKeys(key);});
    glutMotionFunc([](int x, int y){Scene->processMotion(x, y);});
    glutMouseFunc([](int btn, int, int, int){Scene->processMouseScroll(btn);});
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

int main(int argc, char *argv[])
{
    if (argc != 2)
        errx(1, "Usage : %s [filename]", argv[0]);

    init_glut(argc, argv);
    if (!init_glew())
        errx(1, "Could not initialize glew");
    init_GL();

    Scene = std::make_shared<scene>(LIGHT_POS, CAM_POS, CAM_UP, CAM_ANGLE,
                                                 SCR_WIDTH, SCR_HEIGHT, argv[1]);
    glutTimerFunc(1000/60, update, 0);
    glutMainLoop();

    return 0;
}