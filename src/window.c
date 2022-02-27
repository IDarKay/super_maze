//
// Created by alice on 22/02/2022.
//

#include "window.h"


window* window_create(int windowX, int windowY) {

    window* self = malloc(sizeof(window));

    self->mods = MAIN_MENU;
    self->window_rec = (rec) {0, 0, windowX, windowY};

    self->window = SDL_CreateWindow(
            "labyrinth",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            windowX,
            windowY,
            SDL_WINDOW_RESIZABLE
    );

    if (self->window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        window_destroy(self);
        exit(EXIT_FAILURE);
    }

    self->renderer = SDL_CreateRenderer(self->window, -1, SDL_RENDERER_SOFTWARE);

    return self;
}


void window_loop_start(window *self) {
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;

    // main loop
    while (!quit) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = SDL_TRUE;
            else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    self->window_rec = (rec) {0, 0, event.window.data1, event.window.data2};
                }
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && self->mods != MAIN_MENU) {
                    window_main_menu(self);
                }
                else if (self->mods == MAIN_MENU) {
                    if (event.key.keysym.scancode == SDL_SCANCODE_Z || event.key.keysym.scancode == SDL_SCANCODE_A || event.key.keysym.scancode == SDL_SCANCODE_M) {
                        self->mods = PLAYABLE_LAB;
                        self->p_labyrinth = playable_labyrinth_create(self, MAP_WIDTH, MAP_HEIGHT,
                                                                      event.key.keysym.scancode == SDL_SCANCODE_A, event.key.keysym.scancode == SDL_SCANCODE_M);
                    } else if (event.key.keysym.scancode == SDL_SCANCODE_C) {
                        self->mods = EDITABLE_LAB;
                        self->e_labyrinth = editable_labyrinth_create(MAP_WIDTH, MAP_HEIGHT);
                    }
                } else if (self->mods == PLAYABLE_LAB) {
                    playable_labyrinth_key_press(self->p_labyrinth, &event.key.keysym);
                } else if (self->mods == EDITABLE_LAB) {
                    editable_labyrinth_key_press(self->e_labyrinth, &event.key.keysym);
                }
            }
        }

        SDL_SetRenderDrawColor(self->renderer, 220, 220, 220, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(self->renderer);

        if (self->mods == PLAYABLE_LAB) {
            playable_labyrinth_render(self->p_labyrinth,self->renderer, &self->window_rec);
        } else if (self->mods == EDITABLE_LAB) {
            editable_labyrinth_render(self->e_labyrinth, self->renderer, &self->window_rec);
        }

        SDL_RenderPresent(self->renderer);
        SDL_Delay(16);
    }
}

void window_main_menu(window* self) {
    self->mods = MAIN_MENU;

    if (self->p_labyrinth != NULL) {
        playable_labyrinth_destroy(self->p_labyrinth);
        self->p_labyrinth = NULL;
    } else if (self->e_labyrinth != NULL) {
        editable_labyrinth_destroy(self->e_labyrinth);
        self->e_labyrinth = NULL;
    }
}

void window_destroy(window *self) {
    // free mem
    if (self->p_labyrinth != NULL) playable_labyrinth_destroy(self->p_labyrinth);
    if (self->e_labyrinth != NULL) editable_labyrinth_destroy(self->e_labyrinth);

    if (self->window != NULL) SDL_DestroyWindow(self->window);
    if (self->renderer != NULL) SDL_DestroyRenderer(self->renderer);

    free(self);

}