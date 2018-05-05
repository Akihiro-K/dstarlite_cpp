/* D * Lite (final version) - Maxim Likhachev (CMU) and Sven Koenig (USC) */
/* This script is based on http://idm-lab.org/project-a.html */

#ifndef INCLUDE_H_
#define INCLUDE_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cmath>

#define LARGE  1000000

#define DISPLAY                   /* display what happens (in ASCII)                                                     */
#define WALLSTOREMOVE 8           /* number of walls to remove if RANDOMMAZE is NOT defined - infinite loop if too large */
#define MAZEWIDTH 11              /* the width of the maze                                                               */
#define MAZEHEIGHT 11             /* the height of the mze                                                               */
#define MAZEDENSITY 0.25          /* percentage of blocked cells if RANDOMMAZE is defined                                */
#define STARTX 0                  /* x coordinate of the start cell                                                      */
#define STARTY 0                  /* y coordinate of the start cell                                                      */
#define GOALX 10                  /* x coordinate of the goal  cell                                                      */
#define GOALY 10                  /* y coordinate of the goal  cell                                                      */
#define TIEBREAKING               /* tie breaking towards larger g-values (otherwise: smaller g-values)                  */

#define DIRECTIONS 8
static const int dx[DIRECTIONS] = {1, 0, -1,  0, 1, 1, -1, -1};
static const int dy[DIRECTIONS] = {0, 1,  0, -1, 1, -1, -1, 1};
static const int reverse[DIRECTIONS] = {2, 3, 0, 1, 6, 7, 4, 5};
static const float edgecost[DIRECTIONS] = {1, 1, 1, 1, sqrt(2), sqrt(2), sqrt(2), sqrt(2)};

#define H(cell, mazegoal) (abs((cell)->y - mazegoal->y) + abs((cell)->x - mazegoal->x))

#endif // INCLUDE_H_
