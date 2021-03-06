/* D * Lite (final version) - Maxim Likhachev (CMU) and Sven Koenig (USC) */
/* This script is based on http://idm-lab.org/project-a.html */

#include "../include/include.h"
#include "../include/maze.h"

#include "json.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

Maze::Maze(int goaly, int goalx, int starty, int startx, int height, int width)
{
	mazeiteration_ = 0;

	goaly_ = goaly;
	goalx_ = goalx;
	starty_ = starty;
	startx_ = startx;

	height_ = height;
	width_ = width;
}

void Maze::SetMazeParameters(int goaly, int goalx, int starty, int startx, int height, int width)
{
	goaly_ = goaly;
	goalx_ = goalx;
	starty_ = starty;
	startx_ = startx;

	height_ = height;
	width_ = width;	
}

void Maze::PreprocessMaze()
{
    if (!graph_.size())
    {
		// initialize graph
		graph_.resize(height_);
		for (int y=0; y<height_; ++y)
		{
			graph_[y].resize(width_);
		}

		// allocate values for each cells
		for (int y=0; y<height_; ++y)
		{
			for (int x=0; x<width_; ++x)
			{
				graph_[y][x].x = x;
				graph_[y][x].y = y;
				for (int d=0; d<DIRECTIONS; ++d)
				{
					int newy = y + dy[d];
					int newx = x + dx[d];
					graph_[y][x].succ[d] = (newy >= 0 && newy < height_ && newx >= 0 && newx < width_) ? & graph_[newy][newx] : NULL;
				}
			}
		}
    }

    mazestart_ = &graph_[starty_][startx_];
    mazegoal_ = &graph_[goaly_][goalx_];
    mazeiteration_ = 0;	
}

void Maze::PostprocessMaze()
{
    cell *tmpcell;

    for (int y=0; y<height_; ++y)
	{
		for (int x=0; x<width_; ++x)
		{
			graph_[y][x].generated = 0;
			graph_[y][x].heapindex = 0; 
			for (int d1 = 0; d1 < DIRECTIONS; ++d1)
			{
				graph_[y][x].move[d1] = graph_[y][x].succ[d1];
			}
		}
	}
	for (int d1=0; d1<DIRECTIONS; ++d1)
	{
		if (mazegoal_->move[d1] && mazegoal_->move[d1]->obstacle)
		{
			tmpcell = mazegoal_->move[d1];
			for (int d2=0; d2<DIRECTIONS; ++d2)
			{
				if (tmpcell->move[d2])
				{
					tmpcell->move[d2] = NULL;
					tmpcell->succ[d2]->move[reverse[d2]] = NULL;
				}
			}
		}
	}
}

void Maze::NewRandomMaze()
{
    cell *tmpcell;

    PreprocessMaze();
    for (int y=0; y<height_; ++y)
	{
		for (int x=0; x<width_; ++x)
		{
			graph_[y][x].obstacle = (random() % 10000 < 10000 * MAZEDENSITY);
		}
	}
	mazegoal_->obstacle = 0;
    mazestart_->obstacle = 0;
    PostprocessMaze();
}

