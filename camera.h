#pragma once

#include "linear_alg.h"

typedef struct camera_t {
    vec4 eye, ref;
    float fovy, near_clip, far_clip, aspect;
} Camera;

Camera init_Camera(vec4 eye, vec4 ref, float fovy, float nc, float fc, float aspect);

mat4 getViewProj(Camera *c);
void generateAxes(Camera *c, vec4 *R, vec4 *U, vec4 *F);