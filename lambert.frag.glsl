#version 150

in vec3 fs_Col;
in vec3 fs_Nor;
out vec4 out_Col;

void main() {
    out_Col = vec4(0.5 * (fs_Nor + vec3(1.0, 1.0, 1.0)), 1.0);//vec4(fs_Col, 1.0);
}