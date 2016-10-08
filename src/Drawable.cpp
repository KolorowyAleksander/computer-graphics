//
// Created by a on 15.09.16.
//

#include <glm/gtc/type_ptr.hpp>
#include "Drawable.h"
#include "Camera.h"
void Drawable::draw(GLuint vao) {
  shaderProgram->use();

  glUniformMatrix4fv(shaderProgram->getUniformLocation("P"), 1, false, glm::value_ptr(this->modelMatrix));
  glUniformMatrix4fv(shaderProgram->getUniformLocation("V"), 1, false, glm::value_ptr(Camera::getPerspecitveMatrix()));
  glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, glm::value_ptr(Camera::getViewMatrix()));
  glUniform4f(shaderProgram->getUniformLocation("lightPos0"), 0, 0, 5, 1);

  glBindVertexArray(vao);

  //Drawing of an object
  glDrawArrays(GL_TRIANGLES, 0, vertexCount);

  //Tidying up after ourselves (not needed if we use VAO for every object)
  glBindVertexArray(0);
}
Drawable::Drawable(glm::mat4 modelMatrix) {
  this->modelMatrix = modelMatrix;
}
Drawable::Drawable(float x, float y, float z) {
  this->modelMatrix = glm::mat4(
      glm::vec4(1, 0, 0, x),
      glm::vec4(0, 1, 0, y),
      glm::vec4(0, 0, 1, z),
      glm::vec4(0, 0, 0, 1)
  );
}
Drawable::~Drawable() {
  if(shaderProgram != nullptr)
    delete shaderProgram;
}

const float *Drawable::getVertices() {
  return vertices;
}

const float *Drawable::getColors() {
  return colors;
}

const float *Drawable::getNormals() {
  return normals;
}

const int Drawable::getVertexCount() {
  return vertexCount;
}

const float *Drawable::getVertexNormals() {
  return vertexNormals;
}
void Drawable::setShaderProgram(ShaderProgram *shaderProgram) {
  Drawable::shaderProgram = shaderProgram;
}
