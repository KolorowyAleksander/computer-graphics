/*
 * used for movement of player
 */

#ifndef GRAFIKA_CAMERA_H
#define GRAFIKA_CAMERA_H


#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/detail/type_mat.hpp>
#include "Settings.h"

class Camera { //camera singleton
 private:
  static Camera *instance;
  Camera();

  bool lightSwitch;

  glm::vec3 direction;
  glm::vec3 up;
  glm::vec3 position;

  float moveSpeed, mouseSpeed;
  int moveX, moveY; // moving in both planes
  float horizontalSightAngle, verticalSightAngle;

  // const float fieldOfVision = 45.0f; field of vision is constant, so i might as well
  glm::mat4 perspectiveMatrix;
  glm::mat4 viewMatrix;

 public:
  static Camera *getInstance();
  static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
  void computeCamera(GLFWwindow *window, float deltaTime, std::vector<glm::mat4> vector);

  glm::mat4 getPerspectiveMatrix();
  glm::mat4 getVievMatrix();
  glm::vec3 getPosition();
  glm::vec3 getDirection();
  int getSwitch();

  void setMoveX(int moveX);
  void setMoveY(int moveY);
  bool checkCollision(glm::vec4 position, std::vector<glm::mat4> vector);
};


#endif //GRAFIKA_CAMERA_H
