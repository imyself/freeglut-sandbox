#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdbool.h>

typedef struct drawable_t {
    int count;
    GLuint bufIdx, bufPos, bufNor, bufCol;
    bool idxGenerated, posGenerated, norGenerated, colGenerated;
} Drawable;

Drawable init_Drawable();
Drawable* new_Drawable();

void generateIdx(Drawable *d);
void generatePos(Drawable *d);
void generateNor(Drawable *d);
void generateCol(Drawable *d);

bool bindIdx(Drawable *d);
bool bindPos(Drawable *d);
bool bindNor(Drawable *d);
bool bindCol(Drawable *d);

void create_triangle_vbo(Drawable *d);