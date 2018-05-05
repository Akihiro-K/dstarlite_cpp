/* D * Lite (final version) - Maxim Likhachev (CMU) and Sven Koenig (USC) */
/* This script is based on http://idm-lab.org/project-a.html */

#ifndef DSTARLITE_H_
#define DSTARLITE_H_

#include "../include/include.h"
#include "../include/heap.h"
#include "../include/maze.h"

class DstarLite
{
private:
    cell goaltmpcell, oldtmpcell;
    Heap heap_;
    void SwapPermutations(int n);

public:
    int keymodifier;
    cell *lastcell;
    Maze maze_;
    int permute[DIRECTIONS];
    int* permutation[DIRECTIONS];
    int permutations;

    DstarLite(int goaly=GOALY, int goalx=GOALX, int starty=STARTY, int startx=STARTX, int height=MAZEHEIGHT, int width=MAZEWIDTH);
    void CreatePermutations();
    void Initialize();
    void InitializeCell(cell *thiscell);
    void UpdateCell(cell *thiscell);
    void UpdateKey(cell *thiscell);
    void UpdateRHS(cell *thiscell);
    int ComputeShortestPath();
    void UpdateMaze(cell *robot);
    bool IsGoal();
    bool IsObstacle();
    void UpdateLastCell();
    void ClearTrace();
    void UpdateKeymodifier();
    void SearchMaze();

};

#endif // DSTARLITE_H_
