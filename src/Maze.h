#ifndef GRAFIKA_MAZE_H
#define GRAFIKA_MAZE_H

#include <GL/gl.h>
class Maze {
 public:
  void draw();

 private:
  static const GLfloat g_vertex_buffer_data[];
};

#endif //GRAFIKA_MAZE_H
