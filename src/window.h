//
// Created by alice on 22/02/2022.
//

#ifndef LABY_WINDOW_H
#define LABY_WINDOW_H

#include "utils.h"
#include "mods/playable_labyrinth.h"
#include "mods/editable_labyrinth.h"

enum mods {
    MAIN_MENU,
    PLAYABLE_LAB,
    EDITABLE_LAB
};
typedef enum mods mods;

//struct mod {
//
//    mods type;
//
//    void* obj;
//
//
//
//
//};

struct window {
    SDL_Window* window;
    SDL_Renderer* renderer;
    rec window_rec;
    mods mods;

    // possible mods
    playable_labyrinth* p_labyrinth;
    editable_labyrinth* e_labyrinth;

};

typedef struct window window;

window* window_create(int windowX, int windowY);

void window_loop_start(window* self);

void window_main_menu(window* self);

void window_destroy(window* self);

#endif //LABY_WINDOW_H
