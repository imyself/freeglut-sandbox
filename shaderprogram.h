#pragma once
//#include <GL/glut.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "drawable.h"
#include "linear_alg.h"

typedef struct shader_program_t {
    GLuint vertShader, fragShader;
    GLuint prog;

    int attrPos;
    int attrNor;
    int attrCol;

    int unifViewProj;

} ShaderProgram;

ShaderProgram init_ShaderProgram(const char *vertfile, const char *fragfile);
ShaderProgram* new_ShaderProgram(const char *vertfile, const char *fragfile);

void setUniformMat4(ShaderProgram *s, int unifHandle, mat4 *m);

void initShaderVarHandles(ShaderProgram *s);
void draw(ShaderProgram *s, Drawable *d);

void printShaderInfoLog(int shader);
void printLinkInfoLog(int prog);