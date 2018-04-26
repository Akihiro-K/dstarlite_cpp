/* D* Lite (final version) - Maxim Likhachev (CMU) and Sven Koenig (USC) */

#ifndef MAZE_H_
#define MAZE_H_

#include "include.h"
#include <vector>

struct cell;
typedef struct cell cell;

struct cell
{
    cell *move[DIRECTIONS];
    cell *succ[DIRECTIONS];
    cell *searchtree;
    cell *trace;
    short obstacle;
    int x, y;
    int dfsx, dfsy; /* needed only for generating dfs mazes */
    int g;
    int rhs;
    int key[3];
    int generated;
    int heapindex;
};

class Maze
{
private:
    void preprocessmaze();
    void postprocessmaze();

    int goaly_;
    int goalx_;
    int starty_;
    int startx_;
    int height_;
    int width_;

public:
    Maze(int goaly=GOALY, int goalx=GOALX, int starty=STARTY, int startx=STARTX, int height=MAZEHEIGHT, int width=MAZEWIDTH);
    void newrandommaze();
    void newdfsmaze(int wallstoremove);
    void printactualmaze();
    void printknownmaze();

    std::vector<std::vector<cell>> graph_;
    cell *mazestart_; // Note: goal cell of the robot
    cell *mazegoal_; // Note: start cell of the robot
    int mazeiteration_;
};

#endif // MAZE_H_
