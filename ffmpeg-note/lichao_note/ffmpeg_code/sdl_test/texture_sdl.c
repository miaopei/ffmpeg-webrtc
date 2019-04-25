/*************************************************************************
	> File Name: first_sdl.c
	> Author: Mr.Miaow
	> Mail: miaopei163@163.com 
	> Created Time: 一  4/22 14:19:45 2019
 ************************************************************************/

#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    int quit = 1;

    SDL_Window *window = NULL;
    SDL_Renderer *render = NULL;
    SDL_Event event;
    SDL_Texture *texture = NULL;

    SDL_Rect rect;
    rect.w = 30;
    rect.h = 30;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Log("Hello SDL2!\n");

    window = SDL_CreateWindow("SDL2 Window", 
                     200, 400,
                     640,
                     480,
                     SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Failed to Create window!\n");
        goto __EXIT;
    }

    render = SDL_CreateRenderer(window, -1, 0);
    if (!render) {
        SDL_Log("Failed to Create Render!\n");
        goto __DWINDOW;
    }
#if 0
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);

    SDL_RenderClear(render);

    SDL_RenderPresent(render);
#endif
    texture = SDL_CreateTexture(render, 
                      SDL_PIXELFORMAT_BGR888,
                      SDL_TEXTUREACCESS_TARGET,
                      640, 480);
    if (!texture) {
        SDL_Log("Failed to Create Texture!\n");
        goto __RENDER;
    }

    do {
        SDL_PollEvent(&event);     
        switch(event.type) {
        case SDL_QUIT:
            quit = 0;
            break;
        default:
            SDL_Log("Event type is %d", event.type);
        }

        rect.x = rand() % 600;
        rect.y = rand() % 480;

        SDL_SetRenderTarget(render, texture);
        SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
        SDL_RenderClear(render);

        SDL_RenderDrawRect(render, &rect);
        SDL_SetRenderDrawColor(render, 255, 0, 0, 0);
        SDL_RenderFillRect(render, &rect);

        SDL_SetRenderTarget(render, NULL);
        SDL_RenderCopy(render, texture, NULL, NULL);

        SDL_RenderPresent(render);

    } while(quit);

    SDL_DestroyTexture(texture);
    
__RENDER:
    SDL_DestroyRenderer(render);
    
__DWINDOW:
    SDL_DestroyWindow(window);

__EXIT:
    SDL_Quit();

    return 0;
}

