// Don't edit this file, generated by glsl_2_header.py

#pragma once

namespace glsl {

[[maybe_unused]] constexpr auto fs = R"(
#version 330 core
in vec3 o_color;
out vec4 FragColor;

void main() {
    FragColor = vec4(o_color, 1.0f);
}
)";
}  // namespace glsl