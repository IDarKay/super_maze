//
// Created by alice on 27/02/2022.
//

#ifndef LABY_MAIN_SCREEN_H
#define LABY_MAIN_SCREEN_H

#include "../utils.h"
#include "../labyrinth.h"

struct main_screen {

    labyrinth * background_maze;

    BOOL need_solve;
    BOOL need_generate;

    SDL_Texture* logo;
    SDL_Rect logo_rect;
};

typedef struct main_screen main_screen;

main_screen* main_screen_create(uint32 win_width, uint32 win_height, SDL_Renderer *renderer);

void main_screen_render(main_screen* self, SDL_Renderer *renderer, rec *render_rec);

void main_screen_key_press(main_screen* self, const SDL_Keysym* key);

void main_screen_screen_resize(main_screen* self, Sint32 width, Sint32 height);

void main_screen_destroy(main_screen* self);


#endif //LABY_MAIN_SCREEN_H
