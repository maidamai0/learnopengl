// Don't edit this file, generated by glsl_2_header.py

#pragma once

namespace glsl {

[[maybe_unused]] constexpr auto vs = R"(
#version 410
in vec3 vp;
void main() {
    gl_Position = vec4(vp.x, vp.y, vp.z, 1.0);
}
)";
}  // namespace glsl