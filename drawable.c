#include "drawable.h"

Drawable init_Drawable() {
    Drawable d;
    d.count = -1;
    d.bufPos = d.bufNor = d.bufCol = d.bufIdx = 0;
    d.posGenerated = d.norGenerated = d.colGenerated = d.idxGenerated = false;
    return d;
}

Drawable* new_Drawable() {
    Drawable *d = (Drawable*) malloc(sizeof(Drawable));
    *d = init_Drawable();
    return d;
}


void generateIdx(Drawable *d) {
    d->idxGenerated = true;
    glGenBuffers(1, &d->bufIdx);
}

void generatePos(Drawable *d) {
    d->posGenerated = true;
    glGenBuffers(1, &d->bufPos);
}

void generateNor(Drawable *d) {
    d->norGenerated = true;
    glGenBuffers(1, &d->bufNor);
}

void generateCol(Drawable *d) {
    d->colGenerated = true;
    glGenBuffers(1, &d->bufCol);
}

bool bindIdx(Drawable *d) {
    if(d->idxGenerated) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d->bufIdx);
    }
    return d->idxGenerated;
}

bool bindPos(Drawable *d) {
    if(d->posGenerated) {
        glBindBuffer(GL_ARRAY_BUFFER, d->bufPos);
    }
    return d->posGenerated;
}

bool bindNor(Drawable *d) {
    if(d->norGenerated) {
        glBindBuffer(GL_ARRAY_BUFFER, d->bufNor);
    }
    return d->norGenerated;
}

bool bindCol(Drawable *d) {
    if(d->colGenerated) {
        glBindBuffer(GL_ARRAY_BUFFER, d->bufCol);
    }
    return d->colGenerated;
}

// For now, just a screen-space triangle
void create_triangle_vbo(Drawable *d) {
    GLuint idx[] = {0, 1, 2};
    // vec3s
    float pos[] = {-1.f, -1.f, 0.f,
                   1.f, -1.f, 0.f,
                   0.f, 1.f, 0.f};
    float col[] = {1.f, 0.f, 0.f,
                   0.f, 0.f, 1.f,
                   0.f, 1.f, 0.f};

    d->count = 3;

    generateIdx(d);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d->bufIdx);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), idx, GL_STATIC_DRAW);
    generatePos(d);
    glBindBuffer(GL_ARRAY_BUFFER, d->bufPos);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), pos, GL_STATIC_DRAW);
    generateCol(d);
    glBindBuffer(GL_ARRAY_BUFFER, d->bufCol);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), col, GL_STATIC_DRAW);
}