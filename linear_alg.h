#pragma once

typedef struct vec4_t {
    float data[4];
} vec4;

vec4 init_vec4(float x, float y, float z, float w);
float dot(vec4 a, vec4 b);
vec4 diff(vec4 a, vec4 b);
vec4 normalize(vec4 v);
vec4 cross(vec4 a, vec4 b);
float length(vec4 v);

typedef struct mat4_t {
    vec4 data[4];
} mat4;

mat4 identity();
mat4 translate(float x, float y, float z);
mat4 rotate(float rad, float x, float y, float z);
mat4 scale(float x, float y, float z);
mat4 viewMat(vec4 R, vec4 U, vec4 F, vec4 eye);
mat4 projMat(float fovy, float nc, float fc, float aspect);

mat4 mult_mat4(mat4 m1, mat4 m2);
vec4 mult_mat4_vec4(mat4 m, vec4 v);
vec4 col(mat4 *m, unsigned int i);
vec4 row(mat4 *m, unsigned int i);