/* D * Lite (final version) - Maxim Likhachev (CMU) and Sven Koenig (USC) */
/* This script is based on http://idm-lab.org/project-a.html */

#include "../include/include.h"
#include "../include/heap.h"
#include "../include/maze.h"
#include "../include/dstarlite.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	DstarLite system;

    srandom(15);

	if (argc==1)
	{
		system.maze_.NewDfsMaze(WALLSTOREMOVE);
	} else if (argc==2)
	{
		std::string filepath =  std::string(argv[1]);
		system.maze_.ReadFromFile(filepath.c_str());
	} else
	{
		return -1;
	}

#ifdef DISPLAY
	system.maze_.PrintActualMaze();
#endif
	system.Initialize();

	system.UpdateLastCell();
	while (!system.IsGoal())
	{
		if (system.ComputeShortestPath())
			break;
#ifdef DISPLAY
		system.maze_.PrintKnownMaze();
#endif
		system.ClearTrace();
		do
		{
			system.SearchMaze();
		} while (!system.IsGoal() && !system.IsObstacle());
		if (!system.IsGoal())
		{
			system.UpdateKeymodifier();
			system.UpdateLastCell();
			for (cell *tmpcell=system.maze_.mazegoal_; tmpcell; tmpcell=tmpcell->trace)
			{
				system.UpdateMaze(tmpcell);
			}
		}			
	}

	return 0;
}
