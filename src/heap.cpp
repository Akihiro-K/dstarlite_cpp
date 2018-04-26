/* D* Lite (final version) - Maxim Likhachev (CMU) and Sven Koenig (USC) */

#include "../include/include.h"
#include "../include/heap.h"

int Heap::keyless(cell *cell1, cell* cell2)
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

int Heap::testheap()
{
    for (int d = heapsize_/2; d > 0; d--)
    {
	    assert(!keyless(heap_[2*d],heap_[d]));
	    if (2*d+1 <= heapsize_)
	        assert(!keyless(heap_[2*d+1],heap_[d]));
    }
}

void Heap::percolatedown(int hole, cell *tmp)
{
    int child;

    if (heapsize_ != 0)
    {
        for (; 2*hole <= heapsize_; hole = child)
        {
            child = 2*hole;
            if (child != heapsize_ && keyless(heap_[child+1], heap_[child]))
                ++child;
            if (keyless(heap_[child], tmp))
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

void Heap::percolateup(int hole, cell *tmp)
{
    if (heapsize_ != 0)
    {
        for (; hole > 1 && keyless(tmp, heap_[hole/2]); hole /= 2)
        {
            heap_[hole] = heap_[hole/2];
            heap_[hole]->heapindex = hole;
        }
        heap_[hole] = tmp;
        heap_[hole]->heapindex = hole;
    }
}

void Heap::percolateupordown(int hole, cell *tmp)
{
    if (heapsize_ != 0)
    {
        if (hole > 1 && keyless(tmp, heap_[hole/2]))
            percolateup(hole, tmp);
        else
            percolatedown(hole, tmp);
    }
}

void Heap::emptyheap(int length)
{
    keylength_ = length;
    heapsize_ = 0;
}

cell *Heap::topheap()
{
    if (heapsize_ == 0)
	    return NULL;
    return heap_[1];
}

void Heap::deleteheap(cell *thiscell, int mazeiteration)
{
    if (thiscell->heapindex != 0 && thiscell->generated == mazeiteration)
    {
        percolateupordown(thiscell->heapindex, heap_[heapsize_--]);
        thiscell->heapindex = 0;
    }
}

cell* Heap::popheap()
{
    cell *thiscell;

    if (heapsize_ == 0)
	    return NULL;
    thiscell = heap_[1];
    thiscell->heapindex = 0;
    percolatedown(1, heap_[heapsize_--]);
    return thiscell;
}

void Heap::insertheap(cell *thiscell, int mazeiteration)
{
    if (thiscell->heapindex == 0 || thiscell->generated != mazeiteration)
    {
	    thiscell->heapindex = 0;
#ifdef DEBUG
	    assert(heapsize_ < HEAPSIZE-1);
#endif
	    percolateup(++heapsize_, thiscell);
    }
    else
	    percolateupordown(thiscell->heapindex, heap_[thiscell->heapindex]);
}
