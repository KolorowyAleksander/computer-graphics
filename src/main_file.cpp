#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <tiny_obj_loader.h>

#include <iostream>

#include "shaderprogram.h"
#include "Cube.h"
#include "main_file.h"
#include "Camera.h"

using namespace glm;

//Shader program object
ShaderProgram *shaderProgram;

//VAO and VBO handles
GLuint vao;
GLuint bufferVertices;
GLuint bufferColors;
GLuint bufferNormals;
GLuint bufferTextureCoordinates;

GLuint texture;
GLuint textureShine;

int vertexCount = Cube::vertexCount;

std::vector<std::vector<int>> hardcodedMaze = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

std::vector<glm::mat4> cubeModelVectors = calculateMaze(hardcodedMaze);
std::vector<glm::mat4> floorModelVectors = calculateFloor(hardcodedMaze);

//Error handling procedure
void error_callback(int error, const char *description) {
  std::cerr << description << std::endl;
}

//Initialization procedure
void initOpenGLProgram(GLFWwindow *window) {
  std::cout << "OpenGL vesion: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Shading Language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

  //set cursor position to the middle of the window
  int xpos, ypos;
  glfwGetWindowSize(window, &xpos, &ypos);
  glfwSetCursorPos(window, (double) xpos / 2, (double) ypos / 2);

  //disable mouse cursor
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  glfwSetKeyCallback(window, Camera::key_callback); //Register key event processing procedure

  glClearColor(0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);

  shaderProgram = new ShaderProgram("vshader.glsl", NULL, "fshader.glsl");

  texture = readTexture("brickwall.png");
  textureShine = readTexture("brickwall_contrast.png");


  bufferVertices = makeBuffer(Cube::vertices, Cube::vertexCount, sizeof(float) * 4);
  bufferNormals = makeBuffer(Cube::normals, Cube::vertexCount, sizeof(float) * 4);
  bufferTextureCoordinates = makeBuffer(Cube::texCoords, Cube::vertexCount, sizeof(float) * 2);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  assignVBOtoAttribute(shaderProgram, "vertex", bufferVertices, 4);
  assignVBOtoAttribute(shaderProgram, "normal", bufferNormals, 4);
  assignVBOtoAttribute(shaderProgram, "inTextureCoordinates", bufferTextureCoordinates, 2);

  glBindVertexArray(0);
}

void freeOpenGLProgram() {
  delete shaderProgram;

  glDeleteVertexArrays(1, &vao);

  glDeleteBuffers(1, &bufferVertices);
  glDeleteBuffers(1, &bufferColors);
  glDeleteBuffers(1, &bufferNormals);
  glDeleteBuffers(1, &bufferTextureCoordinates);

  //cleanup of singletons
  delete Camera::getInstance();
  delete Settings::getInstance();
}

void drawObject(
    GLuint vao,
    ShaderProgram *shaderProgram,
    mat4 perspectiveMatrix,
    mat4 viewMatrix,
    mat4 modelMatrix,
    int vertexCount
) {
  shaderProgram->use();

  glUniformMatrix4fv(shaderProgram->getUniformLocation("P"), 1, false, glm::value_ptr(perspectiveMatrix));
  glUniformMatrix4fv(shaderProgram->getUniformLocation("V"), 1, false, glm::value_ptr(viewMatrix));
  glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, glm::value_ptr(modelMatrix));

  glUniform1i(shaderProgram->getUniformLocation("textureMap"), 0);
  glUniform1i(shaderProgram->getUniformLocation("textureShineMap"), 0);
  glUniform1f(shaderProgram->getUniformLocation("cutoffAngle"), glm::cos(glm::radians(12.5f)));

  glUniform4f(
      shaderProgram->getUniformLocation("lightDirection"),
      Camera::getInstance()->getDirection().x,
      Camera::getInstance()->getDirection().y,
      Camera::getInstance()->getDirection().z,
      1
  );

  glUniform4f(
      shaderProgram->getUniformLocation("lightPosition"),
      Camera::getInstance()->getPosition().x,
      Camera::getInstance()->getPosition().y,
      Camera::getInstance()->getPosition().z,
      1
  );

  glBindVertexArray(vao);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textureShine);

  //here we draw
  glDrawArrays(GL_TRIANGLES, 0, vertexCount);

  glBindVertexArray(0);
}

void drawScene(GLFWwindow *window) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //get P and V matrices from camera singleton
  glm::mat4 P = Camera::getInstance()->getPerspectiveMatrix();
  glm::mat4 V = Camera::getInstance()->getVievMatrix();

  for (auto matrix : cubeModelVectors) {
    drawObject(vao, shaderProgram, P, V, matrix, vertexCount);
  }

  for (auto matrix : floorModelVectors) {
    drawObject(vao, shaderProgram, P, V, matrix, vertexCount);
  }

  glfwSwapBuffers(window);
}

int main(void) {
  GLFWwindow *window;
  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    fprintf(stderr, "Can't initialize GLFW GLFW.\n");
    exit(EXIT_FAILURE);
  }

  //Create window as well as OpenGL context.
  window = glfwCreateWindow(Settings::getInstance()->getWindowWidth(),
                            Settings::getInstance()->getWindowHeight(),
                            "maze",
                            Settings::getInstance()->getMonitorOrNull(),
                            NULL);

  if (!window) {
    std::cerr <<"Can't create window.\n";
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK) {
    std::cerr << "Can't initialize GLEW.\n";
    exit(EXIT_FAILURE);
  }

  initOpenGLProgram(window);
  glfwSetTime(0);

  //time difference for game computations
  double deltaTime;

  while (!glfwWindowShouldClose(window)) {
    deltaTime = glfwGetTime();
    Camera::getInstance()->computeCamera(window, (float) deltaTime, cubeModelVectors);
    glfwSetTime(0);
    drawScene(window);
    glfwPollEvents();
  }

  freeOpenGLProgram();

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
