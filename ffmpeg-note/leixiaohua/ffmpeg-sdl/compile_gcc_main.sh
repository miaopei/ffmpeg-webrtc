#! /bin/sh
gcc main.c -g -o main.out -I ./include -L ./lib \
-lSDL2main -lSDL2 -lavformat -lavcodec -lavutil -lswscale
