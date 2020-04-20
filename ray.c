#include "ray.h"
#include <math.h>


Ray init_Ray(vec4 o, vec4 d) {
    Ray r;
    r.origin = o;
    r.direction = d;
    return r;
}

Ray raycast(Camera *c, float ndc_x, float ndc_y) {
    vec4 R, U, F;
    generateAxes(c, &R, &U, &F);

    float len = length(diff(c->ref, c->eye));
    float tanFOVY = tan(c->fovy / 2.f);
    vec4 V = mult_vec4_flt(U, len * tanFOVY);
    vec4 H = mult_vec4_flt(R, len * tanFOVY * c->aspect);

    vec4 P = sum(sum(c->ref, mult_vec4_flt(H, ndc_x)), mult_vec4_flt(V, ndc_y));

    Ray result = init_Ray(c->eye, normalize(diff(P, c->eye)));

    return result;
}