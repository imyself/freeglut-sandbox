#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdbool.h>
#include "global_defines.h"

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

void buffer_to_gpu(Drawable *d, int count, GLuint *idx, float *pos, int posCount, float *nor, int norCount, float *col, int colCount);
void create_triangle_vbo(Drawable *d);
void create_sphere_vbo(Drawable *d);
void create_subdiv_square_vbo(Drawable *d, int subdivisions);
void create_octahedron_vbo(Drawable *d);
void create_icosphere_vbo(Drawable *d, int subdivisions);