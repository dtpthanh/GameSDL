
#include "Header.h"

bool isRunning = true;
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

// tạo ra texture dựa trên đường dẫn được truyền vào tham số
SDL_Texture* loadTex(const char* path) {
    SDL_Surface *tmpSurface = IMG_Load(path);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);

    SDL_FreeSurface(tmpSurface);
    return tex;
}

void loadTextures();
void init();

// ======================= MAIN CODE GOES HERE =======================
struct Pair {
    double x, y;
    int xx() { return x; }
    int yy() { return y; }
    Pair(double x = 0, double y = 0) : x(x), y(y) {}
};

int offset;
Pair p_pos, g_pos, s_pos;
bool flip, p_Hurt = false;
int p_time;
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
        if (keystate[SDL_SCANCODE_A]) velo.x -= 7;
        if (keystate[SDL_SCANCODE_D]) velo.x += 7;
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

        if (pos.x + offset > 560) {
            offset = 560 - pos.x;
        }

        if (pos.x + offset < 50  && pos.x >= 50) {
            offset = 50 - pos.x;
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

bool g_flip=true;
bool g_Dead = false;
int g_time;
struct Goblin {
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
        if((g_pos.x - p_pos.x <= -5 && g_pos.x - p_pos.x >= -30) && (g_pos.y - p_pos.y >= -50 && g_pos.y - p_pos.y <= 50)) {
            state = "attack";
            g_flip = true;
        }
        else if((g_pos.x - p_pos.x < -30 && g_pos.x - p_pos.x >= -110) && (g_pos.y - p_pos.y >= -50 && g_pos.y - p_pos.y <= 50)) {
            state = "attack";
            g_flip = false;
        }
        else if(g_pos.x - p_pos.x >= -40  ) {
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
            int k = SDL_GetTicks() / 175; setSrc(k % g_attack);
            SDL_RenderCopyEx(renderer, attack, &src, &dest, 0.0, NULL, FLIP);
        }
        if (state == "death") {
            int k = SDL_GetTicks() / 160; setSrc(k % g_death);
            SDL_RenderCopyEx(renderer, death, &src, &dest, 0.0, NULL, FLIP);
            if(k >= g_death-1) g_Dead = false;
        }
    }
};

bool s_flip=false;
bool s_Dead=false;
int s_time;
struct Skeleton {
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
        if((s_pos.x - p_pos.x <= 50 && s_pos.x - p_pos.x >= -30) && (s_pos.y - p_pos.y >= -50 && s_pos.y - p_pos.y <= 50)) {
            state = "attack";
            s_flip = true;
        }
        else if((s_pos.x - p_pos.x < -30 && s_pos.x - p_pos.x >= -120) && (s_pos.y - p_pos.y >= -50 && s_pos.y - p_pos.y <= 50)) {
            state = "attack";
            s_flip = false;
        }
        else if(s_pos.x - p_pos.x >= -50  ) {
            s_flip=true;
            state = "run";
            velo.x -= 3.5;
        }
        else if(s_pos.x - p_pos.x <= -50 )
        {
            s_flip=false;
            state = "run";
            velo.x += 3.5;
        }
        else {
            state = "idle";
        }
        if(s_Dead) {
            state = "death";
            s_time=0;
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


int deadCount;
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





SDL_Texture *bck; // background
SDL_Texture *control;
Player player;
Goblin goblin;
Skeleton skeleton;
HP HP;

// tải các hình vào trong các thành phần của game

void loadTextures() {
    player.init();
    goblin.init();
    skeleton.init();
    bck = loadTex("Background.png");
    control = loadTex("WASD.png");
    HP.init();
}

void init(){
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Astray Girl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1226, 700, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    loadTextures();
}

// update trạng thái khi mà ấn phím hoặc ấn chuột
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

    if(player.state=="fight" && ((g_pos.x - p_pos.x >= -125 && g_pos.x - p_pos.x <=50)
                             && (g_pos.y - p_pos.y >= -50 && g_pos.y - p_pos.y <= 50)) && flip != g_flip) {
        g_Dead = true;
        goblin.state = "death";
        if(goblin.state=="death") g_time+=5;
        if(g_time==50) {
            goblin.state="idle";
            goblin.setPos(player.pos.xx()+900,435);
        }
    }

    if(player.state=="fight" && ((s_pos.x - p_pos.x >= -125 && s_pos.x - p_pos.x <=50)
                             && (s_pos.y - p_pos.y >= -50 && s_pos.y - p_pos.y <= 50)) && flip != s_flip) {
        s_Dead=true;
        skeleton.state = "death";
        if(skeleton.state=="death") s_time+=5;
        if(s_time==50) {
            skeleton.state="idle";
            skeleton.setPos(player.pos.xx()-850,435);
        }
    }

    if( player.state != "fight" && (skeleton.state=="attack" || goblin.state=="attack")
                                && (((g_pos.x - p_pos.x >= -125 && g_pos.x - p_pos.x <=50) && (g_pos.y - p_pos.y >= -50 && g_pos.y - p_pos.y <= 50))
                                || ((s_pos.x - p_pos.x >= -125 && s_pos.x - p_pos.x <=50) && (s_pos.y - p_pos.y >= -50 && s_pos.y - p_pos.y <= 50)))
                                && (goblin.src.x == 900 || skeleton.src.x == 900) ) {
        p_Hurt = true;
        player.state = "hurt";

        if(player.state=="hurt") p_time+=5;
        if(p_time==50) {
            player.state="idle";
            deadCount++;
        }
    }
}

// update trạng thái của game
void update() {
    player.update();
    goblin.update();
    skeleton.update();

    HP.update();
}

void draw_bck() {
    int new_offset = (offset % 1226 + 1226) % 1226;
    SDL_Rect dest = { new_offset, 0, 1226, 700 };
    SDL_RenderCopy(renderer, bck, NULL, &dest);
    SDL_Rect dest2 = { new_offset - 1226, 0, 1226, 700 };
    SDL_RenderCopy(renderer, bck, NULL, &dest2);
    while(frame_time < 10000) {
        SDL_Rect dest_control = {80, 150, 256, 256};
        SDL_RenderCopy(renderer, control, NULL, &dest_control);
    }
}

// vẽ ảnh nền và nhân vật lên màn hình
void render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    draw_bck();
    player.draw();
    skeleton.draw();
    goblin.draw();

    HP.draw();

    SDL_RenderPresent(renderer);
}
void Music() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Chunk* sound = Mix_LoadWAV("Music.wav");
    Mix_PlayChannel(-1, sound, 10);
    while (Mix_Playing(-1)) {
        SDL_Delay(100);
    }
    Mix_FreeChunk(sound);
    Mix_CloseAudio();
}

int main(int argc, char * argv []) {
    init();

    const int FRAME_DELAY = 1000 / 60;
    while (isRunning) {
//        Uint32 frame_start = SDL_GetTicks();
//        Uint32 frame_time;
        handle();
        update();
        render();
        Music();

//        frame_time = SDL_GetTicks() - frame_start;
        if (FRAME_DELAY > frame_time) {
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }

    return 0;
}

