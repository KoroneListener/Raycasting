#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>
#include <SDL/SDL_stdinc.h>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <cstdio>

extern Uint8* inkeys;

void screen(int width, int height, bool fullscreen, const std::string& text);

void readKeys();

bool done(bool quit_if_esc = true, bool delay = true);

