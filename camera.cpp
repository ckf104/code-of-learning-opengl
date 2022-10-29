#include "camera.hpp"

#include <iostream>

const float SPEED = 10.0f;
const float SENSITIVITY = 0.4f;

Camera::Camera(float sw, float sh, float near /*=0.1f*/, float far /*=100.0f*/,
               glm::vec3 position /*=glm::vec3(0.0f, 0.0f, 0.0f)*/,
               glm::vec3 up /*=glm::vec3(0.0f, 1.0f, 0.0f)*/,
               float move_speed /*=0.0f*/, float mouse_speed /*=0.0f*/)
    : View(1.0f), Projection(1.0f) {
  MouseSensitivity = mouse_speed == 0.0f ? SENSITIVITY : mouse_speed;
  MovementSpeed = move_speed == 0.0f ? SPEED : move_speed;
  Position = position;
  Up = glm::normalize(up);
  ZNear = near;
  ZFar = far;
  SetScreen(sw, sh);

  updateCameraVectors();
  updateView();
  updateProjection();
}

void Camera::updateCameraVectors() {
  Front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front.y = sin(glm::radians(Pitch));
  Front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Right = glm::normalize(glm::cross(Front, Up));
}

void Camera::updateView() {
  View = glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetView() { return View; }

void Camera::updateProjection() {
  Projection =
      glm::perspective(glm::radians(Zoom), ScreenW / ScreenH, ZNear, ZFar);
}

void Camera::SetScreen(float sw, float sh) {
  ScreenW = sw, ScreenH = sh;
  updateProjection();
}

glm::mat4 Camera::GetProjection() { return Projection; }
glm::vec3 Camera::GetPosition() { return Position; }

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
  float velocity = MovementSpeed * deltaTime;
  if (direction == Camera_Movement::FORWARD)
    Position += Front * velocity;
  else if (direction == Camera_Movement::BACKWARD)
    Position -= Front * velocity;
  else if (direction == Camera_Movement::LEFT)
    Position -= Right * velocity;
  else if (direction == Camera_Movement::RIGHT)
    Position += Right * velocity;
  updateView();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset,
                                  GLboolean constrainPitch /*=true*/) {
  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;
  std::cout << "[info] mouse move, xoff: " << xoffset
            << ", yoffset: " << yoffset << std::endl;

  Yaw += xoffset;
  Pitch += yoffset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch) {
    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;
  }

  std::cout << "[info] now Yaw: " << Yaw << ", Pitch: " << Pitch << std::endl;

  // update Front, Right and Up Vectors using the updated Euler angles
  updateCameraVectors();
  updateView();
}

void Camera::ProcessMouseScroll(float yoffset) {
  std::cout << "[info] mouse scroll, yoffset: " << yoffset << std::endl;
  Zoom -= (float)yoffset;
  if (Zoom < 1.0f) Zoom = 1.0f;
  if (Zoom > 189.0f) Zoom = 189.0f;
  std::cout << "[info] now Zoom: " << Zoom << std::endl;
  updateProjection();
}