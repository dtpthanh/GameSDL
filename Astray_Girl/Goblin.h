#pragma once

#include "Character.h"
#include "Header.h"

struct Goblin{
    SDL_Texture *texture;
    Pair pos, velo;
    SDL_Rect src, dest;

    std::string state;
    int g_idle, g_run, g_attack, g_death;
    SDL_Texture *idle, *run, *attack, *death;

    void init(){
        g_idle = 4;
        idle = loadTex("Goblin_Idle.png");

        g_run = 8;
        run = loadTex("Goblin_Run.png");

        g_attack = 8;
        attack = loadTex("Goblin_Attack.png");

        g_death = 4;
        death = loadTex("Goblin_Death.png");

        g_flip = false;
        setPos(750, 435);
        src.w = src.h = 150;
        dest.w = dest.h = 250;
    }

    void setPos(int x, int y) {
        pos.x = x; pos.y = y;
    }
    void setSrc(int k) {
        src.y = 0;
        src.x = 150 * k;
    }

    void handle() {
        velo.x = 0;
        if(g_pos.x - p_pos.x <= 10 && g_pos.x - p_pos.x >= -30) {
            state = "attack";
            g_flip = true;
        }
        else if(g_pos.x - p_pos.x < -30 && g_pos.x - p_pos.x >= -110) {
            state = "attack";
            g_flip = false;
        }
        else if(g_pos.x - p_pos.x >= -30  ) {
            state = "run";
            g_flip = true;
            velo.x -= 3.5;
        }
        else if(g_pos.x - p_pos.x <= -50 )
        {
            state = "run";
            g_flip = false;
            velo.x += 3.5;
        }
        else {
            state = "idle";
        }
        if(g_Dead) {
            state = "death";
            g_time=0;
        }
    }
    void update() {
        g_pos.x = pos.x += velo.x;
        g_pos.y = pos.y += velo.y;
        dest.x = pos.xx() + offset;
        dest.y = pos.yy();

        if(pos.y > 435) velo.y = 5;
    }

    void draw() {
        SDL_RendererFlip FLIP;
        if (g_flip == false) FLIP = SDL_FLIP_NONE;
        if (g_flip == true)  FLIP = SDL_FLIP_HORIZONTAL;

        if (state == "idle") {
            int k = SDL_GetTicks() / 100; setSrc(k % g_idle);
            SDL_RenderCopyEx(renderer, idle, &src, &dest, 0.0, NULL, FLIP);
        }
        if (state == "run") {
            int k = SDL_GetTicks() / 160; setSrc(k % g_run);
            SDL_RenderCopyEx(renderer, run, &src, &dest, 0.0, NULL, FLIP);
        }
        if (state == "attack") {
            int k = SDL_GetTicks() / 155; setSrc(k % g_attack);
            SDL_RenderCopyEx(renderer, attack, &src, &dest, 0.0, NULL, FLIP);
        }
        if (state == "death") {
            int k = SDL_GetTicks() / 160; setSrc(k % g_death);
            SDL_RenderCopyEx(renderer, death, &src, &dest, 0.0, NULL, FLIP);
            if(k >= g_death-1) g_Dead = false;
        }
    }
};
