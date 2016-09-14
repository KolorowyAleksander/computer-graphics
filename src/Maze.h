#ifndef GRAFIKA_MAZE_H
#define GRAFIKA_MAZE_H


class Maze {
public:
    void draw();

private:
    float *array;
    void generate();
};


#endif //GRAFIKA_MAZE_H
