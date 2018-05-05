/* D * Lite (final version) - Maxim Likhachev (CMU) and Sven Koenig (USC) */
/* This script is based on http://idm-lab.org/project-a.html */

#ifndef HEAP_H_
#define HEAP_H_

#include "maze.h"

#define HEAPSIZE 100000

class Heap
{
private:
    void PercolateDown(int hole, cell *tmp);
    void PercolateUp(int hole, cell *tmp);
    void PercolateUpordown(int hole, cell *tmp);

    cell *heap_[HEAPSIZE];
    int heapsize_;
    int keylength_;

public:
    Heap() : heapsize_(0), keylength_(3) { EmptyHeap(3); };
    int KeyLess(cell *cell1, cell* cell2);
    void EmptyHeap(int length);
    int TestHeap();
    cell* PopHeap();
    cell *TopHeap();
    void DeleteHeap(cell *thiscell, int mazeiteration);
    void InsertHeap(cell *thiscell, int mazeiteration);
};

#endif // HEAP_H_
