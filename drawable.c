#include "drawable.h"
#include <math.h>
#include <string.h>
#include "linear_alg.h"

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


void buffer_to_gpu(Drawable *d, int count, GLuint *idx, float *pos, int posCount, float *nor, int norCount, float *col, int colCount) {
    d->count = count;

    generateIdx(d);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d->bufIdx);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), idx, GL_STATIC_DRAW);

    if(pos) {
        generatePos(d);
        glBindBuffer(GL_ARRAY_BUFFER, d->bufPos);
        glBufferData(GL_ARRAY_BUFFER, posCount * sizeof(float), pos, GL_STATIC_DRAW);
    }
    if(nor) {
        generateNor(d);
        glBindBuffer(GL_ARRAY_BUFFER, d->bufNor);
        glBufferData(GL_ARRAY_BUFFER, norCount * sizeof(float), nor, GL_STATIC_DRAW);
    }
    if(col) {
        generateCol(d);
        glBindBuffer(GL_ARRAY_BUFFER, d->bufCol);
        glBufferData(GL_ARRAY_BUFFER, colCount * sizeof(float), col, GL_STATIC_DRAW);
    }
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

    buffer_to_gpu(d, 3, idx, pos, 9, NULL, 0, col, 9);
}

#define SPH_IDX_COUNT 2280
#define SPH_VERT_COUNT 382

void createSphereVertexPositions(float *sph_vert_pos) {
    // Create rings of vertices for the non-pole vertices
    // These will fill indices 1 - 380. Indices 0 and 381 will be filled by the two pole vertices.
    vec4 v;
    // i is the Z axis rotation
    for (int i = 1; i < 20; i++) {
        // j is the Y axis rotation
        for (int j = 0; j < 20; j++){
            mat4 rotate1 = rotate(j * 18.0f * TO_RADIANS, 0, 1, 0);
            mat4 rotate2 = rotate(i * -9.0f * TO_RADIANS, 0, 0, 1);
            v = mult_mat4_vec4(mult_mat4(rotate1, rotate2), init_vec4(0, 0.5f, 0, 1));
            int base_idx = ((i - 1) * 20 + j + 1) * 3;
            sph_vert_pos[base_idx + 0] = v.data[0];
            sph_vert_pos[base_idx + 1] = v.data[1];
            sph_vert_pos[base_idx + 2] = v.data[2];
        }
    }

    // Add the pole vertices
    sph_vert_pos[0] = 0;
    sph_vert_pos[1] = 0.5f;
    sph_vert_pos[2] = 0;

    sph_vert_pos[381 * 3] = 0;
    sph_vert_pos[381 * 3 + 1] = -0.5f;
    sph_vert_pos[381 * 3 + 2] = 0;
}


void createSphereVertexNormals(float *sph_vert_nor) {
    // Unlike a cylinder, a sphere only needs to be one normal per vertex
    // because a sphere does not have sharp edges.
    vec4 v;

    // i is the Z axis rotation
    for (int i = 1; i < 20; i++){
        // j is the Y axis rotation
        for (int j = 0; j < 20; j++) {
            mat4 rotate1 = rotate(j * 18.0f * TO_RADIANS, 0, 1, 0);
            mat4 rotate2 = rotate(i * -9.0f * TO_RADIANS, 0, 0, 1);
            v = mult_mat4_vec4(mult_mat4(rotate1, rotate2), init_vec4(0, 1, 0, 0));
            int base_idx = ((i - 1) * 20 + j + 1) * 3;
            sph_vert_nor[base_idx + 0] = v.data[0];
            sph_vert_nor[base_idx + 1] = v.data[1];
            sph_vert_nor[base_idx + 2] = v.data[2];
        }
    }
    // Add the pole normals
    sph_vert_nor[0] = 0;
    sph_vert_nor[1] = 1.f;
    sph_vert_nor[2] = 0;

    sph_vert_nor[381 * 3] = 0;
    sph_vert_nor[381 * 3 + 1] = -1.f;
    sph_vert_nor[381 * 3 + 2] = 0;
}

