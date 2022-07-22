#include "SDL.h"

bool isquit = false;
SDL_Rect playerRect;

void EventHandle(SDL_Event*);
void ClearScreen(SDL_Renderer*);
void Present(SDL_Renderer*);

void LogicUpdate(SDL_Window*);
void Render(SDL_Renderer*);

#define WindowWidth 500
#define WindowHeight 400

#define TriangleWidth 25
#define TriangleHeight 25

int speed = 1;

int timer = 0;

int WindowCurrentWidth = WindowWidth;
int WindowCurrentHeight = WindowHeight;

bool isGameOver = false;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("sdl init failed");
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("shaped",
                                           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           WindowWidth, WindowHeight,
                                           SDL_WINDOW_SHOWN|SDL_WINDOW_ALWAYS_ON_TOP|SDL_WINDOW_RESIZABLE);
    SDL_SetWindowMinimumSize(window, 50, 50);
    SDL_SetWindowMaximumSize(window, 1000, 1000);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Event event;

    playerRect.w = 50;
    playerRect.h = 50;
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

void ClearScreen(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Present(SDL_Renderer* renderer) {
    SDL_RenderPresent(renderer);
}

void LogicUpdate(SDL_Window* window) {
    if (isGameOver) {
        return;
    }

    timer ++;

    SDL_GetWindowSize(window, &WindowCurrentWidth, &WindowCurrentHeight);
    if (timer >= 0 && timer <= 1000) {
        speed = 1;
    } else {
        speed = 3;
    }
    WindowCurrentHeight -= speed;
    WindowCurrentWidth -= speed;

    SDL_SetWindowSize(window, WindowCurrentWidth, WindowCurrentHeight);

    if (playerRect.w + playerRect.x >= WindowCurrentWidth - TriangleHeight ||
        playerRect.h + playerRect.y >= WindowCurrentHeight - TriangleHeight) {
        isGameOver = true;
        SDL_SetWindowAlwaysOnTop(window, SDL_FALSE);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "游戏结束", "玩家已经死亡", NULL);
        SDL_SetWindowResizable(window, SDL_FALSE);
    }
}

void Render(SDL_Renderer* renderer) {
    ClearScreen(renderer);

    if (!isGameOver) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawRect(renderer, &playerRect);
    }

    int num = (int)((WindowCurrentWidth / (float)TriangleWidth) + 0.5);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (int i = 0; i < num; i++) {
        SDL_RenderDrawLine(renderer, i * TriangleWidth, 0, i * TriangleWidth + (0.5 * TriangleWidth), TriangleHeight);
        SDL_RenderDrawLine(renderer, i * TriangleWidth + (0.5 * TriangleWidth), TriangleHeight, (i + 1) * TriangleWidth, 0);

        SDL_RenderDrawLine(renderer, i * TriangleWidth, WindowCurrentHeight, i * TriangleWidth + (0.5 * TriangleWidth), WindowCurrentHeight - TriangleHeight);
        SDL_RenderDrawLine(renderer, i * TriangleWidth + (0.5 * TriangleWidth), WindowCurrentHeight - TriangleHeight, (i + 1) * TriangleWidth, WindowCurrentHeight);
    }

    num = (int)((WindowCurrentHeight / (float)TriangleWidth) + 0.5);
    for (int i = 0; i < num; i++) {
        SDL_RenderDrawLine(renderer, 0, i * TriangleWidth, TriangleHeight, (i + 0.5) * TriangleWidth);
        SDL_RenderDrawLine(renderer, TriangleHeight, (i + 0.5) * TriangleWidth, 0, (i + 1) * TriangleWidth);

        SDL_RenderDrawLine(renderer, WindowCurrentWidth, i * TriangleWidth, WindowCurrentWidth - TriangleHeight, (i + 0.5) * TriangleWidth);
        SDL_RenderDrawLine(renderer, WindowCurrentWidth - TriangleHeight, (i + 0.5) * TriangleWidth, WindowCurrentWidth, (i + 1) * TriangleWidth);
    }

    Present(renderer);
}