#ifndef __camera_HPP
#define __camera_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
 public:
  // camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;  // direction vector
  glm::vec3 Up;
  glm::vec3 Right;  // Front and Right will be deduced from Yaw and Pitch

  // euler Angles
  float Yaw = -90.0f;
  float Pitch = 0.0f;
  // camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom = 45.0f;  // relation between fov and sense of distance of object

  float ScreenW;
  float ScreenH;
  float ZNear;
  float ZFar;

  glm::mat4 View;
  glm::mat4 Projection;

  Camera(float sw, float sh, float near = 0.1f, float far = 100.0f,
         glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float move_speed = 0.0f,
         float mouse_speed = 0.0f);
  void ProcessKeyboard(Camera_Movement direction, float deltaTime);
  void ProcessMouseMovement(float xoffset, float yoffset,
                            GLboolean constrainPitch = true);
  void SetScreen(float sw, float sh);
  void ProcessMouseScroll(float yoffset);
  glm::mat4 GetProjection();
  glm::mat4 GetView();

 private:
  void updateProjection();
  void updateCameraVectors();
  void updateView();
};

#endif