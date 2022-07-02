//
// Created by alice on 22/03/2022.
//

#ifndef LABY_WIDGETCONTROLER_H
#define LABY_WIDGETCONTROLER_H

#include "../utils.h"
#include "widget.h"

struct WidgetController {

    Widget* focus;
    Widget** widgets;
    size_t widgetAmount;
    uint32 height, width;

};

typedef struct WidgetController WidgetController;

WidgetController* widget_controller_create(uint32 height, uint32 width);

void widget_controller_register_widget(WidgetController* self, Widget* widget);

void widget_controller_key_press(WidgetController* self, const SDL_Keysym* key);

void widget_controller_render(WidgetController* self, SDL_Renderer *renderer);

// destroy controller destroy all widgets
void widget_controller_destroy(WidgetController* self);

#endif //LABY_WIDGETCONTROLER_H
