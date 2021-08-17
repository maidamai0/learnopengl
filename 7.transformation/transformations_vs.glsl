#version 400
in vec3 vPos;
in vec3 vCol;
in vec2 vTex;

uniform mat4 trans;

out vec4 Color;
out vec2 TexCoord;

void main() {
    gl_Position = trans * vec4(vPos, 1.0);
    Color = vec4(vCol, 1.0);
    TexCoord = vTex;
}
