#pragma once

#include "linear_alg.h"
#include "camera.h"

typedef struct ray_t {
    vec4 origin;
    vec4 direction;
} Ray;

Ray init_Ray(vec4 o, vec4 d);

Ray raycast(Camera *c, float ndc_x, float ndc_y);