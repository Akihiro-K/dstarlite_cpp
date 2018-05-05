/* D * Lite (final version) - Maxim Likhachev (CMU) and Sven Koenig (USC) */
/* This script is based on http://idm-lab.org/project-a.html */

#include "../include/include.h"
#include "../include/heap.h"

int Heap::KeyLess(cell *cell1, cell* cell2)
{
    for (int keyindex = 0; keyindex < keylength_; ++keyindex)
    {
        if (cell1->key[keyindex] < cell2->key[keyindex])
            return 1;
        else if (cell1->key[keyindex] > cell2->key[keyindex])
            return 0;
    }
    return 0;
}

int Heap::TestHeap()
{
    for (int d = heapsize_/2; d > 0; d--)
    {
	    assert(!KeyLess(heap_[2*d],heap_[d]));
	    if (2*d+1 <= heapsize_)
	        assert(!KeyLess(heap_[2*d+1],heap_[d]));
    }
}

void Heap::PercolateDown(int hole, cell *tmp)
{
    int child;

    if (heapsize_ != 0)
    {
        for (; 2*hole <= heapsize_; hole = child)
        {
            child = 2*hole;
            if (child != heapsize_ && KeyLess(heap_[child+1], heap_[child]))
                ++child;
            if (KeyLess(heap_[child], tmp))
            {
                heap_[hole] = heap_[child];
                heap_[hole]->heapindex = hole;
            }
            else
                break;
        }
        heap_[hole] = tmp;
        heap_[hole]->heapindex = hole;
    }
}

void Heap::PercolateUp(int hole, cell *tmp)
{
    if (heapsize_ != 0)
    {
        for (; hole > 1 && KeyLess(tmp, heap_[hole/2]); hole /= 2)
        {
            heap_[hole] = heap_[hole/2];
            heap_[hole]->heapindex = hole;
        }
        heap_[hole] = tmp;
        heap_[hole]->heapindex = hole;
    }
}

void Heap::PercolateUpordown(int hole, cell *tmp)
{
    if (heapsize_ != 0)
    {
        if (hole > 1 && KeyLess(tmp, heap_[hole/2]))
            PercolateUp(hole, tmp);
        else
            PercolateDown(hole, tmp);
    }
}

void Heap::EmptyHeap(int length)
{
    keylength_ = length;
    heapsize_ = 0;
}

cell *Heap::TopHeap()
{
    if (heapsize_ == 0)
	    return NULL;
    return heap_[1];
}

void Heap::DeleteHeap(cell *thiscell, int mazeiteration)
{
    if (thiscell->heapindex != 0 && thiscell->generated == mazeiteration)
    {
        PercolateUpordown(thiscell->heapindex, heap_[heapsize_--]);
        thiscell->heapindex = 0;
    }
}

cell* Heap::PopHeap()
{
    cell *thiscell;

    if (heapsize_ == 0)
	    return NULL;
    thiscell = heap_[1];
    thiscell->heapindex = 0;
    PercolateDown(1, heap_[heapsize_--]);
    return thiscell;
}

void Heap::InsertHeap(cell *thiscell, int mazeiteration)
{
    if (thiscell->heapindex == 0 || thiscell->generated != mazeiteration)
    {
	    thiscell->heapindex = 0;
	    PercolateUp(++heapsize_, thiscell);
    }
    else
	    PercolateUpordown(thiscell->heapindex, heap_[thiscell->heapindex]);
}
