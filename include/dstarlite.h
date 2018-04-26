/* D* Lite (final version) - Maxim Likhachev (CMU) and Sven Koenig (USC) */

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
    void swappermutations(int n);

public:
    int keymodifier;
    cell *lastcell;
    Maze maze_;
    int permute[DIRECTIONS];
    int* permutation[DIRECTIONS];
    int permutations;
    void createpermutations();
    void initialize();
    void initializecell(cell *thiscell);
    void updatecell(cell *thiscell);
    void updatekey(cell *thiscell);
    void updaterhs(cell *thiscell);
    int computeshortestpath();
    void updatemaze(cell *robot);
    bool isgoal();
    bool isobstacle();
    void updatelastcell();
    void cleartrace();
    void updatekeymodifier();
    void searchmaze();

};

#endif // DSTARLITE_H_
