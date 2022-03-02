//
// Created by alice on 27/02/2022.
//

#include "main_screen.h"

void build_lab(main_screen *self, uint32 win_width, uint32 win_height) {
    if (self->background_maze != NULL) {
        labyrinth_destroy(self->background_maze);
    }
    if (win_width > 40 && win_height > 40) {
        self->background_maze = labyrinth_create(win_width / 10, win_height / 10, FALSE);
    } else {
        self->background_maze = NULL;
    }
}

main_screen *main_screen_create(uint32 win_width, uint32 win_height, SDL_Renderer *renderer) {
    main_screen *self = malloc(sizeof(main_screen));
    self->background_maze = NULL;

    build_lab(self, win_width, win_height);

    self->need_generate = TRUE;
    self->need_solve = FALSE;

    SDL_Surface* image = SDL_LoadBMP("res/logo.bmp");

    if (!image) {
        fprintf(stderr, "Error loading image : %s", SDL_GetError());
        main_screen_destroy(self);
        return NULL;
    }

    self->logo = SDL_CreateTextureFromSurface(renderer, image);
    self->logo_rect = (SDL_Rect) {0, 0, 0, 0};
    SDL_QueryTexture(self->logo, NULL, NULL, &self->logo_rect.w, &self->logo_rect.h);

    SDL_FreeSurface(image);

    return self;

}



void main_screen_screen_resize(main_screen* self, Sint32 width, Sint32 height) {
    build_lab(self, width, height);
}

void main_screen_render(main_screen *self, SDL_Renderer *renderer, rec *render_rec) {

    SDL_Rect rect = {render_rec->x1, render_rec->y1, render_rec->x2 - render_rec->x1, render_rec->y2 - render_rec->y1 };

    if (self->need_generate) {
        self->need_generate = !labyrinth_generator_Kruskal(self->background_maze, 50);
        if (!self->need_generate) self->need_solve = TRUE;
    } else if (self->need_solve) {
        self->need_solve = !labyrinth_solve(self->background_maze, -1, TRUE);
        if (!self->need_solve) {
            labyrinth_reset(self->background_maze);
            self->need_generate = TRUE;
        }
    }

    if (self->background_maze != NULL) {
        labyrinth_render(renderer, self->background_maze, NULL, NULL, render_rec);
    }


    SDL_SetRenderDrawColor(renderer, 138, 145, 156, 180);
    SDL_RenderFillRect(renderer, NULL);

    SDL_RenderCopy(renderer, self->logo, NULL, &(SDL_Rect) {rect.x + (rect.w / 2) - self->logo_rect.w / 2,
                                                            rect.y + rect.h / 10,self->logo_rect.w,self->logo_rect.h});

}

void main_screen_key_press(main_screen *self, const SDL_Keysym *key) {

}

void main_screen_destroy(main_screen *self) {
    if (self->background_maze != NULL) {
        labyrinth_destroy(self->background_maze);
        self->background_maze = NULL;
    }
    if (self->logo) {
        SDL_DestroyTexture(self->logo);
        self->logo = NULL;
    }
    free(self);
}