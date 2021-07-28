#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 color;

out vec3 o_color;
void main() {
    gl_Position = vec4(aPos, 1.0f);
    o_color = color;
    // o_color = vec3(aPos.x + 0.5, aPos.y + 0.5, aPos.z + 0.5);
}
