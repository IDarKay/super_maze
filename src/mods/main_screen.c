//
// Created by alice on 27/02/2022.
//

#include "main_screen.h"
#include "../window.h"

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

void p_set_show_text_width(struct main_screen* self, int value) {
    snprintf(self->menus[3].name, 13, "Largeur: %d", value);
}

void p_set_show_text_height(struct main_screen* self, int value) {
    snprintf(self->menus[4].name, 13, "Hauteur: %d", value);
}


void p_height(struct window* window, struct main_screen* self, BOOL right) {
    int v = window->height;
    if (right && v < 100){
        v++;
    }
    if (!right && v > 5){
        v--;
    }
    window->height = v;
    p_set_show_text_height(self, v);
}

void p_width(struct window* window, struct main_screen* self, BOOL right) {
    int v = window->width;
    if (right && v < 100){
        v++;
    }
    if (!right && v > 5){
        v--;
    }
    window->width = v;
    p_set_show_text_width(self, v);
}



void p_map(struct window* window, struct main_screen* self) {
    if (window->useFile) {
        self->menus[5].name = "Charger une map";
    } else {
        self->menus[5].name = "Decharger la map";
    }
    window->useFile ^= 1;
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

    self->amountMenu = 8;

    self->menus = malloc(sizeof (struct  menu) * self->amountMenu);

     self->menus[0] = (struct menu) {"Jouer", start_solo, NULL, FALSE};
     self->menus[1] = (struct menu) {"Jouer à 2", start_two, NULL, FALSE};
     self->menus[2] = (struct menu) {"Automaituqe", start_animated, NULL, FALSE};
     self->menus[3] = (struct menu) {malloc(sizeof (char ) * 13), NULL, p_width, TRUE};
     self->menus[4] = (struct menu) {malloc(sizeof (char ) * 13), NULL, p_height, TRUE};
     self->menus[5] = (struct menu) {"Charger une map", p_map, NULL, FALSE};
     self->menus[6] = (struct menu) {"Modifier / Créer", start_create, NULL, FALSE};
     self->menus[7] = (struct menu) {"Quitter", quit_game, NULL, FALSE};
     p_set_show_text_width(self, MAP_WIDTH);
     p_set_show_text_height(self, MAP_HEIGHT);
     self->currentSelected = 0;

    return self;
}


void main_screen_screen_resize(main_screen* self, Sint32 width, Sint32 height) {
    build_lab(self, width, height);
    self->need_generate = TRUE;
    self->need_solve = FALSE;
}

void main_screen_render(main_screen *self, SDL_Renderer *renderer, rec *render_rec, TTF_Font * font) {
    SDL_Color color = { 255, 255, 255 };
    SDL_Color color2 = { 255, 10, 10 };
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


    for (int cpt = 0 ; cpt < self->amountMenu ; ++cpt) {
        struct menu m = self->menus[cpt];
        SDL_Surface * surface = TTF_RenderUTF8_Blended(font,m.name,  self->currentSelected == cpt ? color2 : color);
        SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

        int texW = 0;
        int texH = 0;
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
        SDL_Rect dstrect = { rect.x + (rect.w / 2) - texW / 2, (rect.y + rect.h / 10 * 3) + 45 * cpt , texW, texH };

        SDL_RenderCopy(renderer, texture, NULL, &dstrect);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

}

void main_screen_key_press(main_screen *self, const SDL_Keysym *key, window* window) {
    if (key->scancode == SDL_SCANCODE_DOWN) {
        self->currentSelected += 1;
        self->currentSelected %= self->amountMenu;
    } else if (key->scancode == SDL_SCANCODE_UP) {
        self->currentSelected -= 1;
        if (self->currentSelected < 0) self->currentSelected = self->amountMenu - 1;
    } else if (key->scancode == SDL_SCANCODE_SPACE) {
        if (self->menus[self->currentSelected].click != NULL) self->menus[self->currentSelected].click(window, self);
    } else if (key->scancode == SDL_SCANCODE_LEFT) {
        if (self->menus[self->currentSelected].increas != NULL) self->menus[self->currentSelected].increas(window, self, 0);
    } else if (key->scancode == SDL_SCANCODE_RIGHT) {
        if (self->menus[self->currentSelected].increas != NULL) self->menus[self->currentSelected].increas(window, self, 1);
    }
}

void main_screen_destroy(main_screen *self) {
    if (self->background_maze != NULL) {
        labyrinth_destroy(self->background_maze);
        self->background_maze = NULL;
    }
    if (self->logo != NULL) {
        SDL_DestroyTexture(self->logo);
        self->logo = NULL;
    }
    for (int cpt = 0 ; cpt < self->amountMenu ; ++cpt) {
        if (self->menus[cpt].need_free == TRUE) free(self->menus[cpt].name);
    }
    free(self->menus);

    free(self);
}