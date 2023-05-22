#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>
#include <SDL/SDL_stdinc.h>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <cstdio>
#include "screen.h"

Uint8* inkeys = 0;

void screen(int width, int height, bool fullscreen, const std::string& text) {

    int colorDepth = 32;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        std::exit(1);
    }
    atexit(SDL_Quit);

    Uint32 flags = SDL_HWSURFACE | SDL_HWPALETTE;

    if (fullscreen) {
        flags |= SDL_FULLSCREEN;
    }

    SDL_Surface* screenSurface = SDL_SetVideoMode(width, height, colorDepth, flags);

    if (screenSurface == NULL)
    {
        printf("Unable to set video mode : %s\n", SDL_GetError());
        std::exit(1);
    }

    SDL_WM_SetCaption(text.c_str(), NULL);
    SDL_EnableUNICODE(1);

}

void readKeys() {
    SDL_PumpEvents();
     inkeys = SDL_GetKeyState(NULL);
}

bool done(bool quit_if_esc, bool delay) {
    SDL_Event event;

    if (delay) {
        SDL_Delay(5);
    }

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return true;
        }
    }

    readKeys();

    if (inkeys[SDLK_ESCAPE] && quit_if_esc) {
        return true;
    }

    return false;
}

