#ifndef GRAFIKA_MAIN_H
#define GRAFIKA_MAIN_H

static char *TEXTURE_MAP = "textureMap";
static char *LIGHT_POSITION = "lightPosition";
static char *LIGHT_DIRECTION = "lightDirection";

#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include <GLFW/glfw3.h>

class Main {
 public:
  Main();
  ~Main();
  void run();

 private:
  GLFWwindow *window;
  ShaderProgram *shaderProgram;
  GLuint vao;
  GLuint bufferVertices;
  GLuint bufferColors;
  GLuint bufferNormals;
  GLuint bufferTextureCoordinates;
  GLuint wallTexture;
  GLuint floorTexture;

  std::vector<glm::mat4> wallsModelsVector;
  std::vector<glm::mat4> floorModelsVector;
  std::vector<glm::mat4> ceilingModelsVector;

  GLuint readTexture(std::string filename);

  void init(GLFWwindow *window);
  void destroy();
  GLuint makeBuffer(void *data, int vertexCount, int vertexSize);
  void assignVBOtoAttribute(char *attributeName, GLuint bufVBO, int vertexSize);
  void drawScene();
  void drawObject(
      GLuint vao,
      glm::mat4 perspectiveMatrix,
      glm::mat4 viewMatrix,
      glm::mat4 modelMatrix
  );

  static void errorCallback(int error, const char *description);
};

#endif //GRAFIKA_MAIN_H
