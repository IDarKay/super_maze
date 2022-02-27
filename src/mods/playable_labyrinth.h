//
// Created by alice on 22/02/2022.
//

#ifndef LABY_PLAYABLE_LABYRINTH_H
#define LABY_PLAYABLE_LABYRINTH_H

#include "../utils.h"
#include "../labyrinth.h"

struct window;

struct playable_labyrinth {
    labyrinth* labyrinth;
    struct window* window;

    player player, player2;

    BOOL is_animated;
    BOOL two_players;

    BOOL need_solve;
    BOOL need_generate;
};

typedef struct playable_labyrinth playable_labyrinth;

playable_labyrinth* playable_labyrinth_create(struct window* window, uint32 width, uint32 height, BOOL is_animated, BOOL two_player);

void playable_labyrinth_key_press(playable_labyrinth* self, const SDL_Keysym* key);

void playable_labyrinth_render(playable_labyrinth* self, SDL_Renderer *renderer, rec* render_rec);

void playable_labyrinth_destroy(playable_labyrinth* self);

#endif //LABY_PLAYABLE_LABYRINTH_H
