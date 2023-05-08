#include "Header.h"
#include "Character.h"
#include "Player.h"
#include "Goblin.h"
#include "Skeleton.h"
#include "DrawText.h"
#include "GameOver.h"

Player player;
Goblin goblin;
Skeleton skeleton;
HP hp;
Button button1;


void loadTextures() {
    player.init();
    goblin.init();
    skeleton.init();
    hp.init();
    button1.LoadTextures(renderer, "Button1.png", "Button2.png" );
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

    if( ((player.state != "fight") || (player.state == "fight" && (flip != g_flip || flip != s_flip))) && (skeleton.state=="attack" || goblin.state=="attack")
                                && (((g_pos.x - p_pos.x >= -125 && g_pos.x - p_pos.x <=60) && (g_pos.y - p_pos.y >= -50 && g_pos.y - p_pos.y <= 50))
                                || ((s_pos.x - p_pos.x >= -125 && s_pos.x - p_pos.x <=50) && (s_pos.y - p_pos.y >= -50 && s_pos.y - p_pos.y <= 50)))
                                && (goblin.src.x == 750 || skeleton.src.x == 1200) ) {
        p_Hurt = true;
        player.state = "hurt";

        if(player.state=="hurt") p_time+=5;
        if(p_time==45) {
            player.state="idle";
            deadCount++;
            p_time = 0;
        }
    }
}

void update() {
    player.update();
    goblin.update();
    skeleton.update();
    hp.update();
    button1.SetPosition(500, 420, 220, 100);
    button1.HandleEvent(event);
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
    if(deadCount<7){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    draw_bck();
    player.draw();
    skeleton.draw();
    goblin.draw();
    hp.draw();
    drawText(renderer, "Kills: ", 25, 1000, 20, 255, 255, 255, 0, 0, 0);
    drawNumber(renderer, kills, 25, 1100, 20, 255, 255, 255, 0, 0, 0);
    SDL_RenderPresent(renderer);
    }
    //hien ket qua khi nhan vat het 7 mang
    if(deadCount>=7) {
        player.setPos(500, 480);    //dat lai vi tri nhan vat
        goblin.setPos(player.pos.xx()+900,435);     //dat lai vi tri goblin
        skeleton.setPos(player.pos.xx()-850,435);   //dat lai vi tri skeleton
        SDL_RenderClear(renderer);
        draw_bck();
        showResults();  //hien thi ket qua
        button1.Render(renderer);
        SDL_RenderPresent(renderer);
    }
}

//ham main
int main(int argc, char * argv []) {
    init();

    const int FRAME_DELAY = 1000 / 60;
    while (isRunning) {
        Uint32 frame_start = SDL_GetTicks();

        handle();
        update();
        render();

        int frame_time = SDL_GetTicks() - frame_start;
        if (FRAME_DELAY > frame_time) {
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }
    return 0;
}