void createSphereIndices(GLuint *sph_idx) {
    int index = 0;
    // Build indices for the top cap (20 tris, indices 0 - 60, up to vertex 20)
    for (int i = 0; i < 19; i++) {
        sph_idx[index] = 0;
        sph_idx[index + 1] = i + 1;
        sph_idx[index + 2] = i + 2;
        index += 3;
    }

    // Must create the last triangle separately because its indices loop
    sph_idx[57] = 0;
    sph_idx[58] = 20;
    sph_idx[59] = 1;
    index += 3;

    // Build indices for the body vertices
    // i is the Z axis rotation
    for (int i = 1; i < 19; i++) {
        //j is the Y axis rotation
        for (int j = 0; j < 20; j++) {
            sph_idx[index] = (i - 1)*20 + j + 1;
            sph_idx[index + 1] = (i - 1)*20 +  j + 2;
            sph_idx[index + 2] = (i - 1)*20 +  j + 22;
            sph_idx[index + 3] = (i - 1)*20 +  j + 1;
            sph_idx[index + 4] = (i - 1)*20 +  j + 22;
            sph_idx[index + 5] = (i - 1)*20 +  j + 21;
            index += 6;
        }
    }

    // Build indices for the bottom cap (20 tris, indices 2220 - 2279)
    for (int i = 0; i < 19; i++) {
        sph_idx[index] = 381;
        sph_idx[index + 1] = i+361;
        sph_idx[index + 2] = i+362;
        index += 3;
    }

    // Must create the last triangle separately because its indices loop
    sph_idx[2277] = 381;
    sph_idx[2278] = 380;
    sph_idx[2279] = 361;
    index += 3;
}

void create_sphere_vbo(Drawable *d) {
    GLuint sph_idx[SPH_IDX_COUNT];
    float sph_vert_pos[SPH_VERT_COUNT * 3];
    float sph_vert_nor[SPH_VERT_COUNT * 3];

    createSphereVertexPositions(sph_vert_pos);
    createSphereVertexNormals(sph_vert_nor);
    createSphereIndices(sph_idx);

    buffer_to_gpu(d, SPH_IDX_COUNT, sph_idx, sph_vert_pos, SPH_VERT_COUNT * 3, sph_vert_nor, SPH_VERT_COUNT * 3, NULL, 0);
}

void create_subdiv_square_vbo(Drawable *d, int subdivisions) {
    float scale[2] = {1.f, 1.f};
    float center[3] = {0.f, 0.f, 0.f};

    int width = pow(2.0, subdivisions / 2.0);
    float normalize = 1.f / width;
    float positions[(width + 1) * (width + 1) * 3];
    float normals[(width + 1) * (width + 1) * 3];
    GLuint indices[width * width * 6];

    int posIdx = 0;
    for(int x = 0; x <= width; ++x) {
        for (int z = 0; z <= width; ++z) {
            normals[posIdx] = 0;
            positions[posIdx++] = x * normalize * scale[0] + center[0] - scale[0] * 0.5;
            normals[posIdx] = 1;
            positions[posIdx++] = 0 + center[1];
            normals[posIdx] = 0;
            positions[posIdx++] = z * normalize * scale[1] + center[2] - scale[1] * 0.5;
            normals[posIdx] = 0;
            positions[posIdx++] = 1;
        }
    }

    int indexIdx = 0;
    for(int i = 0; i < width; ++i) {
        for (int j = 0; j < width; ++j) {
            indices[indexIdx++] = j + i * (width + 1);
            indices[indexIdx++] = j + 1 + i * (width + 1);
            indices[indexIdx++] = j + (i + 1) * (width + 1);

            indices[indexIdx++] = j + 1 + i * (width + 1);
            indices[indexIdx++] = j + (i + 1) * (width + 1);
            indices[indexIdx++] = j + 1 + (i + 1) * (width + 1);
        }
    }

    buffer_to_gpu(d, width * width * 6, indices,
            positions, (width + 1) * (width + 1) * 3,
            normals, (width + 1) * (width + 1) * 3,
            NULL, 0);
}


