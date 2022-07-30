#include "SDL.h"
#include "SDL_image.h"
#include <math.h>

#define WindowWidth 800
#define WindowHeight 600

#define TriangleWidth 25
#define TriangleHeight 25

void EventHandle(SDL_Event*);
void ClearScreen(SDL_Renderer*);
void Present(SDL_Renderer*);

void LogicUpdate(SDL_Window*);
void Render(SDL_Renderer*);

bool isquit = false;
SDL_Rect playerRect;

int speed;

int timer = 0;

int WindowCurrentWidth = WindowWidth;
int WindowCurrentHeight = WindowHeight;

bool isGameOver = false;

SDL_Texture* PlayerTexture;
SDL_Texture* SpikeTexture;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("sdl init failed");
        return 1;
    }
    IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);

    SDL_Window* window = SDL_CreateWindow("shaped",
                                           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           WindowWidth, WindowHeight,
                                           SDL_WINDOW_SHOWN|SDL_WINDOW_ALWAYS_ON_TOP|SDL_WINDOW_RESIZABLE);
    SDL_SetWindowMinimumSize(window, 50, 50);
    SDL_SetWindowMaximumSize(window, 1000, 1000);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    PlayerTexture = IMG_LoadTexture(renderer, "resources/player.png");
    SpikeTexture = IMG_LoadTexture(renderer, "resources/Spike.png");
    if (!PlayerTexture || !SpikeTexture) {
        SDL_Log("load image failed");
    }

    SDL_Event event;

    playerRect.w = 32;
    playerRect.h = 32;
    playerRect.x = (WindowWidth - playerRect.w) / 2;
    playerRect.y = (WindowHeight - playerRect.h) / 2;

    while (!isquit) {
        EventHandle(&event);
        LogicUpdate(window);
        Render(renderer);
        SDL_Delay(30);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_DestroyTexture(PlayerTexture);
    SDL_DestroyTexture(SpikeTexture);
    IMG_Quit();
    SDL_Quit();
    return 0;
}

void EventHandle(SDL_Event* event) {
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            isquit = true;
        }
    }
}

/******************/
/*  Logic Update  */
/******************/

bool IsPlayerDead() {
    return playerRect.w + playerRect.x >= WindowCurrentWidth - TriangleHeight ||
           playerRect.h + playerRect.y >= WindowCurrentHeight - TriangleHeight ||
           playerRect.x <= TriangleHeight || playerRect.y <= TriangleHeight;
}

void UpdateGameScene(SDL_Window* window) {
    SDL_GetWindowSize(window, &WindowCurrentWidth, &WindowCurrentHeight);

    int halfSpd = ceil(speed / 2.0);
    WindowCurrentHeight -= speed;
    WindowCurrentWidth -= speed;

    SDL_Point pos;
    SDL_GetWindowPosition(window, &pos.x, &pos.y);
    SDL_SetWindowPosition(window, pos.x + halfSpd, pos.y + halfSpd);

    SDL_SetWindowSize(window, WindowCurrentWidth, WindowCurrentHeight);

    playerRect.x -= halfSpd;
    playerRect.y -= halfSpd;
}

void UpdateHardLevelByTime(int time) {
    if (timer >= 0 && timer <= 1000) {
        speed = 2;
    } else {
        speed = 3;
    }
}

void IncTime(int* time) {
    *time += 1;
}

void DoGameOver(SDL_Window* window) {
    isGameOver = true;

    SDL_SetWindowAlwaysOnTop(window, SDL_FALSE);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "游戏结束", "玩家已经死亡", NULL);
    SDL_SetWindowResizable(window, SDL_FALSE);
}

void LogicUpdate(SDL_Window* window) {
    if (isGameOver) {
        return;
    }

    IncTime(&timer);
    UpdateHardLevelByTime(timer);
    UpdateGameScene(window);

    if (IsPlayerDead()) {
        DoGameOver(window);
    }
}


/***********************/
/*     Render Part     */
/***********************/

void DrawPlayer(SDL_Renderer* renderer, SDL_Rect* playerRect) {
    SDL_RenderCopy(renderer, PlayerTexture, NULL, playerRect);
}

void DrawSpine(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    int num = (int)((WindowCurrentWidth / (float)TriangleWidth) + 0.5);
    for (int i = 0; i < num; i++) {
        SDL_Rect dstrect = {TriangleWidth * i, 0, TriangleWidth, TriangleWidth};
        SDL_RenderCopy(renderer, SpikeTexture, NULL, &dstrect);

        dstrect.y = WindowCurrentHeight - TriangleHeight;
        SDL_RenderCopyEx(renderer, SpikeTexture, NULL, &dstrect, 0, NULL, SDL_FLIP_VERTICAL);
    }

    num = (int)((WindowCurrentHeight / (float)TriangleWidth) + 0.5);
    for (int i = 0; i < num; i++) {
        SDL_Rect dstrect = {0, TriangleWidth * i, TriangleWidth, TriangleWidth};
        SDL_Point p{0, 0};
        SDL_RenderCopyEx(renderer, SpikeTexture, NULL, &dstrect, -90, &p, SDL_FLIP_NONE);

        dstrect.x = WindowCurrentWidth;
        SDL_RenderCopyEx(renderer, SpikeTexture, NULL, &dstrect, 90, &p, SDL_FLIP_NONE);
    }
}

void ClearScreen(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Present(SDL_Renderer* renderer) {
    SDL_RenderPresent(renderer);
}

void Render(SDL_Renderer* renderer) {
    ClearScreen(renderer);
    if (!isGameOver) {
        DrawPlayer(renderer, &playerRect);
    }
    DrawSpine(renderer);
    Present(renderer);
}
