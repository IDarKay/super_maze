//
// Created by alice on 22/02/2022.
//

#include "window.h"
#include "mods/main_screen.h"

window* window_create(int windowX, int windowY) {

    window* self = malloc(sizeof(window));

    self->quit = SDL_FALSE;
    self->mods = NONE;
    self->mainScreen = NULL;
    self->e_labyrinth = NULL;
    self->p_labyrinth = NULL;
    self->height = MAP_HEIGHT;
    self->width = MAP_WIDTH;
    self->useFile = FALSE;

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
    SDL_SetRenderDrawBlendMode(self->renderer, SDL_BLENDMODE_BLEND);

    window_main_menu(self);

    self->arial = TTF_OpenFont("res/arial.ttf", 25);
    if (self->arial == NULL) {
        fprintf(stderr, "Can't find arial.TTF exist");
        //todo free
        exit(EXIT_FAILURE);
    }

    return self;
}


void window_loop_start(window *self) {
    SDL_Event event;


    // main loop
    while (!self->quit) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                self->quit = SDL_TRUE;
            else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    self->window_rec = (rec) {0, 0, event.window.data1, event.window.data2};
                    if (self->mainScreen != NULL) main_screen_screen_resize(self->mainScreen, event.window.data1, event.window.data2);
                }
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && self->mods != MAIN_MENU) {
                    window_main_menu(self);
                } else if (self->mods == MAIN_MENU) {
                    main_screen_key_press(self->mainScreen, &event.key.keysym, self);
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
        } else if (self->mods == MAIN_MENU) {
            main_screen_render(self->mainScreen, self->renderer, &self->window_rec, self->arial);
        }

        SDL_RenderPresent(self->renderer);
        SDL_Delay(16);
    }
}

void p_start_lab(window* self, BOOL is_animated, BOOL two_player) {
    if (self->mods != MAIN_MENU) return;

//    if (self->mainScreen != NULL) {
//        main_screen_destroy(self->mainScreen);
//        self->mainScreen = NULL;
//    }
    self->mods = PLAYABLE_LAB;
    self->p_labyrinth = playable_labyrinth_create(self, self->width, self->height, is_animated, two_player, self->useFile);
}

void start_solo(window* self, struct main_screen* ms) {
    p_start_lab(self, FALSE, FALSE);
}

void start_two(window* self, struct main_screen* ms) {
    p_start_lab(self, FALSE, TRUE);
}

void start_animated(window* self, struct main_screen* ms) {
    p_start_lab(self, TRUE, FALSE);
}

void start_create(window* self, struct  main_screen* ms) {
    if (self->mods != MAIN_MENU) return;

//    if (self->mainScreen != NULL) {
//        main_screen_destroy(self->mainScreen);
//        self->mainScreen = NULL;
//    }
    self->mods = EDITABLE_LAB;
    if (self->useFile) self->e_labyrinth = editable_labyrinth_create_from_file("save_test.lab");
    else self->e_labyrinth = editable_labyrinth_create(MAP_WIDTH, MAP_HEIGHT);
}

void quit_game(window* self, struct main_screen* ms) {
    self->quit = TRUE;
}

void window_main_menu(window* self) {
    if (self->mods != MAIN_MENU || self->mainScreen == NULL) {
        self->mods = MAIN_MENU;

        if (self->p_labyrinth != NULL) {
            playable_labyrinth_destroy(self->p_labyrinth);
            self->p_labyrinth = NULL;
        } else if (self->e_labyrinth != NULL) {
            editable_labyrinth_destroy(self->e_labyrinth);
            self->e_labyrinth = NULL;
        }
        if (self->mainScreen == NULL) {
            self->mainScreen = main_screen_create(self->window_rec.x2 - self->window_rec.x1, self->window_rec.y2 - self->window_rec.y1, self->renderer);
            if (!self->mainScreen) self->quit = SDL_TRUE;
        }
    }
}

void window_destroy(window *self) {
    if (self == NULL) return;

    if (self->p_labyrinth != NULL) {
        playable_labyrinth_destroy(self->p_labyrinth);
        self->p_labyrinth = NULL;
    }
    if (self->e_labyrinth != NULL) {
        editable_labyrinth_destroy(self->e_labyrinth);
        self->e_labyrinth = NULL;
    }
    if (self->mainScreen != NULL) {
        main_screen_destroy(self->mainScreen);
        self->mainScreen = NULL;
    }

    if (self->window != NULL) {
        SDL_DestroyWindow(self->window);
        self->window = NULL;
    }
    if (self->renderer != NULL) {
        SDL_DestroyRenderer(self->renderer);
        self->renderer = NULL;
    }
    if (self->arial != NULL)
        TTF_CloseFont(self->arial);
    free(self);

}