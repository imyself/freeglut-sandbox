#include "linear_alg.h"
#include <string.h>
#include <math.h>


vec4 init_vec4(float x, float y, float z, float w) {
    vec4 v = {x, y, z, w};
    return v;
}

float dot(vec4 a, vec4 b) {
    float sum = 0.f;
    for(int i = 0; i < 4; ++i) {
        sum += a.data[i] * b.data[i];
    }
    return sum;
}


vec4 sum(vec4 a, vec4 b) {
    vec4 v = {a.data[0] + b.data[0],
              a.data[1] + b.data[1],
              a.data[2] + b.data[2],
              a.data[3] + b.data[3]};
    return v;
}

vec4 diff(vec4 a, vec4 b) {
    vec4 v = {a.data[0] - b.data[0],
            a.data[1] - b.data[1],
            a.data[2] - b.data[2],
            a.data[3] - b.data[3]};
    return v;
}

vec4 normalize(vec4 v) {
    float len = length(v);
    vec4 v2 = {v.data[0] / len, v.data[1] / len, v.data[2] / len, v.data[3] / len};
    return v2;
}


vec4 cross(vec4 a, vec4 b) {
    vec4 v = {a.data[1] * b.data[2] - a.data[2] * b.data[1],
              a.data[2] * b.data[0] - a.data[0] * b.data[2],
              a.data[0] * b.data[1] - a.data[1] * b.data[0],
              0.f};
    return v;
}

float length(vec4 v) {
    return sqrt(v.data[0] * v.data[0] +
                        v.data[1] * v.data[1] +
                        v.data[2] * v.data[2] +
                        v.data[3] * v.data[3]);
}


vec4 mult_vec4_flt(vec4 v, float c) {
    vec4 result = {v.data[0] * c, v.data[1] * c, v.data[2] * c, v.data[3] * c};
    return result;
}

mat4 identity() {
    mat4 m;
    bzero(m.data, 16 * sizeof(float));
    m.data[0].data[0] = 1.f;
    m.data[1].data[1] = 1.f;
    m.data[2].data[2] = 1.f;
    m.data[3].data[3] = 1.f;
    return m;
}

mat4 translate(float x, float y, float z) {
    mat4 m = identity();
    m.data[3].data[0] = x;
    m.data[3].data[1] = y;
    m.data[3].data[2] = z;
    return m;
}

mat4 rotate(float rad, float x, float y, float z) {
    float cs = cos(rad);
    float sn = sin(rad);

    mat4 m = {cs + x*x*(1 - cs), z*sn + x*y*(1 - cs), -y*sn + x*z*(1 - cs), 0.f,  // Col 0
              x*y*(1 - cs) - z*sn, cs + y*y*(1 - cs), x*sn + y*z*(1 - cs), 0.f,   // Col 1
              y*sn + x*z*(1 - cs), -x*sn + y*z*(1 - cs), cs + z*z*(1 - cs), 0.f,  // Col 2
              0.f,                 0.f,                  0.f,               1.f}; // Col 3
    return m;
}
mat4 scale(float x, float y, float z) {
    mat4 m = identity();
    m.data[0].data[0] = x;
    m.data[1].data[1] = y;
    m.data[2].data[2] = z;
    return m;
}

mat4 viewMat(vec4 R, vec4 U, vec4 F, vec4 eye) {
    mat4 orient = {R.data[0], U.data[0], F.data[0], 0.f,
                   R.data[1], U.data[1], F.data[1], 0.f,
                   R.data[2], U.data[2], F.data[2], 0.f,
                   0.f, 0.f, 0.f, 1.f};
    mat4 tr = translate(-eye.data[0], -eye.data[1], -eye.data[2]);
    return mult_mat4(orient, tr);
}

mat4 projMat(float fovy, float nc, float fc, float aspect) {
    mat4 m = identity();
    float S = 1.0 / tan(fovy / 2.f);
    m.data[0].data[0] = S / aspect;
    m.data[1].data[1] = S;
    m.data[2].data[2] = fc / (fc - nc);
    m.data[2].data[3] = 1.f;
    m.data[3].data[2] = -fc * nc / (fc - nc);
    m.data[3].data[3] = 0.f;
    return m;
}

mat4 mult_mat4(mat4 m1, mat4 m2) {
    mat4 m;
    for(int c = 0; c < 4; ++c) {
        for(int r = 0; r < 4; ++r) {
            m.data[c].data[r] = dot(row(&m1, r), col(&m2, c));
        }
    }
    return m;
}

vec4 mult_mat4_vec4(mat4 m, vec4 v) {
    vec4 result = {dot(row(&m, 0), v),
                   dot(row(&m, 1), v),
                   dot(row(&m, 2), v),
                   dot(row(&m, 3), v)};
    return result;
}

vec4 col(mat4 *m, unsigned int i) {
    return m->data[i];
}
vec4 row(mat4 *m, unsigned int i) {
    vec4 v = {m->data[0].data[i], m->data[1].data[i], m->data[2].data[i], m->data[3].data[i]};
    return v;
}