void create_octahedron_vbo(Drawable *d) {
    vec4 points[6] = {
            init_vec4(1, 0, 0, 1),
            init_vec4(-1, 0, 0, 1),
            init_vec4(0, 0, -1, 1),
            init_vec4(0, 0, 1, 1),
            init_vec4(0, 1, 0, 1),
            init_vec4(0, -1, 0, 1)
    };

    float vertices[24 * 3] = {
            // Face 1
            points[4].data[0], points[4].data[1], points[4].data[2],
            points[0].data[0], points[0].data[1], points[0].data[2],
            points[2].data[0], points[2].data[1], points[2].data[2],
            // Face 2
            points[4].data[0], points[4].data[1], points[4].data[2],
            points[2].data[0], points[2].data[1], points[2].data[2],
            points[1].data[0], points[1].data[1], points[1].data[2],
            // Face 3
            points[4].data[0], points[4].data[1], points[4].data[2],
            points[1].data[0], points[1].data[1], points[1].data[2],
            points[3].data[0], points[3].data[1], points[3].data[2],
            // Face 4
            points[4].data[0], points[4].data[1], points[4].data[2],
            points[3].data[0], points[3].data[1], points[3].data[2],
            points[0].data[0], points[0].data[1], points[0].data[2],
            // Face 5
            points[5].data[0], points[5].data[1], points[5].data[2],
            points[3].data[0], points[3].data[1], points[3].data[2],
            points[0].data[0], points[0].data[1], points[0].data[2],
    };
}

#if 0
void create_icosphere_vbo(Drawable *d, int subdivisions) {
    const float X = 0.525731112119133606f;
    const float Z = 0.850650808352039932f;
    const int N = 0;


    int maxIndexCount = 20 * (int)pow(4, subdivisions);
    int maxVertexCount = 10 * (int)pow(4, subdivisions) + 2;


    uint8_t *buffer0 = (uint8_t*) malloc(
            maxIndexCount * 3 * sizeof(GLuint) + // Index
            maxVertexCount * 3 * sizeof(float) + // Pos
            maxVertexCount * 3 * sizeof(float)); // Col or Nor

    uint8_t *buffer1 = (uint8_t*) malloc(maxIndexCount * 3 * sizeof(GLuint));

    uint8_t *buffers[2] = {buffer0, buffer1};

    int b = 0;

    int indexByteOffset = 0;
    int vertexByteOffset = maxIndexCount * 3 * sizeof(GLuint);
    int normalByteOffset = vertexByteOffset;
    int positionByteOffset = vertexByteOffset + maxVertexCount * 3 * sizeof(float);


#if 0

    // Create 3-float buffer views into the backing buffer to represent positions
    let vertices: Array<vec4> = new Array(12);
    for (let i = 0; i < 12; ++i) {
      vertices[i] = <vec4> new Float32Array(buffer0, vertexByteOffset + i * 4 * Float32Array.BYTES_PER_ELEMENT, 4);
    }
#endif

    GLuint **triangles = (GLuint**) malloc(sizeof(GLuint*) * 20);
    for(int i = 0; i < 20; ++i) {
        triangles[i] = (GLuint*) malloc(3 * sizeof(GLuint*));
        memcpy(triangles[i], &buffers[b][indexByteOffset + i * 3 * sizeof(GLuint)], 3 * sizeof(GLuint));
    }

    // 12 vec3s
    float *vertices = (float*) malloc(sizeof(float) * 3 * 12);
    for(int i = 0; i < 12; ++i) {
        vertices[i] =
    }
}
#endif