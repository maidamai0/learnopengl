#version 400
in vec3 aPos;
in vec3 aCol;
in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    Color = vec4(aCol, 1.0);
}