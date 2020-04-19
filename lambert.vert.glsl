#version 150

uniform mat4 u_ViewProj;

in vec3 vs_Pos;
in vec3 vs_Col;

out vec3 fs_Col;

void main() {
    fs_Col = vs_Col;

    gl_Position = u_ViewProj * vec4(vs_Pos, 1.0);
}