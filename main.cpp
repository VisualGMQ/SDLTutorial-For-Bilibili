#include "SDL.h"
#include "SDL_image.h"

#define WindowWidth 800
#define WindowHeight 600

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("sdl init failed");
        return 1;
    }
    IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);

    SDL_Window* window = SDL_CreateWindow("Hello SDL2",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WindowWidth, WindowHeight,
                                          SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Texture* texture = IMG_LoadTexture(renderer, "resources/1.png");
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    bool isquit = false;
    SDL_Event event;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    while (!isquit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isquit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_Rect dstrect;
        dstrect.x = 100;
        dstrect.y = 100;
        dstrect.w = w;
        dstrect.h = h;

        SDL_RenderCopy(renderer, texture, NULL, &dstrect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    /*
     * 1. Renderer <- Create <- SDL_SetRenderDrawColor <- SDL_RenderClear <- SDL_RenderDrawXXX Line Rect Pixel <- SDL_RenderCop[Ex] <- SDL_RenderPresent
     * 2. Texture <- SDL_CreateTextureFromSurface <- SDL_CreateTexture SDL_UpdateTexture <- SDL_QueryTexture <- SDL_SetTextureBlendMod <- SDL_SetTextureDrawColor
     * 3. Surface <- SDL_CreateRGBSurfaceWithFormat <- Point - Surface
     * 4. SDL_image
     */

    IMG_Quit();
    SDL_Quit();
    return 0;
}
