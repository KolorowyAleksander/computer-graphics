#ifndef GRAFIKA_MAIN_FILE_H
#define GRAFIKA_MAIN_FILE_H

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

#include <GL/glew.h>
#include "shaderprogram.h"

#endif //GRAFIKA_MAIN_FILE_H
