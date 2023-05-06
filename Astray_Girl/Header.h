#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <SDL_image.h>
#include <SDL_ttf.h>

Uint32 frame_start = SDL_GetTicks();
Uint32 frame_time = SDL_GetTicks() - frame_start;
