
#ifndef GRAFIKA_CAMERA_H
#define GRAFIKA_CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/detail/type_mat.hpp>
#include "constants.h"

/**
 * used for movement of player
 **/
class Camera { //camera singleton
 private:
  static Camera *instance;
  Camera();

  glm::vec3 direction;
  glm::vec3 up;
  glm::vec3 position;

  float moveSpeed, mouseSpeed;
  int moveX, moveY; // moving in both planes
  float horizontalSightAngle, verticalSightAngle;

  // const float fieldOfVision = 45.0f; field of vision is constant, so i might as well
  glm::mat4 perspectiveMatrix;
  glm::mat4 viewMatrix;

  glm::mat4 _getPerspectiveMatrix();
  glm::mat4 _getVievMatrix();
 public:
  static Camera *getInstance();
  static glm::mat4 getPerspecitveMatrix();
  static glm::mat4 getViewMatrix();

  static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
  void computeCamera(GLFWwindow *window, float deltaTime);
  void setMoveX(int moveX);
  void setMoveY(int moveY);
};


#endif //GRAFIKA_CAMERA_H
