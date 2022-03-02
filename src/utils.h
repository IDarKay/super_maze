//
// Created by alice on 22/02/2022.
//

#ifndef LABY_UTILS_H
#define LABY_UTILS_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>


#define MAP_WIDTH 30
#define MAP_HEIGHT 30

#define DEFAULT_WIN_WIDTH 920
#define DEFAULT_WIN_HEIGHT 920

#define BOOL int
#define TRUE 1
#define FALSE 0

#define DIRECTION_NONE 0b0000
#define DIRECTION_LEFT 0b0001
#define DIRECTION_RIGHT 0b0010
#define DIRECTION_TOP 0b0100
#define DIRECTION_DOWN 0b1000

#define DIRECTION_ALL 0xF
#define DIRECTION_MASK DIRECTION_ALL

#define DISTANCE_MASK 0xFFFF0
#define DISTANCE_OFFSET 4

#define IS_IN_PATH_MASK 0x100000

struct rec {
    int x1, y1, x2, y2;
};

typedef struct rec rec;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

typedef uint32 player;
typedef uint16 direction;

#endif //LABY_UTILS_H
