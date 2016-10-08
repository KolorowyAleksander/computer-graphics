
#ifndef GRAFIKA_DRAWABLE_H
#define GRAFIKA_DRAWABLE_H

#include "shaderprogram.h"
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>

/**
 * general abstraction for objects that can be drawn
 **/
class Drawable {
 public:
  Drawable(){};
  virtual void move(float x, float y, float z) = 0;
  static const float *getVertices();
  static const float *getVertexNormals();
  static const float *getColors();
  static const float *getNormals();
  static const int getVertexCount();

  static void setShaderProgram(ShaderProgram *shaderProgram);
  static const int getOne() {
    return 1;
  }
 protected:
  Drawable(glm::mat4 modelMatrix);
  Drawable(float x, float y, float z);
  ~Drawable();
  virtual void draw(GLuint vao);
  glm::mat4 modelMatrix;

  static ShaderProgram *shaderProgram;
  static const float *vertices;
  static float *vertexNormals;
  static const float *colors;
  static const float *normals;
  static const int vertexCount;
};

#endif //GRAFIKA_DRAWABLE_H
