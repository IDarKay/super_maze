//
// Created by alice on 27/02/2022.
//

#ifndef LABY_EDITABLE_LABYRINTH_H
#define LABY_EDITABLE_LABYRINTH_H

#include "../utils.h"
#include "../labyrinth.h"

struct window;

struct editable_labyrinth {
    labyrinth* labyrinth;

    player editor;
};

typedef struct editable_labyrinth editable_labyrinth;

editable_labyrinth* editable_labyrinth_create(uint32 width, uint32 height);

editable_labyrinth* editable_labyrinth_create_from_file(const char* filename);

void editable_labyrinth_key_press(editable_labyrinth* self,  const SDL_Keysym* key);

void editable_labyrinth_render(editable_labyrinth* self, SDL_Renderer *renderer, rec* render_rec);

void editable_labyrinth_destroy(editable_labyrinth* self);




#endif //LABY_EDITABLE_LABYRINTH_H
