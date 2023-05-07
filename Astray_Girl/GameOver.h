#pragma once

#include "Header.h"

void showResults() {
        SDL_Texture* menu = loadTex("menu.png");
        SDL_Rect rect = {450, 100, 318, 450};
        SDL_RenderCopy(renderer, menu, NULL, &rect);
        drawNumber(renderer, kills, 50, 592, 250, 0, 0, 0, 210, 184, 140);
        drawText(renderer, "KILLS", 40, 560, 320, 0, 0, 0, 210, 184, 140);
}

struct Button {
    SDL_Texture* normalTexture;
    SDL_Texture* hoverTexture;
    SDL_Texture* pressedTexture;
    SDL_Rect position; // vi tri button
    bool isHovered; //chuot co o vi tri nut hay khong
    bool isPressed; //chuot co click vao nut hay khong

    void LoadTextures(SDL_Renderer* renderer, const char* normalPath, const char* hoverPath) {
        normalTexture = loadTex(normalPath);
        hoverTexture = loadTex(hoverPath);
    }

    void SetPosition(int x, int y, int width, int height) {
        position.x = x;
        position.y = y;
        position.w = width;
        position.h = height;
    }

    void HandleEvent(SDL_Event& event) {
        if (event.type == SDL_MOUSEMOTION) {
            //khi di chuot den vi tri nut
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;
            isHovered = IsMouseInsideButton(mouseX, mouseY);
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            //khi click chuot o trong pham vi nut
            int mouseX = event.button.x;
            int mouseY = event.button.y;
            if (event.button.button == SDL_BUTTON_LEFT && IsMouseInsideButton(mouseX, mouseY)) {
                isPressed = true;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            //khi click chuot ngoai pham vi nut
            if (event.button.button == SDL_BUTTON_LEFT) {
                isPressed = false;
            }
        }
    }

    void Render(SDL_Renderer* renderer) {
        SDL_Texture* texture = normalTexture;
        if (isPressed) {
            deadCount = 0;  //dat lai so lan chet
            kills = 0;  //dat lai so kill
        }
        else if (isHovered) {
            texture = hoverTexture;
        }
        SDL_RenderCopy(renderer, texture, nullptr, &position);
    }

private:

    bool IsMouseInsideButton(int mouseX, int mouseY) {
        return (mouseX >= position.x && mouseX <= position.x + position.w &&
                mouseY >= position.y && mouseY <= position.y + position.h);
    }
};
