/*
 * used for movement of player
 */

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Camera.h"
#include "Settings.h"


Camera *Camera::instance(nullptr);

//Key event processing procedure
void Camera::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  Camera *camera = Camera::getInstance();
  if (action == GLFW_PRESS) {
    switch (key) {
      case GLFW_KEY_W:
        camera->setMoveX(1);
        break;
      case GLFW_KEY_S:
        camera->setMoveX(-1);
        break;
      case GLFW_KEY_A:
        camera->setMoveY(-1);
        break;
      case GLFW_KEY_D:
        camera->setMoveY(1);
        break;
      default:
        break;
    }
  }

  if (action == GLFW_RELEASE) {
    switch (key) {
      case GLFW_KEY_W:
        camera->setMoveX(0);
        break;
      case GLFW_KEY_S:
        camera->setMoveX(0);
        break;
      case GLFW_KEY_A:
        camera->setMoveY(0);
        break;
      case GLFW_KEY_D:
        camera->setMoveY(0);
        break;
      default:
        break;
    }
  }
}

/* computes camera vectors given a time beetween frames */
void Camera::computeCamera(GLFWwindow *window, float deltaTime) {

  int windowWidth, windowHeight;
  glfwGetWindowSize(window, &windowWidth, &windowHeight);

  /* this part measures the mouse movement in both x and y axis on the screen */
  double xpos, ypos; // mouse position in x and y axis
  glfwGetCursorPos(window, &xpos, &ypos);
  glfwSetCursorPos(window, double(windowWidth / 2), double(windowHeight / 2));

  this->horizontalSightAngle += mouseSpeed * deltaTime * (windowWidth / 2 - xpos);
  this->verticalSightAngle += mouseSpeed * deltaTime * (windowHeight / 2 - ypos);

  this->direction = glm::vec3(cos(Camera::verticalSightAngle) * sin(Camera::horizontalSightAngle),
                              sin(Camera::verticalSightAngle),
                              cos(Camera::verticalSightAngle) * cos(Camera::horizontalSightAngle));

  glm::vec3 right = glm::vec3(sin(horizontalSightAngle - PI / 2.0f),
                              0,
                              cos(horizontalSightAngle - PI / 2.0f));

  this->up = glm::cross(right, direction);
  this->position += direction * deltaTime * moveSpeed * (float) moveX + right * deltaTime * moveSpeed * (float) moveY;
  this->viewMatrix = glm::lookAt(position, position + direction, up);
}

Camera *Camera::getInstance() {
  if (Camera::instance == nullptr) {
    instance = new Camera();
  }
  return instance;
}

glm::mat4 Camera::getPerspectiveMatrix() {
  return this->perspectiveMatrix;
}

glm::mat4 Camera::getVievMatrix() {
  return this->viewMatrix;
}

void Camera::setMoveX(int moveX) {
  Camera::moveX = moveX;
}
void Camera::setMoveY(int moveY) {
  Camera::moveY = moveY;
}

//default starting point is -5.0f actually
Camera::Camera() {
  this->horizontalSightAngle = 3.14f;
  this->verticalSightAngle = 0.0f;
  this->moveSpeed = 3.0f;
  this->mouseSpeed = 0.1f;
  this->position = glm::vec3(0.0f, 0.0f, 5.0f);

  this->direction = glm::vec3(cos(verticalSightAngle) * sin(horizontalSightAngle),
                              sin(verticalSightAngle),
                              cos(verticalSightAngle) * cos(horizontalSightAngle));

  glm::vec3 right = glm::vec3(sin(horizontalSightAngle - PI / 2.0f),
                              0,
                              cos(horizontalSightAngle - PI / 2.0f));

  this->up = glm::cross(right, direction);

  this->viewMatrix = glm::lookAt(position, position + direction, up);
}

//maybe constructor for any point in space would be nice?
