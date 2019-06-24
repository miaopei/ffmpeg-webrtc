#! /bin/sh
gcc main.cpp -g -o main.out -I ./include -L ./lib \
-lSDL2main -lSDL2 -lavformat -lavcodec -lavutil -lswscale