void Maze::NewDfsMaze(int wallstoremove)
{
    int d, dtmp;
    int x, y;
    int newx, newy;
    int randomnumber;
    cell *tmpcell;
    int permute[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int permutetmp;

    PreprocessMaze();
    for (y = 0; y < height_; ++y)
		for (x = 0; x < width_; ++x)
		{
			graph_[y][x].obstacle = 1;
			graph_[y][x].dfsx = 0;
			graph_[y][x].dfsy = 0;
		}
    x = 0;
    y = 0;
    graph_[y][x].dfsx = -1;
    graph_[y][x].dfsy = -1;
    while (1)
    {
		if (graph_[y][x].obstacle)
			graph_[y][x].obstacle = 0;
		for (d = 0; d < DIRECTIONS-1; ++d)
		{
			randomnumber = random() % (DIRECTIONS-d);
			permutetmp = permute[randomnumber];
			permute[randomnumber] = permute[DIRECTIONS-1-d];
			permute[DIRECTIONS-1-d] = permutetmp;
		}
		for (dtmp = 0; dtmp < DIRECTIONS; ++dtmp)
		{
			d = permute[dtmp];
			newx = x + 2*dx[d];
			newy = y + 2*dy[d];
			if (graph_[y][x].succ[d] != NULL && graph_[newy][newx].obstacle)
			{
				if (graph_[y + dy[d]][x + dx[d]].obstacle)
					graph_[y + dy[d]][x + dx[d]].obstacle = 0;
				graph_[newy][newx].dfsx = x;
				graph_[newy][newx].dfsy = y;
				x = newx;
				y = newy;
				break;
			}
		}
		if (dtmp == DIRECTIONS)
		{
			if (graph_[y][x].dfsx == -1)
			break;
			newx = graph_[y][x].dfsx;
			newy = graph_[y][x].dfsy;
			x = newx;
			y = newy;
		}
    }
    while (wallstoremove)
    {
		newx = random() % width_;
		newy = random() % height_;
		if (graph_[newy][newx].obstacle)
		{
			graph_[newy][newx].obstacle = 0;
			--wallstoremove;
		}
    }
    mazegoal_->obstacle = 0;
    mazestart_->obstacle = 0;
    PostprocessMaze();
}

void Maze::ReadFromFile(std::string filepath)
{
	std::ifstream ifs(filepath);
	json j_;
	ifs >> j_;

	SetMazeParameters(j_["goaly"], j_["goalx"], j_["starty"], j_["startx"], j_["height"], j_["width"]);

    PreprocessMaze();
    for (int i=0; i<(int)j_["obstacles"].size(); ++i)
	{
		std::ostringstream oss;
		oss << i;
		std::string obs_num = oss.str();
		graph_[j_["obstacles"][obs_num.c_str()]["y"]][j_["obstacles"][obs_num.c_str()]["x"]].obstacle = 1;
	}
	mazegoal_->obstacle = 0;
    mazestart_->obstacle = 0;
    PostprocessMaze();	
}

void Maze::PrintActualMaze()
{
    std::cout << "start cell: " << mazegoal_->x << " " << mazegoal_->y << std::endl;
    std::cout << "goal cell: " << mazestart_->x << " " << mazestart_->y << std::endl;

	for (int x = 0; x < width_+2; ++x)
        std::cout << "x";
    std::cout << std::endl;

    for (int y = 0; y < height_; ++y)
    {
	    std::cout << "x";
        for (int x = 0; x < width_; ++x)
        {
            if (mazegoal_ == &graph_[y][x])
                std::cout << "R";
            else if (mazestart_ == &graph_[y][x])
                std::cout << "G";	
            else if (graph_[y][x].obstacle)
                std::cout << "x";
            else
                std::cout << " ";
        }
	    std::cout << "x\n";
    }
    for (int x = 0; x < width_+2; ++x)
        std::cout << "X";
    std::cout << "\n\n\n";
}

void Maze::PrintKnownMaze()
{
    std::vector<std::vector<char>> display;

    if (!display.size())
    {
        display.resize(height_);
	    for (int y = 0; y < height_; ++y)
	        display[y].resize(width_);
    }
    for (int y = 0; y < height_; ++y)
	    for (int x = 0; x < width_; ++x)
	    {
		    display[y][x] = 'X';
		    for (int d = 0; d < DIRECTIONS; ++d)
		        if (graph_[y][x].move[d])
			        display[y][x] = ' ';
	    }
    for (cell *tmpcell = mazegoal_; tmpcell != mazestart_; tmpcell = tmpcell->searchtree)
	{
		display[tmpcell->y][tmpcell->x] = '.';
	}
    display[mazestart_->y][mazestart_->x] = 'G';
    display[mazegoal_->y][mazegoal_->x] = 'R';
    for (int x = 0; x < width_+2; ++x)
        std::cout << "X";
    std::cout << "\n";
    for (int y = 0; y < height_; ++y)
    {
	    std::cout << "X";
	    for (int x = 0; x < width_; ++x)
	        std::cout << (char)display[y][x];
	    std::cout << "X\n";
    }
    for (int x = 0; x < width_+2; ++x)
        std::cout << "X";
    std::cout << "\n\n\n";
}