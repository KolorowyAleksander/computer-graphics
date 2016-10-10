#ifndef GRAFIKA_MAIN_FILE_H
#define GRAFIKA_MAIN_FILE_H

#include <lodepng.h>

//reads texture into a texture handle
GLuint readTexture(std::string filename) {
  GLuint tex;
  glActiveTexture(GL_TEXTURE0);
  //Load into computer's memory
  std::vector<unsigned char> image; //Allocate a vector for image storage
  unsigned width, height; //Variables for image size
  //Read image
  unsigned error = lodepng::decode(image, width, height, filename);
  //Import into graphics card's memory
  glGenTextures(1, &tex); //Initialize one handle
  glBindTexture(GL_TEXTURE_2D, tex); //Activate the handle
  //Import image into graphics card's memory associated with the handle
  glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char *) image.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  return tex;
}

//Creates a VBO buffer from an array
GLuint makeBuffer(void *data, int vertexCount, int vertexSize) {
  GLuint handle;

  glGenBuffers(1, &handle);//Generate handle for VBO buffer
  glBindBuffer(GL_ARRAY_BUFFER, handle);  //Active the handle
  glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, data, GL_STATIC_DRAW);//Copy the array to VBO

  return handle;
}

//Assigns VBO buffer handle to an attribute of a given name
void assignVBOtoAttribute(ShaderProgram *shaderProgram, char *attributeName, GLuint bufVBO, int vertexSize) {
  GLuint location = shaderProgram->getAttribLocation(attributeName); //Get slot number for the attribute
  glBindBuffer(GL_ARRAY_BUFFER, bufVBO);  //Activate VBO handle
  glEnableVertexAttribArray(location); //Turn on using of an attribute of a number passed as an argument
  glVertexAttribPointer(location, vertexSize, GL_FLOAT, GL_FALSE, 0,
                        NULL); //Data for the slot should be taken from the current VBO buffer
}

#endif //GRAFIKA_MAIN_FILE_H
