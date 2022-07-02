//
// Created by alice on 22/03/2022.


#include "WidgetController.h"

WidgetController* widget_controller_create(uint32 height, uint32 width) {
     WidgetController* self = malloc(sizeof(WidgetController));
     self->widgetAmount = 0;
     self->focus = NULL;
     self->widgets = NULL;
     self->height = height;
     self->width = width;
}

void widget_controller_register_widget(WidgetController *self, Widget *widget) {
    if (self == NULL || widget == NULL) {
        fprintf(stderr, "Can't add widget to null controller");
        exit(EXIT_FAILURE);
    }

    if (self->widgetAmount == 0) {
        self->widgetAmount = 1;
        self->widgets = malloc(sizeof(void*));
        self->widgets[0] = widget;
    } else {
        Widget** tmp = malloc((self->widgetAmount + 1) * sizeof(void*));
        for (int cpt = 0 ; cpt < self->widgetAmount ; ++cpt) {
            tmp[cpt] = self->widgets[cpt];
        }
        tmp[self->widgetAmount] = widget;
        self->widgetAmount += 1;
        free(self->widgets);
        self->widgets = tmp;

    }
}

void widget_controller_key_press(WidgetController *self, const SDL_Keysym *key) {

}

void widget_controller_render(WidgetController *self, SDL_Renderer *renderer) {

}

// destroy controller destroy all widgets
void widget_controller_destroy(WidgetController *self) {
//    if (self == NULL) return;
//    if (self->widgets) {
//        for (int i = 0 ; i < self->widgetAmount ; ++i) {
//            widget_destroy(self->widgets[i]);
//        }
//        free(self->widgets);
//        self->widgets = NULL;
//    }
//    free(self->focus);
//    self->focus = NULL;
//    free(self);
}