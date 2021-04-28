// Don't edit this file, generated by glsl_2_header.py

#pragma once

namespace glsl {

[[maybe_unused]] constexpr auto color_fs = R"(
#version 400

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
    FragColor = vec4(objectColor * lightColor, 1.0);
})";
}  // namespace glsl