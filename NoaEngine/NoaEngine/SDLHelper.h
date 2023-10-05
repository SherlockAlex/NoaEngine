#ifndef NOAENGINE_SDL_HELPER_H
#define NOAENGINE_SDL_HELPER_H

#define SDL_MAIN_HANDLED

#if defined(_WIN64)

#include <SDL.h>
#include <SDL_render.h>
#include <SDL_audio.h>
#include <SDL_gamecontroller.h>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <SDL_mixer.h>
#include <SDL_thread.h>

#elif defined(__linux__)

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_thread.h>

#endif // _WIN64




#endif // !NOAENGINE_SDL_HELPER_H
