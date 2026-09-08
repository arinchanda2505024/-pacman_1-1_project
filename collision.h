#ifndef COLLISION
#define COLLISION

#include <stdbool.h>
#include "raylib.h"


#define wtiles 28
#define htiles 31



bool collision(Rectangle pacman, char map[htiles][wtiles+1]);


#endif
