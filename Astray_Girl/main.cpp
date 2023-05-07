#include "Header.h"
#include "Character.h"
#include "Player.h"
#include "Goblin.h"
#include "Skeleton.h"
#include "DrawText.h"

Player player;
Goblin goblin;
Skeleton skeleton;
HP hp;

void loadTextures() {
    player.init();
    goblin.init();
    skeleton.init();
    hp.init();
    bck = loadTex("Background.png");
}

void init(){
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Astray Girl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1226, 700, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    loadTextures();
    TTF_Init();
}

void handle() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
    }

    player.handle();
    goblin.handle();
    skeleton.handle();
    if(player.state=="fight" && ((((g_pos.x - p_pos.x >= -125 && g_pos.x - p_pos.x <=-80) || (g_pos.x - p_pos.x >= 10 && g_pos.x - p_pos.x <=50)) && flip != g_flip) || (g_pos.x - p_pos.x >= -80 && g_pos.x - p_pos.x <= 10))
                             && (g_pos.y - p_pos.y >= -50 && g_pos.y - p_pos.y <= 50) ) {
        g_Dead = true;
        goblin.state = "death";
        if(goblin.state=="death") g_time+=5;
        if(g_time==50) {
            goblin.state="idle";
            goblin.setPos(player.pos.xx()+900,435);
            kills++;
        }
    }

    if(player.state=="fight" && ((((s_pos.x - p_pos.x >= -110 && s_pos.x - p_pos.x <=-80) || (s_pos.x - p_pos.x >= 10 && s_pos.x - p_pos.x <=50)) && flip != s_flip) || (s_pos.x - p_pos.x >= -80 && s_pos.x - p_pos.x <= 10))
                             && (s_pos.y - p_pos.y >= -50 && s_pos.y - p_pos.y <= 50) ) {
        s_Dead=true;
        skeleton.state = "death";
        if(skeleton.state=="death") s_time+=5;
        if(s_time==50) {
            skeleton.state="idle";
            skeleton.setPos(player.pos.xx()-850,435);
            kills++;
        }
    }

    if( player.state != "fight" && (skeleton.state=="attack" || goblin.state=="attack")
                                && (((g_pos.x - p_pos.x >= -125 && g_pos.x - p_pos.x <=50) && (g_pos.y - p_pos.y >= -50 && g_pos.y - p_pos.y <= 50))
                                || ((s_pos.x - p_pos.x >= -125 && s_pos.x - p_pos.x <=50) && (s_pos.y - p_pos.y >= -50 && s_pos.y - p_pos.y <= 50)))
                                && (goblin.src.x == 750 || skeleton.src.x == 750) ) {
        p_Hurt = true;
        player.state = "hurt";

        if(player.state=="hurt") p_time+=5;
        if(p_time==50) {
            player.state="idle";
            deadCount++;
        }
    }
}

void update() {
    player.update();
    goblin.update();
    skeleton.update();
    hp.update();
}

//ve background
void draw_bck() {
    int new_offset = (offset % 1226 + 1226) % 1226;
    SDL_Rect dest = { new_offset, 0, 1226, 700 };
    SDL_RenderCopy(renderer, bck, NULL, &dest);
    SDL_Rect dest2 = { new_offset - 1226, 0, 1226, 700 };
    SDL_RenderCopy(renderer, bck, NULL, &dest2);
}

void render() {
    if(deadCount<=7){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    draw_bck();
    player.draw();
    skeleton.draw();
    goblin.draw();
    hp.draw();
    drawText(renderer, "Kills: ", 25, 1000, 20, 255, 255, 255, 0, 0, 0);
    drawNumber(renderer, kills, 25, 1100, 20, 255, 255, 255, NULL, NULL, NULL);
    SDL_RenderPresent(renderer);
    }
    //hien ket qua khi nhan vat het 7 mang
    if(deadCount>7) {
        SDL_RenderClear(renderer);
        draw_bck();
        SDL_Texture* menu = loadTex("menu.png");
        SDL_Rect rect = {450, 100, 318, 450};
        SDL_RenderCopy(renderer, menu, NULL, &rect);
        SDL_RenderPresent(renderer);
    }
}


//void Music() {
//    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
//    Mix_Chunk* sound = Mix_LoadWAV("Music.wav");
//    Mix_PlayChannel(-1, sound, 10);
//    while (Mix_Playing(-1)) {
//        SDL_Delay(100);
//    }
//    Mix_FreeChunk(sound);
//    Mix_CloseAudio();
//}


int main(int argc, char * argv []) {
    init();

    const int FRAME_DELAY = 1000 / 60;
    while (isRunning) {
        Uint32 frame_start = SDL_GetTicks();

        handle();
        update();
        render();

//        Music();

        int frame_time = SDL_GetTicks() - frame_start;
        if (FRAME_DELAY > frame_time) {
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }
    return 0;
}
