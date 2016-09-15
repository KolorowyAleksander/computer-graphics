/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TEAPOT_H
#define TEAPOT_H

//Utah teapot model made out of triangles
//Contains arrays
//vertices - vertex positions in homogenous coordinates
//normals -vertex normals in homogenous coordinates (flat shading)
//vertexNormals - vertex normals in homogenous coordinates (smooth shading)
//texCoords -  texturing coordinates
//colors - vertex colors (rgba)
//TBN friendly
//Culling GL_CW

#include "model.h"
#include "Drawable.h"

class Teapot : public Drawable {
 public:
  Teapot();
  Teapot(glm::mat4 modelMatrix);
  Teapot(float x, float y, float z);
  void move(float x, float y, float z);
 private:
  static ShaderProgram *shaderProgram;
  static float *vertices;
  static float *vertexNormals;
  static const float *colors;
  static const float *normals;
  static const int vertexCount;
 public:
  static float *getVertices();
  static float *getVertexNormals();
  static const float *getColors();
  static const float *getNormals();
  static const int getVertexCount();
  static void setShaderProgram(ShaderProgram *shaderProgram);
};
/*
namespace Models {

namespace TeapotInternal {
extern float vertices[];
extern float normals[];
extern float vertexNormals[];
extern float texCoords[];
extern float colors[];
extern unsigned int vertexCount;
}

class Teapot : public Model {
 public:
  Teapot();
  virtual ~Teapot();
  virtual void drawSolid();
};

extern Teapot teapot;

}*/
#endif