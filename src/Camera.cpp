/*
 * used for movement of player
 */

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Camera.h"

const float PI = 3.141592653589793f;

Camera *Camera::instance(nullptr);

//Key event processing procedure
void Camera::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  Camera *camera = Camera::getInstance();
  if (action == GLFW_PRESS) {
    switch (key) {
      case GLFW_KEY_W:camera->setMoveX(1);
        break;
      case GLFW_KEY_S:camera->setMoveX(-1);
        break;
      case GLFW_KEY_A:camera->setMoveY(-1);
        break;
      case GLFW_KEY_D:camera->setMoveY(1);
        break;
      case GLFW_KEY_L:camera->lightSwitch = !camera->lightSwitch;
        break;
      case GLFW_KEY_ESCAPE:glfwSetWindowShouldClose(window, 1);
      default:break;
    }
  }

  if (action == GLFW_RELEASE) {
    switch (key) {
      case GLFW_KEY_W:camera->setMoveX(0);
        break;
      case GLFW_KEY_S:camera->setMoveX(0);
        break;
      case GLFW_KEY_A:camera->setMoveY(0);
        break;
      case GLFW_KEY_D:camera->setMoveY(0);
        break;
      default:break;
    }
  }
}

//checks if player is trying to step into something
bool Camera::checkCollision(glm::vec4 position, std::vector<glm::mat4> vector) {

  float boxLength = 0.35f;

  glm::vec4 a = position - glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) * boxLength;
  glm::vec4 A = position + glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) * boxLength;

  for (glm::mat4 m : vector) {
    glm::vec4 b = m * glm::vec4(-1.0f, 0.0f, -1.0f, 1.0f);
    glm::vec4 B = m * glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

    //very simple AABB collision detection in 2d
    if (!(A.x < b.x || B.x < a.x || A.z < b.z || B.z < a.z)) {
      return true;
    }
  }
  return false;
}

/* computes camera vectors given a time beetween frames */
void Camera::computeCamera(GLFWwindow *window, float deltaTime, std::vector<glm::mat4> vector) {

  int windowWidth, windowHeight;
  glfwGetWindowSize(window, &windowWidth, &windowHeight);

  /* this part measures the mouse movement in both x and y axis on the screen */
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  glfwSetCursorPos(window, double(windowWidth / 2), double(windowHeight / 2));

  this->horizontalSightAngle += mouseSpeed * deltaTime * (windowWidth / 2 - xpos);
  if (this->verticalSightAngle + mouseSpeed * deltaTime * (windowHeight / 2 - ypos) < 1.0f
      && this->verticalSightAngle + mouseSpeed * deltaTime * (windowHeight / 2 - ypos) > -1.0f) {
    this->verticalSightAngle += mouseSpeed * deltaTime * (windowHeight / 2 - ypos);
  }

  this->direction =
      glm::vec3(
          cos(Camera::verticalSightAngle) * sin(Camera::horizontalSightAngle),
          sin(Camera::verticalSightAngle),
          cos(Camera::verticalSightAngle) * cos(Camera::horizontalSightAngle)
      );


  //"right" vector to easily computate up vector
  glm::vec3 right =
      glm::vec3(
          sin(horizontalSightAngle - PI / 2.0f),
          0,
          cos(horizontalSightAngle - PI / 2.0f)
      );

  //up is cross product of direction and right vector
  this->up = glm::cross(right, direction);

  glm::vec3 nextPosition = this->position;

  //see next frame position
  nextPosition.x += direction.x * deltaTime * moveSpeed * (float) this->moveX;
  nextPosition.z += direction.z * deltaTime * moveSpeed * (float) this->moveX;
  nextPosition += right * deltaTime * moveSpeed * (float) this->moveY;

  //check for collision - move is 'cancelled' if something's in the way
  if (!checkCollision(glm::vec4(nextPosition, 1), vector)) {
    this->position = nextPosition;
  }

  //calculate viewmatrix
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

Camera::Camera() {
  this->lightSwitch = false;
  this->moveX = 0;
  this->moveY = 0;
  this->horizontalSightAngle = 3.14f;
  this->verticalSightAngle = 0.0f;
  this->moveSpeed = 3.0f;
  this->mouseSpeed = 0.1f;
  this->position = Settings::getInstance()->getStartingPoint();

  this->direction =
      glm::vec3(
          cos(verticalSightAngle) * sin(horizontalSightAngle),
          sin(verticalSightAngle),
          cos(verticalSightAngle) * cos(horizontalSightAngle)
      );

  glm::vec3 right =
      glm::vec3(
          sin(horizontalSightAngle - PI / 2.0f),
          0,
          cos(horizontalSightAngle - PI / 2.0f)
      );

  this->up = glm::cross(right, direction);

  this->viewMatrix = glm::lookAt(position, position + direction, up);
  this->perspectiveMatrix =
      glm::perspective(
          45.0f * PI / 180,
          (float) Settings::getInstance()->getWindowWidth()
              / Settings::getInstance()->getWindowHeight(),
          0.25f, //really close clipping so we don't see inside walls
          150.0f
      );
}
glm::vec3 Camera::getPosition() {
  return this->position;
}
glm::vec3 Camera::getDirection() {
  return this->direction;
}
int Camera::getSwitch() {
  if (lightSwitch) {
    return 1;
  } else {
    return 0;
  }
}
