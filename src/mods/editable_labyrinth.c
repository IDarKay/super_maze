//
// Created by alice on 27/02/2022.
//

#include "editable_labyrinth.h"


editable_labyrinth *editable_labyrinth_create(uint32 width, uint32 height) {
    editable_labyrinth *self = malloc(sizeof(editable_labyrinth));
    self->labyrinth = labyrinth_create(width, height, FALSE);
    self->editor = self->labyrinth->start_case;

    return self;
}

editable_labyrinth *editable_labyrinth_create_from_file(const char *filename) {
    editable_labyrinth *self = malloc(sizeof(editable_labyrinth));
    self->labyrinth = labyrinth_create_from_file(filename, FALSE);
    self->editor = self->labyrinth->start_case;

    return self;
}

BOOL e_try_move_player(player *player, const editable_labyrinth *self, direction direction) {

    // change player pose
    int index = labyrinth_get_directional_index(self->labyrinth, *player, direction);
    if (index != -1) {
        *player = index;
        return TRUE;
    }
    // if finish case reach show message and go back main menu
    return FALSE;
}

BOOL try_change_case_border(const player *player, const editable_labyrinth *self, direction direction) {
    // change player pose
    int index = labyrinth_get_directional_index(self->labyrinth, *player, direction);
    // if not out of maze
    if (index != -1) {
        self->labyrinth->cases[*player] ^= direction;
        self->labyrinth->cases[index] ^= get_opposite_direction(direction);
        return TRUE;
    }
    // if finish case reach show message and go back main menu
    return FALSE;
}

void editable_labyrinth_key_press(editable_labyrinth *self, const SDL_Keysym *key) {
    if (key->scancode == SDL_SCANCODE_UP)
        try_change_case_border(&self->editor, self, DIRECTION_TOP);
    else if (key->scancode == SDL_SCANCODE_LEFT)
        try_change_case_border(&self->editor, self, DIRECTION_LEFT);
    else if (key->scancode == SDL_SCANCODE_DOWN)
        try_change_case_border(&self->editor, self, DIRECTION_DOWN);
    else if (key->scancode == SDL_SCANCODE_RIGHT)
        try_change_case_border(&self->editor, self, DIRECTION_RIGHT);
    else if (key->scancode == SDL_SCANCODE_Z)
        e_try_move_player(&self->editor, self, DIRECTION_TOP);
    else if (key->scancode == SDL_SCANCODE_Q)
        e_try_move_player(&self->editor, self, DIRECTION_LEFT);
    else if (key->scancode == SDL_SCANCODE_S)
        e_try_move_player(&self->editor, self, DIRECTION_DOWN);
    else if (key->scancode == SDL_SCANCODE_D)
        e_try_move_player(&self->editor, self, DIRECTION_RIGHT);
    else if (key->scancode == SDL_SCANCODE_F8) {
        labyrinth_save(self->labyrinth, "save_test.lab");
    }
}

void editable_labyrinth_render(editable_labyrinth *self, SDL_Renderer *renderer, rec *render_rec) {
    labyrinth_render(renderer, self->labyrinth, &self->editor, NULL, render_rec);
}

void editable_labyrinth_destroy(editable_labyrinth *self) {
    if (self == NULL) return;
    if (self->labyrinth != NULL) labyrinth_destroy(self->labyrinth);

    free(self);
}
