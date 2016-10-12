#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "ShaderProgram.h"
#include "Cube.h"
#include "Camera.h"
#include "Maze.h"
#include "Main.h"
#include "../lib/lodepng.h"

Main::Main() {
  glfwSetErrorCallback(errorCallback);

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
    std::cerr << "Can't create window.\n";
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    std::cerr << "Can't initialize GLEW.\n";
    exit(EXIT_FAILURE);
  }

  glfwSwapInterval(1);
  init(window);

  this->wallsModelsVector = Maze::calculateWalls();
  this->floorModelsVector = Maze::calculateFloor();
  this->ceilingModelsVector = Maze::calculateCeiling();
}

void Main::run() {
  glfwSetTime(0);

  while (!glfwWindowShouldClose(window)) {
    Camera::getInstance()->computeCamera(window, (float) glfwGetTime(), wallsModelsVector);
    glfwSetTime(0);
    drawScene();
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

Main::~Main() {
  destroy();
}
void Main::init(GLFWwindow *window) {
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Shading Language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

  //mouse and keyboard
  glfwSetKeyCallback(window, Camera::key_callback);

  int xpos, ypos;
  glfwGetWindowSize(window, &xpos, &ypos);
  glfwSetCursorPos(window, (double) xpos / 2, (double) ypos / 2);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  glClearColor(0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);

  shaderProgram = new ShaderProgram("wallVertexShader.glsl", NULL, "wallFragmentShader.glsl");

  wallTexture = readTexture("brickwall.png");
  floorTexture = readTexture("brickwall_contrast.png");

  bufferVertices = makeBuffer(Cube::vertices, Cube::vertexCount, sizeof(float) * 4);
  bufferNormals = makeBuffer(Cube::normals, Cube::vertexCount, sizeof(float) * 4);
  bufferTextureCoordinates = makeBuffer(Cube::texCoords, Cube::vertexCount, sizeof(float) * 2);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  assignVBOtoAttribute("vertex", bufferVertices, 4);
  assignVBOtoAttribute("normal", bufferNormals, 4);
  assignVBOtoAttribute("inTextureCoordinates", bufferTextureCoordinates, 2);

  glBindVertexArray(0);
}

//from slides
GLuint Main::readTexture(std::string filename) {
  GLuint tex;
  glActiveTexture(GL_TEXTURE0);
  std::vector<unsigned char> image;
  unsigned width, height;

  unsigned error = lodepng::decode(image, width, height, filename);
  std::cerr << error << std::endl;

  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  return tex;
}

void Main::destroy() {
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

void Main::drawObject(
    GLuint vao,
    glm::mat4 perspectiveMatrix,
    glm::mat4 viewMatrix,
    glm::mat4 modelMatrix
) {
  shaderProgram->use();

  glUniformMatrix4fv(shaderProgram->getUniformLocation("P"), 1, false, glm::value_ptr(perspectiveMatrix));
  glUniformMatrix4fv(shaderProgram->getUniformLocation("V"), 1, false, glm::value_ptr(viewMatrix));
  glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, glm::value_ptr(modelMatrix));

  glUniform1i(shaderProgram->getUniformLocation(TEXTURE_MAP), 0);
  glUniform1i(shaderProgram->getUniformLocation("textureShineMap"), 0);
  glUniform1f(shaderProgram->getUniformLocation("umbra"), glm::cos(glm::radians(12.5f)));
  glUniform1f(shaderProgram->getUniformLocation("penumbra"), glm::cos(glm::radians(20.0f)));

  glUniform1i(shaderProgram->getUniformLocation("lightSwitch"), Camera::getInstance()->getSwitch());

  glUniform4f(
      shaderProgram->getUniformLocation(LIGHT_DIRECTION),
      Camera::getInstance()->getDirection().x,
      Camera::getInstance()->getDirection().y,
      Camera::getInstance()->getDirection().z,
      0
  );

  glUniform4f(
      shaderProgram->getUniformLocation(LIGHT_POSITION),
      Camera::getInstance()->getPosition().x,
      Camera::getInstance()->getPosition().y,
      Camera::getInstance()->getPosition().z,
      1
  );

  glBindVertexArray(vao);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, wallTexture);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, floorTexture);

  //here we draw
  glDrawArrays(GL_TRIANGLES, 0, Cube::vertexCount);

  glBindVertexArray(0);
}

void Main::drawScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //get P and V matrices from camera singleton
  glm::mat4 P = Camera::getInstance()->getPerspectiveMatrix();
  glm::mat4 V = Camera::getInstance()->getVievMatrix();

  for (auto matrix : wallsModelsVector) {
    drawObject(vao, P, V, matrix);
  }

  for (auto matrix : floorModelsVector) {
    drawObject(vao, P, V, matrix);
  }

  for (auto matrix : ceilingModelsVector) {
    drawObject(vao, P, V, matrix);
  }

  glfwSwapBuffers(window);
}
void Main::errorCallback(int error, const char *description) {
  std::cerr << description << std::endl;
}
GLuint Main::makeBuffer(void *data, int vertexCount, int vertexSize) {
  GLuint handle;

  glGenBuffers(1, &handle);
  glBindBuffer(GL_ARRAY_BUFFER, handle);
  glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, data, GL_STATIC_DRAW);

  return handle;
}

void Main::assignVBOtoAttribute(char *attributeName, GLuint bufVBO, int vertexSize) {
  GLuint location = shaderProgram->getAttribLocation(attributeName);
  glBindBuffer(GL_ARRAY_BUFFER, bufVBO);
  glEnableVertexAttribArray(location);
  glVertexAttribPointer(
      location,
      vertexSize,
      GL_FLOAT,
      GL_FALSE,
      0,
      NULL
  );
}
