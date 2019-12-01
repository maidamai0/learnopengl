#version 400
in vec3 vPos;
in vec3 vCol;
in vec2 vTex;

out vec4 Color;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 trans;

void main() {
    gl_Position = trans * vec4(vPos, 1.0);
    // gl_Position = model * view * projection * vec4(vPos, 1.0);
    Color = vec4(vCol, 1.0);
    TexCoord = vTex;
}
