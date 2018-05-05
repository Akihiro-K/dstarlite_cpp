/* D * Lite (final version) - Maxim Likhachev (CMU) and Sven Koenig (USC) */
/* This script is based on http://idm-lab.org/project-a.html */

#include <iostream>

#include "../include/dstarlite.h"
#include "../include/include.h"

DstarLite::DstarLite(int goaly, int goalx, int starty, int startx, int height, int width)
{
	if (goaly!=GOALY || goalx!=GOALX || starty!=STARTY || startx!=STARTX || height!=MAZEHEIGHT || width!=MAZEWIDTH)
		maze_.SetMazeParameters(goaly, goalx, starty, startx, height, width);
}

void DstarLite::SwapPermutations(int n)
{
    int i;
    int swap;

    if (n)
		for (i = 0; i <= n; ++i)
		{
			SwapPermutations(n-1);
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

void DstarLite::CreatePermutations()
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
    SwapPermutations(DIRECTIONS-1);
}

void DstarLite::Initialize()
{
    ++maze_.mazeiteration_;
    keymodifier = 0;
    maze_.mazestart_->g = LARGE;
    maze_.mazestart_->rhs = 0;
#ifdef TIEBREAKING
    heap_.EmptyHeap(3);
    maze_.mazestart_->key[0] = H(maze_.mazestart_, maze_.mazegoal_);
    maze_.mazestart_->key[1] = H(maze_.mazestart_, maze_.mazegoal_) + 1;
    maze_.mazestart_->key[2] = H(maze_.mazestart_, maze_.mazegoal_);
#else
    heap_.EmptyHeap(2);
    maze_.mazestart_->key[0] = H(maze_.mazestart_, maze_.mazegoal_);
    maze_.mazestart_->key[1] = 0;
#endif
    maze_.mazestart_->searchtree = NULL;
    maze_.mazestart_->generated = maze_.mazeiteration_;
    heap_.InsertHeap(maze_.mazestart_, maze_.mazeiteration_);
    maze_.mazegoal_->g = LARGE;
    maze_.mazegoal_->rhs = LARGE;
    maze_.mazegoal_->searchtree = NULL;
    maze_.mazegoal_->generated = maze_.mazeiteration_;
}

void DstarLite::InitializeCell(cell *thiscell)
{
    if (thiscell->generated != maze_.mazeiteration_)
    {
		thiscell->g = LARGE;
		thiscell->rhs = LARGE;
		thiscell->searchtree = NULL;
		thiscell->generated = maze_.mazeiteration_;
    }
}

void DstarLite::UpdateCell(cell *thiscell)
{
    if (thiscell->g < thiscell->rhs)
    {
#ifdef TIEBREAKING
	thiscell->key[0] = thiscell->g + H(thiscell, maze_.mazegoal_) + keymodifier;
	thiscell->key[1] = thiscell->g + H(thiscell, maze_.mazegoal_) + keymodifier;
	thiscell->key[2] = thiscell->g;
#else
	thiscell->key[0] = thiscell->g + H(thiscell, maze_.mazegoal_) + keymodifier;
	thiscell->key[1] = thiscell->g; 
#endif
	heap_.InsertHeap(thiscell, maze_.mazeiteration_);
    }
    else if (thiscell->g > thiscell->rhs)
    {
#ifdef TIEBREAKING
	thiscell->key[0] = thiscell->rhs + H(thiscell, maze_.mazegoal_) + keymodifier;
	thiscell->key[1] = thiscell->rhs + H(thiscell, maze_.mazegoal_) + keymodifier + 1;
	thiscell->key[2] = H(thiscell, maze_.mazegoal_) + keymodifier;
#else
	thiscell->key[0] = thiscell->rhs + H(thiscell, maze_.mazegoal_) + keymodifier;
	thiscell->key[1] = thiscell->rhs;
#endif
	heap_.InsertHeap(thiscell, maze_.mazeiteration_);
    }
    else 
	heap_.DeleteHeap(thiscell, maze_.mazeiteration_);
}

void DstarLite::UpdateKey(cell *thiscell)
{
    if (thiscell->g < thiscell->rhs)
    {
#ifdef TIEBREAKING
		thiscell->key[0] = thiscell->g + H(thiscell, maze_.mazegoal_) + keymodifier;
		thiscell->key[1] = thiscell->g + H(thiscell, maze_.mazegoal_) + keymodifier;
		thiscell->key[2] = thiscell->g;
#else
        thiscell->key[0] = thiscell->g + H(thiscell, maze_.mazegoal_) + keymodifier;
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
        thiscell->key[0] = thiscell->rhs + H(thiscell, maze_.mazegoal_) + keymodifier;
        thiscell->key[1] = thiscell->rhs;
#endif
    }
}

void DstarLite::UpdateRHS(cell *thiscell)
{
    int d;

    thiscell->rhs = LARGE;
    thiscell->searchtree = NULL;
    for (d = 0; d < DIRECTIONS; ++d)
    {
		if (thiscell->move[d] && thiscell->move[d]->generated == maze_.mazeiteration_ && thiscell->rhs > thiscell->move[d]->g + edgecost[d])
		{
			thiscell->rhs = thiscell->move[d]->g + edgecost[d];
			thiscell->searchtree = thiscell->move[d];
		}
    }
    UpdateCell(thiscell);
}

int DstarLite::ComputeShortestPath()
{
    cell *tmpcell1, *tmpcell2;
    int x, d;

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
    while (heap_.TopHeap() && (maze_.mazegoal_->rhs > maze_.mazegoal_->g || heap_.KeyLess(heap_.TopHeap(), &goaltmpcell)))
    {
		tmpcell1 = heap_.TopHeap();
		oldtmpcell.key[0] = tmpcell1->key[0];
		oldtmpcell.key[1] = tmpcell1->key[1];
#ifdef TIEBREAKING
		oldtmpcell.key[2] = tmpcell1->key[2];
#endif
		UpdateKey(tmpcell1); 
		if (heap_.KeyLess(&oldtmpcell, tmpcell1))
			UpdateCell(tmpcell1);
		else if (tmpcell1->g > tmpcell1->rhs)
		{
			tmpcell1->g = tmpcell1->rhs;
			heap_.DeleteHeap(tmpcell1, maze_.mazeiteration_);
			for (d = 0; d < DIRECTIONS; ++d)
			{
				if (tmpcell1->move[d])
				{
					tmpcell2 = tmpcell1->move[d];
					InitializeCell(tmpcell2);
					if (tmpcell2 != maze_.mazestart_ && tmpcell2->rhs > tmpcell1->g + edgecost[d])
					{
						tmpcell2->rhs = tmpcell1->g + edgecost[d];
						tmpcell2->searchtree = tmpcell1;
						UpdateCell(tmpcell2);
					}
				}
	    	}
		}
      	else
      	{
			tmpcell1->g = LARGE;
			UpdateCell(tmpcell1);
			for (d = 0; d < DIRECTIONS; ++d) 
			{
				if (tmpcell1->move[d])
				{
					tmpcell2 = tmpcell1->move[d];
					InitializeCell(tmpcell2);
					if (tmpcell2 != maze_.mazestart_ && tmpcell2->searchtree == tmpcell1)
						UpdateRHS(tmpcell2);
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

void DstarLite::UpdateMaze(cell *robot)
{
    int d1, d2;
    cell *tmpcell;

    for (d1 = 0; d1 < DIRECTIONS; ++d1)
    {
		if (robot->move[d1] && robot->move[d1]->obstacle)
		{
			tmpcell = robot->move[d1];
			InitializeCell(tmpcell);
			for (d2 = 0; d2 < DIRECTIONS; ++d2)
				if (tmpcell->move[d2])
				{
					tmpcell->move[d2] = NULL;
					tmpcell->succ[d2]->move[reverse[d2]] = NULL;
					InitializeCell(tmpcell->succ[d2]);
					if (tmpcell->succ[d2] != maze_.mazestart_ && tmpcell->succ[d2]->searchtree == tmpcell)
					UpdateRHS(tmpcell->succ[d2]);
				}
			if (tmpcell != maze_.mazestart_)
			{
				tmpcell->rhs = LARGE;
				UpdateCell(tmpcell);
			}
		}
    }
}

bool DstarLite::IsGoal()
{
	return (maze_.mazestart_ == maze_.mazegoal_);
}

bool DstarLite::IsObstacle()
{
	return maze_.mazegoal_->searchtree->obstacle;
}

void DstarLite::UpdateLastCell()
{
	lastcell = maze_.mazegoal_;
}

void DstarLite::ClearTrace()
{
	maze_.mazegoal_->trace = NULL;
}

void DstarLite::UpdateKeymodifier()
{
	keymodifier += H(lastcell, maze_.mazegoal_);
}

void DstarLite::SearchMaze()
{
	maze_.mazegoal_->searchtree->trace = maze_.mazegoal_;
	maze_.mazegoal_ = maze_.mazegoal_->searchtree;
}