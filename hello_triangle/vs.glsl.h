// Don't edit this file, generated by glsl_2_header.py

#pragma once

namespace glsl {

[[maybe_unused]] constexpr auto vs = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
void main() {
  gl_Position = vec4(aPos, 1.0f);
}
)";
}  // namespace glsl