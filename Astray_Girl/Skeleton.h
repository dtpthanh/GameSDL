#pragma once

#include "Header.h"
#include "Character.h"

struct Skeleton{
    Pair pos, velo;
    SDL_Rect src, dest;

    std::string state;
    int s_idle, s_run, s_attack, s_death;
    SDL_Texture *idle, *run, *attack, *death;

    void init(){
        s_idle = 4;
        idle = loadTex("Skeleton_Idle.png");

        s_run = 4;
        run = loadTex("Skeleton_Run.png");

        s_attack = 8;
        attack = loadTex("Skeleton_Attack.png");

        s_death = 4;
        death = loadTex("Skeleton_Death.png");

        s_flip = false;
        setPos(-50, 435);
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
        if(s_pos.x - p_pos.x <= 0 && s_pos.x - p_pos.x >= -10) {
            state = "attack";
            s_flip = true;
        }
        else if(s_pos.x - p_pos.x < -10 && s_pos.x - p_pos.x >= -120) {
            state = "attack";
            s_flip = false;
        }
        else if(s_pos.x - p_pos.x >= -50  ) {
            s_flip=true;
            state = "run";
            velo.x -= 4.6;
        }
        else if(s_pos.x - p_pos.x <= -50 )
        {
            s_flip=false;
            state = "run";
            velo.x += 4.6;
        }
        else {
            state = "idle";
        }
        if(s_Dead) {
            state = "death";
            s_time = 0;
        }
    }
    void update() {
        s_pos.x = pos.x += velo.x;
        s_pos.y = pos.y += velo.y;
        dest.x = pos.xx() + offset;
        dest.y = pos.yy();

        if(pos.y > 435) velo.y = 5;
    }

    void draw() {
        SDL_RendererFlip FLIP;
        if (s_flip == false) FLIP = SDL_FLIP_NONE;
        if (s_flip == true)  FLIP = SDL_FLIP_HORIZONTAL;

        if (state == "idle") {
            int k = SDL_GetTicks() / 100; setSrc(k % s_idle);
            SDL_RenderCopyEx(renderer, idle, &src, &dest, 0.0, NULL, FLIP);
        }
        if (state == "run") {
            int k = SDL_GetTicks() / 160; setSrc(k % s_run);
            SDL_RenderCopyEx(renderer, run, &src, &dest, 0.0, NULL, FLIP);
        }
        if (state == "attack") {
            int k = SDL_GetTicks() / 175; setSrc(k % s_attack);
            SDL_RenderCopyEx(renderer, attack, &src, &dest, 0.0, NULL, FLIP);
        }
        if (state == "death") {
            int k = SDL_GetTicks() / 160; setSrc(k % s_death);
            SDL_RenderCopyEx(renderer, death, &src, &dest, 0.0, NULL, FLIP);
            if(k >= s_death-1) s_Dead = false;
        }
    }
};

