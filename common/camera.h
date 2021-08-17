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

#include "common/log.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "magic_enum.hpp"

#include <cassert>

enum class CameraDirection { kForward, kBackward, kLeft, kRight };
enum class CameraMode { kOrthographic, kPerspective };
enum class MouseMode { Rotate, Zoom, Pan, None };

class Camera {
 public:
  explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f,
                  float pitch = 0.0f)
      : position_{position}, world_up_{up}, yaw_{yaw}, pitch_{pitch} {
    update_camera_vectors();
  }

  Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw,
         float pitch)
      : Camera(glm::vec3(pos_x, pos_y, pos_z), glm::vec3(up_x, up_y, up_z), yaw, pitch) {
  }

  [[nodiscard]] auto GetViewMatrix() const {
    return glm::lookAt(position_, position_ + front_, up_);
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
        fmt::print("warn:invalid direction: {}\n", direction);
        break;
      }
    }
  }

  auto ProcessMouseMovement(float offset_x, float offset_y, bool constrain_pitch) {
    if (mouse_mode_ == MouseMode::Rotate) {
      yaw_ += offset_x * mouse_sensitivity_;
      pitch_ += offset_y * mouse_sensitivity_;

      if (constrain_pitch) {
        if (pitch_ > 89.0f) {
          pitch_ = 89.0f;
        } else if (pitch_ < -89.0f) {
          pitch_ = -89.0f;
        }
      }
    }

    update_camera_vectors();
  }

  auto ProcessMouseScroll(float yoffset) {
    zoom_ -= yoffset;

    // if (zoom_ < 1.0f) {
    //     zoom_ = 1.0f;
    // }

    fmt::print("zoom is {}\n", zoom_);
  }

 private:
  void update_camera_vectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));

    front_ = glm::normalize(front);
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
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
  float mouse_sensitivity_{0.1f};
  float zoom_{45.0f};

  // euler angles
  float yaw_{-0.0f};
  float pitch_{0.0f};

  MouseMode mouse_mode_{MouseMode::None};
};