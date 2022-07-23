/**
 * @file camera.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-12-07
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "common/guard.h"
#include "common/log.h"
#include "common/trackball.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "magic_enum.hpp"

#include <algorithm>
#include <cassert>

enum class CameraDirection { kForward, kBackward, kLeft, kRight };
enum class CameraMode { kOrthographic, kPerspective };
enum class MouseMode { Rotate, Pan, None };

class Camera {
 public:
  explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
      : position_{position}, world_up_{up} {
    update_camera_vectors();
  }

  Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z)
      : Camera(glm::vec3(pos_x, pos_y, pos_z), glm::vec3(up_x, up_y, up_z)) {
  }

  [[nodiscard]] auto GetViewMatrix() const {
    auto view = glm::lookAt(position_, position_ + front_, up_);
    view = view * trackball_.RotationMatrix();
    // view = glm::rotate(view, yaw_, glm::vec3(0.0F, 1.0F, 0.0F));
    // view = glm::rotate(view, -pitch_, glm::vec3(1.0F, 0.0F, 0.0F));
    return view;
  }

  [[nodiscard]] auto GetProjectionMatrix(const float aspect) const {
    return glm::perspective(glm::radians(zoom_), aspect, near_, far_);
  }

  [[nodiscard]] auto GetZoom() const {
    return zoom_;
  }

  [[nodiscard]] auto GetPosition() const {
    return position_;
  }

  auto SetMouseMode(MouseMode mode) {
    mouse_mode_ = mode;
    LOGI("mouse mode changed to {}", magic_enum::enum_name(mouse_mode_));
  }

  auto ProcessKeyBoardEvent(CameraDirection direction, float delta_time) {
    auto velocity = move_speed_ * delta_time;
    switch (direction) {
      case CameraDirection::kForward: {
        position_ -= front_ * velocity;
        break;
      }
      case CameraDirection::kBackward: {
        position_ += front_ * velocity;
        break;
      }
      case CameraDirection::kLeft: {
        position_ += right_ * velocity;
        break;
      }
      case CameraDirection::kRight: {
        position_ -= right_ * velocity;
        break;
      }
      default: {
        assert(false && "invalid direction");
        fmt::print("warn:invalid direction: {}\n", magic_enum::enum_name(direction));
        break;
      }
    }
  }

  auto ProcessMouseMovement(float xpos, float ypos, int window_w, int window_h) {
    const auto update_mouse = make_guard([this, xpos, ypos]() {
      mouse_x_ = xpos;
      mouse_y_ = ypos;
    });

    static bool first_time = true;
    if (first_time) {
      first_time = false;
      return;
    }

    switch (mouse_mode_) {
      case MouseMode::Rotate:
        return trackball_.OnMouseMove(mouse_x_ / window_h - 0.5F, -(mouse_y_ / window_h - 0.5F),
                                      xpos / window_w - 0.5F, -(ypos / window_h - 0.5F));
      case MouseMode::Pan:
        return on_pan(xpos - mouse_x_, ypos - mouse_y_);
      case MouseMode::None:
        return;
    }
  }

  auto ProcessMouseScroll(float yoffset) {
    zoom_ -= yoffset;
    zoom_ = std::clamp(zoom_, near_, far_);
  }

 private:
  void update_camera_vectors() {
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
  }

  void on_pan(float offset_x, float offset_y) {
    position_ -= right_ * offset_x * mouse_sensitivity_;
    position_ -= up_ * offset_y * mouse_sensitivity_;
  }

  static float normalize_mouse_position(float pos, int dim) {
    return pos / dim - 0.5F;
  }

 private:
  // camera attributes
  glm::vec3 position_;
  glm::vec3 front_{0.0f, 0.0f, -1.0f};
  glm::vec3 up_{};
  glm::vec3 right_{};
  glm::vec3 world_up_;

  // camera options
  float move_speed_{250.0f};
  float mouse_sensitivity_{0.01f};
  float zoom_{45.0f};
  float mouse_x_ = 0;
  float mouse_y_ = 0;

  // trackball for ratation
  Trackball trackball_;

  float near_ = 0.1F;
  float far_ = 100.0F;

  MouseMode mouse_mode_{MouseMode::None};
};