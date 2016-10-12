#ifndef GRAFIKA_MAZE_H
#define GRAFIKA_MAZE_H

#include <vector>
class Maze {
 public:
  static std::vector<std::vector<int>> maze;
  static std::vector<glm::mat4> calculateCeiling();
  static std::vector<glm::mat4> calculateFloor();
  static std::vector<glm::mat4> calculateWalls();
};

#endif //GRAFIKA_MAZE_H
