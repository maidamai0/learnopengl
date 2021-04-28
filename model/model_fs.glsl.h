// Don't edit this file, generated by glsl_2_header.py

#pragma once

namespace glsl {

[[maybe_unused]] constexpr auto model_fs = R"(
#version 400

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main() {
    FragColor = texture(texture_diffuse1, TexCoords);
})";
}  // namespace glsl