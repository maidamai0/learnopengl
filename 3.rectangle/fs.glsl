#version 330 core
uniform float color_scalar;
in vec3 fs_color;
out vec4 FragColor;

void main() {
    FragColor = vec4(fs_color * color_scalar, 1.0);
}
