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
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Quit();
    return 0;
}

