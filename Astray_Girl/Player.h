#pragma once

#include "Header.h"
#include "Character.h"

struct HP{
    Pair pos;
    SDL_Rect src, dest;
    int HP_n;
    SDL_Texture *HP;
    void init(){
        HP_n=8;
        HP = loadTex("HP.png");
        setPos(60,0);
        src.w = 160;
        src.h = 64;
        dest.w = 240;
        dest.h = 96;
    }

    void setPos(int x, int y) {
        pos.x = x; pos.y = y;
    }


    void setSrc() {
        src.x = 0;
        src.y = 64 * deadCount;
    }

    void update(){
        dest.x = pos.xx() ;
        dest.y = pos.yy();
    }
    void draw() {
        setSrc();
        SDL_RenderCopy(renderer, HP, &src, &dest);
    }
};

struct Player {
    Pair pos, velo;
    SDL_Rect src, dest;

    std::string state;
    int idle_n, jump_n, run_n, fight_n, hurt_n;
    SDL_Texture *idle, *jump, *run, *fight, *hurt;

    void init() {
        idle_n = 18;
        idle = loadTex("idle.png");

        jump_n = 19;
        jump = loadTex("jump.png");

        run_n = 24;
        run = loadTex("run.png");

        fight_n = 26;
        fight = loadTex("fight.png");

        hurt_n = 7;
        hurt = loadTex("hurt.png");

        flip = false;
        setPos(500, 480);
        src.w = src.h = 80;
        dest.w = dest.h = 170;
    }

    void setPos(int x, int y) {
        pos.x = x; pos.y = y;
    }
    void setSrc(int k) {
        src.y = 0;
        src.x = 80 * k;
    }

    int lastJump;

    void handle() {
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);

        velo.x = 0;
        if (keystate[SDL_SCANCODE_A]) velo.x -= 5;
        if (keystate[SDL_SCANCODE_D]) velo.x += 5;
        if (velo.xx() > 0) flip = false;
        if (velo.xx() < 0) flip = true;

        if (pos.y != 480) state = "jump";

        else {
            if (keystate[SDL_SCANCODE_W]) {
                velo.y = -19;
                lastJump = SDL_GetTicks();
            }
//            state = velo.xx() == 0 ? "idle" : "run";
            if (velo.xx() != 0) state = "run";
            else state = "idle";
        }

        if(keystate[SDL_SCANCODE_SPACE]) {
            state = "fight";
            velo.x = 0;
        }
        if(p_Hurt==true) {
            state = "hurt";
            p_time = 0;
        }
    }

    void update() {
        p_pos.x = pos.x += velo.x;
        p_pos.y = pos.y += velo.y;

        if (pos.y >= 480) {
            // nếu nhân vật đang ở dưới mặt đất
            pos.y = 480;
            velo.y = 0;
        } else {
            // nếu nhân vật đang ở trên không trung
            velo.y += 1;
        }

        if (pos.x + offset > 860) {
            offset = 860 - pos.x;
        }

        if (pos.x + offset < 40  && pos.x >= 50) {
            offset = 40 - pos.x;
        }

        else if(pos.x < -80) {pos.x = -80; offset = 0;}

        dest.x = pos.xx() + offset;
        dest.y = pos.yy();
        // cập nhật tọa độ của nhân vật được vẽ trên màn hình
    }

    void draw() {
        SDL_RendererFlip FLIP;
        if (flip == false) FLIP = SDL_FLIP_NONE;
        if (flip == true)  FLIP = SDL_FLIP_HORIZONTAL;

        if (state == "idle") {
            int k = SDL_GetTicks() / 60; setSrc(k % idle_n);
            SDL_RenderCopyEx(renderer, idle, &src, &dest, 0.0, NULL, FLIP);
        }
        if (state == "jump") {
            int k = (SDL_GetTicks() - lastJump) / 60; setSrc(k % jump_n);
            SDL_RenderCopyEx(renderer, jump, &src, &dest, 0.0, NULL, FLIP);
        }
        if (state == "run") {
            int k = SDL_GetTicks() / 30; setSrc(k % run_n);
            SDL_RenderCopyEx(renderer, run, &src, &dest, 0.0, NULL, FLIP);
        }
        if (state == "fight") {
            int k = SDL_GetTicks() / 40; setSrc(k % fight_n);
            SDL_RenderCopyEx(renderer, fight, &src, &dest, 0.0, NULL, FLIP);
        }
        if (state == "hurt") {
            int k = SDL_GetTicks() /80; setSrc(k % hurt_n);
            SDL_RenderCopyEx(renderer, hurt, &src, &dest, 0.0, NULL, FLIP);
            if(k >= hurt_n - 1) p_Hurt = false;
        }
    }
};

