#pragma once

#include <cmath>
#include "common/log.h"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/vector_angle.hpp"

// origin is in the middle of the window
class Trackball {
 public:
  void OnMouseMove(float old_xpos, float old_ypos, float xpos, float ypos) {
    const auto old_pos = glm::vec3(old_xpos, old_ypos, get_z(old_xpos, old_ypos));
    const auto new_pos = glm::vec3(xpos, ypos, get_z(xpos, ypos));

    const auto axis = glm::normalize(glm::cross(old_pos, new_pos));
    const auto angle = glm::angle(old_pos, new_pos) * 10;

    quaternion_ = glm::angleAxis(angle, axis) * quaternion_;
  }

  [[nodiscard("rotation matrix should not be discarded")]] auto RotationMatrix() const {
    return glm::mat4_cast(quaternion_);
  }

 private:
  static float get_z(float mouse_x, float mouse_y) {
    const float temp = std::powf(mouse_x, 2) + std::powf(mouse_y, 2);
    constexpr auto radius = 1.0F;

    // use sphere
    if (temp < radius / 2) {
      return std::sqrtf(radius - temp);
    }

    // use hyperbolic sheet
    return radius / 2 / std::sqrtf(temp);
  }

  glm::quat quaternion_{1, 0, 0, 0};
};