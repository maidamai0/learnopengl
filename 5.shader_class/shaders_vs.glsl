#version 400
in vec3 vPos;
in vec3 vCol;

out vec4 Color;

void main() {
    gl_Position = vec4(vPos, 1.0);
    Color = vec4(vCol, 1.0);
}
