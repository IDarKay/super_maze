//
// Created by alice on 22/02/2022.
//

#include <unistd.h>
#include "playable_labyrinth.h"
#include "../window.h"

playable_labyrinth *playable_labyrinth_create(struct window *window, uint32 width, uint32 height, BOOL is_animated, BOOL two_player, BOOL use_file) {
    playable_labyrinth *self = malloc(sizeof(playable_labyrinth));

    self->is_animated = is_animated;
    self->window = window;
    if (use_file && access("save_test.lab", F_OK) == 0) {
        self->labyrinth = labyrinth_create_from_file("save_test.lab", two_player);
    }
    else self->labyrinth = labyrinth_create(width, height, two_player);

    if (use_file) {
        self->need_generate = FALSE;
        if (is_animated)
            self->need_solve = TRUE;
        else
            self->need_solve = FALSE;

    } else {
        if (is_animated)
            self->need_generate = TRUE;
        else {
            labyrinth_generator_Kruskal(self->labyrinth, FALSE);
            self->need_generate = FALSE;
        }
        self->need_solve = FALSE;
    }

    self->two_players = two_player;

    self->player = self->labyrinth->start_case;
    self->player2 = self->labyrinth->start_case2;

    return self;
}


BOOL try_move_player(player *player, const playable_labyrinth *self, short direction) {
    if (self->is_animated) return FALSE;
    // check if the move is possible
    if (labyrinth_can_pass_through(self->labyrinth, *player, direction)) {
        // change player pose
        *player = labyrinth_get_directional_index(self->labyrinth, *player, direction);
        // if finish case reach show message and go back main menu
        if (*player == self->labyrinth->finish_case) {
            if (self->two_players) {
                if (player == &self->player)
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Victory", "Player 1 win !", self->window->window);
                else
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Victory", "Player 2 win !", self->window->window);
            } else
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Victory", "You win !", self->window->window);
            window_main_menu(self->window);
        }
        return TRUE;
    }
    return FALSE;
}

void playable_labyrinth_key_press(playable_labyrinth *self, const SDL_Keysym *key) {
    // move if directional key is press
    if (key->scancode == SDL_SCANCODE_UP)
        try_move_player(&self->player, self, DIRECTION_TOP);
    else if (key->scancode == SDL_SCANCODE_LEFT)
        try_move_player(&self->player, self, DIRECTION_LEFT);
    else if (key->scancode == SDL_SCANCODE_DOWN)
        try_move_player(&self->player, self, DIRECTION_DOWN);
    else if (key->scancode == SDL_SCANCODE_RIGHT)
        try_move_player(&self->player, self, DIRECTION_RIGHT);
    else if (key->scancode == SDL_SCANCODE_Z && self->two_players)
        try_move_player(&self->player2, self, DIRECTION_TOP);
    else if (key->scancode == SDL_SCANCODE_Q && self->two_players)
        try_move_player(&self->player2, self, DIRECTION_LEFT);
    else if (key->scancode == SDL_SCANCODE_S && self->two_players)
        try_move_player(&self->player2, self, DIRECTION_DOWN);
    else if (key->scancode == SDL_SCANCODE_D && self->two_players)
        try_move_player(&self->player2, self, DIRECTION_RIGHT);
        // refresh if f5
    else if (key->scancode == SDL_SCANCODE_F5) {
        labyrinth_reset(self->labyrinth);

        self->player = self->labyrinth->start_case;
        if (self->is_animated) {
            self->need_generate = TRUE;
            self->need_solve = FALSE;
        } else {
            labyrinth_generator_Kruskal(self->labyrinth, FALSE);
        }
    } else if (key->scancode == SDL_SCANCODE_F3 && !self->is_animated) {
        labyrinth_solve(self->labyrinth, (int) self->player, FALSE);
        labyrinth_calc_path(self->labyrinth, self->player);
    } else if (key->scancode == SDL_SCANCODE_F8) {
        labyrinth_save(self->labyrinth, "save_test.lab");
    }
}


void playable_labyrinth_render(playable_labyrinth *self, SDL_Renderer *renderer, rec *render_rec) {
    // render
    if (self->need_generate) {
        self->need_generate = !labyrinth_generator_Kruskal(self->labyrinth, 40);
        if (!self->need_generate) self->need_solve = TRUE;
    } else if (self->need_solve) {
        self->need_solve = !labyrinth_solve(self->labyrinth, (int) self->player, TRUE);
        if (!self->need_solve) labyrinth_calc_path(self->labyrinth, self->player);
    }

    labyrinth_render(renderer, self->labyrinth, &self->player,  self->two_players ? &self->player2 : NULL, render_rec);
}


void playable_labyrinth_destroy(playable_labyrinth *self) {
    // free mem
    labyrinth_destroy(self->labyrinth);
    free(self);
}
