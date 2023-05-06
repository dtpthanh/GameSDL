#pragma once

#include <vector>
#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


bool isRunning = true;
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

void loadTextures();
void init();
static SDL_Texture *bck;

SDL_Texture* loadTex(const char* path) {
    SDL_Surface *tmpSurface = IMG_Load(path);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);

    SDL_FreeSurface(tmpSurface);
    return tex;
}
