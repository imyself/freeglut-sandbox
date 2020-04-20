//#include <stdio.h>
//#include <GL/glut.h>
//#include <math.h>
//#define PI 3.142857

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shaderprogram.h"
#include "linear_alg.h"
#include "camera.h"
#include "ray.h"
#include "global_defines.h"
#define WINDOW_TITLE_PREFIX "Chapter 1"

// To figure out how to compile and stuff
//https://www.geeksforgeeks.org/getting-started-with-opengl/

// For more extensive tutorials
//http://www.lighthouse3d.com/tutorials/glut-tutorial/

// This code is from
// https://openglbook.com/chapter-1-getting-started.html
int
        CurrentWidth = 800,
        CurrentHeight = 600,
        WindowHandle = 0;

void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);
void MouseFunction(int, int, int, int);

ShaderProgram prog_lambert;
Drawable geom_tri;
Drawable geom_sphere;
Camera camera;
GLuint vao;

int main(int argc, char* argv[])
{
    Initialize(argc, argv);

#if CLION
    prog_lambert = init_ShaderProgram("../lambert.vert.glsl", "../lambert.frag.glsl");
#else
    prog_lambert = init_ShaderProgram("lambert.vert.glsl", "lambert.frag.glsl");
#endif

    geom_tri = init_Drawable();
    geom_sphere = init_Drawable();
    create_triangle_vbo(&geom_tri);
    create_sphere_vbo(&geom_sphere);
    camera = init_Camera(init_vec4(0,0,5,1), init_vec4(0,0,0,1), 45.f * TO_RADIANS,
                          0.001f, 1000.f, CurrentWidth / (float)CurrentHeight);
    mat4 viewProj = getViewProj(&camera);
    setUniformMat4(&prog_lambert, prog_lambert.unifViewProj, &viewProj);

    // I almost forgot the VAO!
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    glutMainLoop();

    exit(EXIT_SUCCESS);
}

void Initialize(int argc, char* argv[]) {
    GLenum GlewInitResult;

    InitWindow(argc, argv);

    GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult) {
        fprintf(
                stderr,
                "ERROR: %s\n",
                glewGetErrorString(GlewInitResult)
        );
        exit(EXIT_FAILURE);
    }

    fprintf(
            stdout,
            "INFO: OpenGL Version: %s\n",
            glGetString(GL_VERSION)
    );

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
}

void InitWindow(int argc, char* argv[])
{
    glutInit(&argc, argv);

    // Set the major and minor version of OpenGL that this program asks for.
    // Set to 3.30 for now since this VM doesn't like 4.0
    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutSetOption(
            GLUT_ACTION_ON_WINDOW_CLOSE,
            GLUT_ACTION_GLUTMAINLOOP_RETURNS
    );

    glutInitWindowSize(CurrentWidth, CurrentHeight);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

    if(WindowHandle < 1) {
        fprintf(
                stderr,
                "ERROR: Could not create a new rendering window.\n"
        );
        exit(EXIT_FAILURE);
    }

    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(RenderFunction);
    glutMouseFunc(MouseFunction);
}

void ResizeFunction(int Width, int Height) {
    CurrentWidth = Width;
    CurrentHeight = Height;
    glViewport(0, 0, CurrentWidth, CurrentHeight);
}

void RenderFunction(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.eye = mult_mat4_vec4(rotate(0.25 * TO_RADIANS, 0, 1, 0), camera.eye);
    mat4 viewProj = getViewProj(&camera);
    setUniformMat4(&prog_lambert, prog_lambert.unifViewProj, &viewProj);

//    draw(&prog_lambert, &geom_tri);
    draw(&prog_lambert, &geom_sphere);
    glutSwapBuffers();
    glutPostRedisplay();
}

// state is GLUT_UP or GLUT_DOWN
void MouseFunction(int button, int state, int x, int y) {
    float ndc_x = x / (float)CurrentWidth * 2.f - 1.f;
    float ndc_y = 1.f - y / (float)CurrentHeight * 2.f;
    if(button == GLUT_LEFT_BUTTON) {
        Ray r = raycast(&camera, ndc_x, ndc_y);
    }
    else if(button == GLUT_RIGHT_BUTTON) {
        ;
    }
    else if(button == GLUT_MIDDLE_BUTTON) {
        ;
    }
}