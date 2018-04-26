#include <iostream>

#include "../include/dstarlite.h"
#include "../include/include.h"

void DstarLite::swappermutations(int n)
{
    int i;
    int swap;

    if (n)
		for (i = 0; i <= n; ++i)
		{
			swappermutations(n-1);
			if (n % 2)
			{
				swap = permute[n];
				permute[n] = permute[i];
				permute[i] = swap;
			}
			else
			{
				swap = permute[n];
				permute[n] = permute[0];
				permute[0] = swap;
			}
		}
    else
    {
		for (i = 0; i < DIRECTIONS; ++i)
			permutation[i][permutations] = permute[i];
		permutations++;
    }
}

void DstarLite::createpermutations()
{
    int i, j;

    permutations = 2;
    for (i = 3; i <= DIRECTIONS; ++i)
		permutations *= i;
    for (i = 0; i < DIRECTIONS; ++i)
    {
		permute[i] = i;
		permutation[i] = (int *)calloc(permutations, sizeof(int));
    }
    permutations = 0;
    swappermutations(DIRECTIONS-1);
}

void DstarLite::initialize()
{
    ++maze_.mazeiteration_;
    keymodifier = 0;
    maze_.mazestart_->g = LARGE;
    maze_.mazestart_->rhs = 0;
#ifdef TIEBREAKING
    heap_.emptyheap(3);
    maze_.mazestart_->key[0] = H(maze_.mazestart_, maze_.mazegoal_);
    maze_.mazestart_->key[1] = H(maze_.mazestart_, maze_.mazegoal_) + 1;
    maze_.mazestart_->key[2] = H(maze_.mazestart_, maze_.mazegoal_);
#else
    heap_.emptyheap(2);
    maze_.mazestart_->key[0] = H(maze_.mazestart_);
    maze_.mazestart_->key[1] = 0;
#endif
    maze_.mazestart_->searchtree = NULL;
    maze_.mazestart_->generated = maze_.mazeiteration_;
    heap_.insertheap(maze_.mazestart_, maze_.mazeiteration_);
    maze_.mazegoal_->g = LARGE;
    maze_.mazegoal_->rhs = LARGE;
    maze_.mazegoal_->searchtree = NULL;
    maze_.mazegoal_->generated = maze_.mazeiteration_;
}

void DstarLite::initializecell(cell *thiscell)
{
    if (thiscell->generated != maze_.mazeiteration_)
    {
	thiscell->g = LARGE;
	thiscell->rhs = LARGE;
	thiscell->searchtree = NULL;
	thiscell->generated = maze_.mazeiteration_;
    }
}

void DstarLite::updatecell(cell *thiscell)
{
    if (thiscell->g < thiscell->rhs)
    {
#ifdef TIEBREAKING
	thiscell->key[0] = thiscell->g + H(thiscell, maze_.mazegoal_) + keymodifier;
	thiscell->key[1] = thiscell->g + H(thiscell, maze_.mazegoal_) + keymodifier;
	thiscell->key[2] = thiscell->g;
#else
	thiscell->key[0] = thiscell->g + H(thiscell) + keymodifier;
	thiscell->key[1] = thiscell->g; 
#endif
	heap_.insertheap(thiscell, maze_.mazeiteration_);
    }
    else if (thiscell->g > thiscell->rhs)
    {
#ifdef TIEBREAKING
	thiscell->key[0] = thiscell->rhs + H(thiscell, maze_.mazegoal_) + keymodifier;
	thiscell->key[1] = thiscell->rhs + H(thiscell, maze_.mazegoal_) + keymodifier + 1;
	thiscell->key[2] = H(thiscell, maze_.mazegoal_) + keymodifier;
#else
	thiscell->key[0] = thiscell->rhs + H(thiscell) + keymodifier;
	thiscell->key[1] = thiscell->rhs;
#endif
	heap_.insertheap(thiscell, maze_.mazeiteration_);
    }
    else 
	heap_.deleteheap(thiscell, maze_.mazeiteration_);
}

