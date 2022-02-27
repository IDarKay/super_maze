//
// Created by alice on 22/02/2022.
//

#ifndef LABY_LABYRINTH_H
#define LABY_LABYRINTH_H

#include "utils.h"

struct labyrinth {

    uint32 width, height;
    uint32 start_case, start_case2;
    uint32 finish_case;

    BOOL is_solve;
    /*
     * biggest distance of case to finish_case
     * max_distance = 0 if labyrinth_solve() not call
     */
    uint32 max_distance;

    /* each short represent on case on short is cut like that :
      uint32 (32b): 0b 0000_0000_000 0 0000_0000_0000_0000 0000
      0xFFE0 0000 : NOT USED (7 bits)
      0x0010 0000  : is in path for render (1bits (BOOL))
      0x000F FFF0 : distance of case from finish_case (short (16 bits))
      0x0000 000F : Border status ( 4 bits)
                   DIRECTION_LEFT  0b0001
                   DIRECTION_RIGHT 0b0010
                   DIRECTION_TOP   0b0100
                   DIRECTION_DOWN  0b1000

    */
    uint32 *cases;

    /*
     * temp value for generator algo
     */
    uint32 *generator_tmp;
};

typedef struct labyrinth labyrinth;

/**
 * create new blank maze
 * @param width width in case of maze
 * @param height height in case of maze
 * @param two_player if need to have 2 start_case
 * @return labyrinth* created or NULL in case of error
 */
labyrinth *labyrinth_create(uint32 width, uint32 height, BOOL two_player);

/**
 * create maze from save file
 * @param filename  path to save file
 * @param two_player if need to have 2 start_case
 * @return labyrinth* created or NULL in case of error
 */
labyrinth *labyrinth_create_from_file(const char *filename, BOOL two_player);

/**
 * generate the maze from blank maze
 * @param self maze pointer must be not NULL
 * @param step_by_step generate only step_by_step case for each call -1 for generate all in one step
 * @return true if generate is end or false is must need call more time
 */
BOOL labyrinth_generator_Kruskal(labyrinth * self, int step_by_step);

/**
 * destroy the maze and free mem
 * @param self maze pointer must be not NULL
 */
void labyrinth_destroy(labyrinth *self);

/**
 * reset maze to blank maze
 * @param self maze pointer must be not NULL
 */
void labyrinth_reset(labyrinth *self);

/**
 * render the maze
 * @param renderer SDL_Renderer pointer must be not NULL
 * @param lab maze pointer must be not NULL
 * @param player1 player1 pose
 * @param player2 player2 pose could be NULL
 * @param render_rec rec in the window where must be render the maze
 */
void labyrinth_render(SDL_Renderer *renderer, const labyrinth *lab, const player *player1, const player *player2,
                      const rec *render_rec);

/**
 * get index from direction and current case
 * @param self maze pointer must be not NULL
 * @param current the current index
 * @param direction the direction
 * @return the index of the case or -1 if not exist (exit from the maze or invalid direction)
 */
int labyrinth_get_directional_index(const labyrinth *self, uint32 current, direction direction);

/**
 *  check if player can move from case to another from direction
 * @param self maze pointer must be not NULL
 * @param pos the current index
 * @param direction the direction
 * @return TRUE if no border else FALSE if border or limit of the maze reached
 */
BOOL labyrinth_can_pass_through(const labyrinth *self, uint32 pos, direction direction);

/**
 * return the distance of case from finish_case
 * @param self maze pointer must be not NULL
 * @param index the index of case
 * @return the distance or 0 if maze isn't solve
 */
uint16 labyrinth_get_distance(const labyrinth *self, uint32 index);

/**
 * set distance of case from finish_case
 * @param self maze pointer must be not NULL
 * @param index the index of case
 * @param distance new distance to set
 */
void labyrinth_set_distance(labyrinth *self, uint32 index, uint16 distance);

/**
 * file the 0x0010 0000 bite in self->case with TRUE if case is in path to finish_case
 * if maze isn't solve labyrinth_solve() will call
 * @param self maze pointer must be not NULL
 * @param start_index start index
 */
void labyrinth_calc_path(labyrinth *self, uint32 start_index);

/**
 * save maze to a file
 * @param self maze pointer must be not NULL
 * @param filename filename
 */
BOOL labyrinth_save(const labyrinth *self, const char *filename);

/**
 *
 * @param self
 * @param end_index
 * @param step_by_step
 * @return
 */
BOOL labyrinth_solve(labyrinth *self, int end_index, BOOL step_by_step);

short get_opposite_direction(direction direction);

#endif //LABY_LABYRINTH_H
