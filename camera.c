#include "camera.h"


Camera init_Camera(vec4 eye, vec4 ref, float fovy, float nc, float fc, float aspect) {
    Camera c;
    c.eye = eye;
    c.ref = ref;
    c.fovy = fovy;
    c.near_clip = nc;
    c.far_clip = fc;
    c.aspect = aspect;

    return c;
}

mat4 getViewProj(Camera *c) {
    vec4 F, R, U;
    generateAxes(c, &R, &U, &F);

    mat4 view = viewMat(R, U, F, c->eye);
    mat4 proj = projMat(c->fovy, c->near_clip, c->far_clip, c->aspect);
    return mult_mat4(proj, view);
}


void generateAxes(Camera *c, vec4 *R, vec4 *U, vec4 *F) {
    *F = normalize(diff(c->ref, c->eye));
    *R = normalize(cross(*F, init_vec4(0,1,0,0)));
    *U = cross(*R, *F);
}