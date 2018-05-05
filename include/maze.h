/* D * Lite (final version) - Maxim Likhachev (CMU) and Sven Koenig (USC) */
/* This script is based on http://idm-lab.org/project-a.html */

#ifndef MAZE_H_
#define MAZE_H_

#include "include.h"
#include <vector>
#include <string>

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
    float g;
    float rhs;
    float key[3];
    int generated;
    int heapindex;
};

class Maze
{
private:
    void PreprocessMaze();
    void PostprocessMaze();

    int goaly_;
    int goalx_;
    int starty_;
    int startx_;
    int height_;
    int width_;

public:
    Maze(int goaly=GOALY, int goalx=GOALX, int starty=STARTY, int startx=STARTX, int height=MAZEHEIGHT, int width=MAZEWIDTH);
    void SetMazeParameters(int goaly, int goalx, int starty, int startx, int height, int width);
    void NewRandomMaze();
    void NewDfsMaze(int wallstoremove);
    void PrintActualMaze();
    void PrintKnownMaze();
    void ReadFromFile(std::string filepath);

    std::vector<std::vector<cell>> graph_;
    cell *mazestart_; // Note: goal cell of the robot
    cell *mazegoal_; // Note: start cell of the robot
    int mazeiteration_;
};

#endif // MAZE_H_
