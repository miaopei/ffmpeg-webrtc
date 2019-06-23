#! /bin/sh
gcc simplest_ffmpeg_player_su.cpp -g -o simplest_ffmpeg_player_su.out -I ./include -L ./lib \
-lSDL2main -lSDL2 -lavformat -lavcodec -lavutil -lswscale
