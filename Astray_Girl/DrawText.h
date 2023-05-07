#pragma once

#include "Header.h"

void drawText (SDL_Renderer* renderer, char* string, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB) {
    TTF_Font* font = TTF_OpenFont("arial.ttf", size);
    SDL_Color foregroundColor = { fR, fG, fB };
    SDL_Color backgroundColor = { bR, bG, bB };
    SDL_Surface* textSurface = TTF_RenderText_Shaded (font, string, foregroundColor, backgroundColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textLocation = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &textLocation);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}

void drawNumber (SDL_Renderer* renderer, int number, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB) {
    char* str;
    sprintf(str, "%d", number);
    TTF_Font* font = TTF_OpenFont("Roboto.ttf", size);
    SDL_Color foregroundColor = { fR, fG, fB };
    SDL_Color backgroundColor = { bR, bG, bB };
    SDL_Surface* textSurface = TTF_RenderText_Shaded (font, str, foregroundColor, backgroundColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textLocation = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &textLocation);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}