void DstarLite::updatekey(cell *thiscell)
{
    if (thiscell->g < thiscell->rhs)
    {
#ifdef TIEBREAKING
		thiscell->key[0] = thiscell->g + H(thiscell, maze_.mazegoal_) + keymodifier;
		thiscell->key[1] = thiscell->g + H(thiscell, maze_.mazegoal_) + keymodifier;
		thiscell->key[2] = thiscell->g;
#else
        thiscell->key[0] = thiscell->g + H(thiscell) + keymodifier;
		thiscell->key[1] = thiscell->g; 
#endif
    }
    else 
    {
#ifdef TIEBREAKING
		thiscell->key[0] = thiscell->rhs + H(thiscell, maze_.mazegoal_) + keymodifier;
		thiscell->key[1] = thiscell->rhs + H(thiscell, maze_.mazegoal_) + keymodifier + 1;
		thiscell->key[2] = H(thiscell, maze_.mazegoal_) + keymodifier;
#else
        thiscell->key[0] = thiscell->rhs + H(thiscell) + keymodifier;
        thiscell->key[1] = thiscell->rhs;
#endif
    }
}

void DstarLite::updaterhs(cell *thiscell)
{
    int d;
#ifdef RANDOMIZESUCCS
    int dcase, dtemp;
#endif

    thiscell->rhs = LARGE;
    thiscell->searchtree = NULL;
#ifdef RANDOMIZESUCCS
	dcase = random() % permutations;
	for (dtemp = 0; dtemp < DIRECTIONS; ++dtemp)
	{
		d = permutation[dtemp][dcase];
#else
    for (d = 0; d < DIRECTIONS; ++d)
    {
#endif
		if (thiscell->move[d] && thiscell->move[d]->generated == maze_.mazeiteration_ && thiscell->rhs > thiscell->move[d]->g + 1)
		{
			thiscell->rhs = thiscell->move[d]->g + 1;
			thiscell->searchtree = thiscell->move[d];
		}
    }
    updatecell(thiscell);
}

int DstarLite::computeshortestpath()
{
    cell *tmpcell1, *tmpcell2;
    int x, d;
#ifdef RANDOMIZESUCCS
    int dcase, dtemp;
#endif

#ifdef TIEBREAKING
    if (maze_.mazegoal_->g < maze_.mazegoal_->rhs)
    {
		goaltmpcell.key[0] = maze_.mazegoal_->g + keymodifier;
		goaltmpcell.key[1] = maze_.mazegoal_->g + keymodifier;
		goaltmpcell.key[2] = maze_.mazegoal_->g;
    }
    else
    {
		goaltmpcell.key[0] = maze_.mazegoal_->rhs + keymodifier;
		goaltmpcell.key[1] = maze_.mazegoal_->rhs + keymodifier + 1;
		goaltmpcell.key[2] = keymodifier;
    }
#else
    if (maze_.mazegoal_->g < maze_.mazegoal_->rhs)
    {
		goaltmpcell.key[0] = maze_.mazegoal_->g + keymodifier;
		goaltmpcell.key[1] = maze_.mazegoal_->g;
    }
    else
    {
		goaltmpcell.key[0] = maze_.mazegoal_->rhs + keymodifier;
		goaltmpcell.key[1] = maze_.mazegoal_->rhs;
    }
#endif
    while (heap_.topheap() && (maze_.mazegoal_->rhs > maze_.mazegoal_->g || heap_.keyless(heap_.topheap(), &goaltmpcell)))
    {
		tmpcell1 = heap_.topheap();
		oldtmpcell.key[0] = tmpcell1->key[0];
		oldtmpcell.key[1] = tmpcell1->key[1];
#ifdef TIEBREAKING
		oldtmpcell.key[2] = tmpcell1->key[2];
#endif
		updatekey(tmpcell1); 
		if (heap_.keyless(&oldtmpcell, tmpcell1))
			updatecell(tmpcell1);
		else if (tmpcell1->g > tmpcell1->rhs)
		{
			tmpcell1->g = tmpcell1->rhs;
			heap_.deleteheap(tmpcell1, maze_.mazeiteration_);
#ifdef RANDOMIZESUCCS
	    	dcase = random() % permutations;
			for (dtemp = 0; dtemp < DIRECTIONS; ++dtemp)
			{
			d = permutation[dtemp][dcase];
#else
			for (d = 0; d < DIRECTIONS; ++d)
			{
#endif
				if (tmpcell1->move[d])
				{
					tmpcell2 = tmpcell1->move[d];
					initializecell(tmpcell2);
					if (tmpcell2 != maze_.mazestart_ && tmpcell2->rhs > tmpcell1->g + 1)
					{
						tmpcell2->rhs = tmpcell1->g + 1;
						tmpcell2->searchtree = tmpcell1;
						updatecell(tmpcell2);
					}
				}
	    	}
		}
      	else
      	{
			tmpcell1->g = LARGE;
			updatecell(tmpcell1);
#ifdef RANDOMIZESUCCS
	  		dcase = random() % permutations;
			for (dtemp = 0; dtemp < DIRECTIONS; ++dtemp)
			{
				d = permutation[dtemp][dcase];
#else
			for (d = 0; d < DIRECTIONS; ++d) 
			{
#endif
				if (tmpcell1->move[d])
				{
					tmpcell2 = tmpcell1->move[d];
					initializecell(tmpcell2);
					if (tmpcell2 != maze_.mazestart_ && tmpcell2->searchtree == tmpcell1)
						updaterhs(tmpcell2);
				}
	  		}
      	}
#ifdef TIEBREAKING
		if (maze_.mazegoal_->g < maze_.mazegoal_->rhs)
		{
			goaltmpcell.key[0] = maze_.mazegoal_->g + keymodifier;
			goaltmpcell.key[1] = maze_.mazegoal_->g + keymodifier;
			goaltmpcell.key[2] = maze_.mazegoal_->g;
		}
		else
		{
			goaltmpcell.key[0] = maze_.mazegoal_->rhs + keymodifier;
			goaltmpcell.key[1] = maze_.mazegoal_->rhs + keymodifier + 1;
			goaltmpcell.key[2] = keymodifier;
		}    
#else
		if (maze_.mazegoal_->g < maze_.mazegoal_->rhs)
		{
			goaltmpcell.key[0] = maze_.mazegoal_->g + keymodifier;
			goaltmpcell.key[1] = maze_.mazegoal_->g;
		}
		else
		{
			goaltmpcell.key[0] = maze_.mazegoal_->rhs + keymodifier;
			goaltmpcell.key[1] = maze_.mazegoal_->rhs;
		}
#endif
    }
	return (maze_.mazegoal_->rhs == LARGE);
}

void DstarLite::updatemaze(cell *robot)
{
    int d1, d2;
    cell *tmpcell;
#ifdef RANDOMIZESUCCS
    int dcase, dtemp;
#endif

#ifdef RANDOMIZESUCCS
      dcase = random() % permutations;
      for (dtemp = 0; dtemp < DIRECTIONS; ++dtemp)
      {
	  d1 = permutation[dtemp][dcase];
#else
    for (d1 = 0; d1 < DIRECTIONS; ++d1)
    {
#endif
	if (robot->move[d1] && robot->move[d1]->obstacle)
	{
	    tmpcell = robot->move[d1];
	    initializecell(tmpcell);
	    for (d2 = 0; d2 < DIRECTIONS; ++d2)
		if (tmpcell->move[d2])
		{
		    tmpcell->move[d2] = NULL;
		    tmpcell->succ[d2]->move[reverse[d2]] = NULL;
		    initializecell(tmpcell->succ[d2]);
		    if (tmpcell->succ[d2] != maze_.mazestart_ && tmpcell->succ[d2]->searchtree == tmpcell)
			updaterhs(tmpcell->succ[d2]);
		}
	    if (tmpcell != maze_.mazestart_)
	    {
		tmpcell->rhs = LARGE;
		updatecell(tmpcell);
	    }
	}
    }
}

bool DstarLite::isgoal()
{
	return (maze_.mazestart_ != maze_.mazegoal_);
}

bool DstarLite::isobstacle()
{
	return maze_.mazegoal_->searchtree->obstacle;
}

void DstarLite::updatelastcell()
{
	lastcell = maze_.mazegoal_;
}

void DstarLite::cleartrace()
{
	maze_.mazegoal_->trace = NULL;
}

void DstarLite::updatekeymodifier()
{
	keymodifier += H(lastcell, maze_.mazegoal_);
}

void DstarLite::searchmaze()
{
	maze_.mazegoal_->searchtree->trace = maze_.mazegoal_;
	maze_.mazegoal_ = maze_.mazegoal_->searchtree;
}