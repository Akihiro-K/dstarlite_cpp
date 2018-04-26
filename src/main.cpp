/* D * Lite (final version) - Maxim Likhachev (CMU) and Sven Koenig (USC) */
/* This script is based on http://idm-lab.org/project-a.html */

#include "../include/include.h"
#include "../include/heap.h"
#include "../include/maze.h"
#include "../include/dstarlite.h"

#include <iostream>

int main(int argc, char *argv[])
{
	DstarLite system;

#ifdef RANDOMIZESUCCS
    system.createpermutations();
#endif
    srandom(13);

    for (int k = 0; k < RUNS; ++k)
    {
		std::cout << "maze " << k << std::endl;

#ifdef RANDOMMAZE	
		system.maze_.newrandommaze();
#else
		system.maze_.newdfsmaze(WALLSTOREMOVE);
#endif
#ifdef DISPLAY
		system.maze_.printactualmaze();
#endif
		system.initialize();

		system.updatelastcell();
		while (system.isgoal())
		{
	    	if (system.computeshortestpath())
				break;
#ifdef DISPLAY
			system.maze_.printknownmaze();
#endif
			system.cleartrace();
	    	do
			{
				system.searchmaze();
			} while (system.isgoal() && !system.isobstacle());
			if (system.isgoal())
			{
				system.updatekeymodifier();
				system.updatelastcell();
				for (cell *tmpcell=system.maze_.mazegoal_; tmpcell; tmpcell=tmpcell->trace)
					system.updatemaze(tmpcell);
			}
		}
    }

	return 0;
}
