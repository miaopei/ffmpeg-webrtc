#! /bin/sh
gcc simplest_ffmpeg_player.cpp -g -o simplest_ffmpeg_player.out \
-I ./include -L ./lib -lSDL2main -lSDL2 -lavformat -lavcodec -lavutil -lswscale
