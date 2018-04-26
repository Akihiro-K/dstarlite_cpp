/* D* Lite (final version) - Maxim Likhachev (CMU) and Sven Koenig (USC) */

#ifndef HEAP_H_
#define HEAP_H_

#include "maze.h"

#define HEAPSIZE 100000

class Heap
{
private:
    void percolatedown(int hole, cell *tmp);
    void percolateup(int hole, cell *tmp);
    void percolateupordown(int hole, cell *tmp);

    cell *heap_[HEAPSIZE];
    int heapsize_;
    int keylength_;

public:
    Heap() : heapsize_(0), keylength_(3) { emptyheap(3); };
    int keyless(cell *cell1, cell* cell2);
    void emptyheap(int length);
    int testheap();
    cell* popheap();
    cell *topheap();
    void deleteheap(cell *thiscell, int mazeiteration);
    void insertheap(cell *thiscell, int mazeiteration);
};

#endif // HEAP_H_
