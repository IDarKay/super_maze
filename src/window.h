//
// Created by alice on 22/02/2022.
//

#ifndef LABY_WINDOW_H
#define LABY_WINDOW_H

#include "utils.h"
#include "mods/playable_labyrinth.h"
#include "mods/editable_labyrinth.h"
#include "mods/main_screen.h"

enum mods {
    NONE,
    MAIN_MENU,
    PLAYABLE_LAB,
    EDITABLE_LAB
};
typedef enum mods mods;

struct main_screen;

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
    main_screen* mainScreen;

    SDL_bool quit;

    TTF_Font* arial;

    int width;
    int height;
    BOOL useFile;
};

typedef struct window window;

window* window_create(int windowX, int windowY);

void window_loop_start(window* self);

void window_main_menu(window* self);

void window_destroy(window* self);

void start_solo(window* self, struct main_screen*);

void start_two(window* self, struct main_screen*);

void start_create(window* self, struct  main_screen*);

void start_animated(window* self, struct main_screen*);
void quit_game(window* self, struct main_screen* ms);

#endif //LABY_WINDOW_H
