#ifndef GRAFIKA_MAIN_FILE_H
#define GRAFIKA_MAIN_FILE_H

#include "../lib/lodepng.h"

//from the slides
GLuint readTexture(std::string filename) {
  GLuint tex;
  glActiveTexture(GL_TEXTURE0);
  std::vector<unsigned char> image;
  unsigned width, height;

  unsigned error = lodepng::decode(image, width, height, filename);
  std::cerr << error << std::endl;

  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, image.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  return tex;
}

GLuint makeBuffer(void *data, int vertexCount, int vertexSize) {
  GLuint handle;

  glGenBuffers(1, &handle);
  glBindBuffer(GL_ARRAY_BUFFER, handle);
  glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, data, GL_STATIC_DRAW);

  return handle;
}

void assignVBOtoAttribute(ShaderProgram *shaderProgram, char *attributeName, GLuint bufVBO, int vertexSize) {
  GLuint location = shaderProgram->getAttribLocation(attributeName);
  glBindBuffer(GL_ARRAY_BUFFER, bufVBO);
  glEnableVertexAttribArray(location);
  glVertexAttribPointer(location,
                        vertexSize,
                        GL_FLOAT,
                        GL_FALSE,
                        0,
                        NULL);
}

std::vector<glm::mat4> calculateMaze(std::vector<std::vector<int>> hardcodedMaze) {
  std::vector<glm::mat4> vector;
  glm::mat4 M = glm::mat4(1.0f);
  for (int i = 0; i < hardcodedMaze.size(); i++) {
    for (int j = 0; j < hardcodedMaze[i].size(); j++) {
      if (hardcodedMaze[i][j] == 1) {
        vector.push_back(glm::translate(M, glm::vec3(i * 2.0001f, 1.0f, 2.0001f * j)));
      }
    }
  }
  return vector;
}

std::vector<glm::mat4> calculateFloor(std::vector<std::vector<int>> hardcodedMaze) {
  std::vector<glm::mat4> vector;
  glm::mat4 M = glm::mat4(1.0f);
  for (int i = 0; i < hardcodedMaze.size(); i++) {
    for (int j = 0; j < hardcodedMaze[i].size(); j++) {
      if (hardcodedMaze[i][j] == 0) {
        vector.push_back(glm::translate(M, glm::vec3(i * 2.0001f, -1.0001f, 2.0001f * j)));
        vector.push_back(glm::translate(M, glm::vec3(i * 2.0001f, 3.0001f, 2.0001f * j)));
      }
    }
  }

  return vector;
}
#endif //GRAFIKA_MAIN_FILE_H
