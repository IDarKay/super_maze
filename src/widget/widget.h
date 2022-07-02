//
// Created by alice on 22/03/2022.
//

#ifndef LABY_WIDGET_H
#define LABY_WIDGET_H

#include "../utils.h"



struct Widget {


    void(* keyPress)(struct Widget*, const SDL_Keysym*);
    void(* render)(struct Widget*, SDL_Renderer*);

    BOOL isEnable;
    BOOL isFocus;
};

typedef void(* keyPressFunc)(struct Widget*, const SDL_Keysym*);

typedef struct Widget Widget;

Widget* widget_create();

void widget_render(Widget* self, SDL_Renderer *renderer);

void widget_key_press(Widget* self, const SDL_Keysym* key);

void widget_destroy(Widget* widget);



#endif //LABY_WIDGET_H
