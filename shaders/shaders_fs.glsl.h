// Don't edit this file, generated by glsl_2_header.py

#pragma once

namespace glsl {

[[maybe_unused]] constexpr auto shaders_fs = R"(
#version 400
out vec4 frag_colour;
in vec4 Color;

void main() {
    frag_colour = Color;
}
)";
}  // namespace glsl