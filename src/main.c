#include "window.h"

int main() {
    // inti sdl
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    // create main window
    window* window = window_create(DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT);

    // main loop
    window_loop_start(window);

    // destroy every think
    window_destroy(window);

    SDL_Quit();

    // exit
    return 0;
}
