/*************************************************************************
	> File Name: pcm_player.c
	> Author: Mr.Miaow
	> Mail: miaopei163@163.com 
	> Created Time: 2019年04月22日 星期一 18时59分01秒
 ************************************************************************/

#include <stdio.h>
#include <SDL2/SDL.h>

#define BLOCK_SIZE 4096000 // 4M

static size_t buffer_len = 0;
static Uint8 *audio_buf = NULL;
static Uint8 *audio_pos = NULL;

void read_audio_data(void *udata, Uint8 *stream, int len)
{
    if (buffer_len == 0) {
        return;
    }

    SDL_memset(stream, 0, len);

    len = (len < buffer_len) ? len : buffer_len;
    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);

    audio_pos += len;
    buffer_len -= len;
}

int main(int argc, char *argv[])
{
    int ret = -1;
    char *path = "./1.pcm";
    FILE *audio_fd = NULL;

    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        SDL_Log("Failed to initial!\n");
        return ret;
    }

    audio_fd = fopen(path, "r");
    if (!audio_fd) {
        SDL_Log("Failed to open pcm file!\n");
        goto __FAIL;
    }

    audio_buf = (Uint8*)malloc(BLOCK_SIZE);
    if (!audio_buf) {
        SDL_Log("Failed to alloc memory!\n");
        goto __FAIL;
    }

    SDL_AudioSpec spec;
    spec.freq = 44100;
    spec.channels = 2;
    spec.format = AUDIO_S16SYS;
    spec.silence = 0;
    spec.callback = read_audio_data;
    spec.userdata = NULL;
    
    if (SDL_OpenAudio(&spec, NULL)) {
        SDL_Log("Failed to open audio device!\n");
        goto __FAIL;
    }

    SDL_PauseAudio(0);

    do {
        buffer_len = fread(audio_buf, 1, BLOCK_SIZE, audio_fd);
        audio_pos = audio_buf;
        while (audio_pos < (audio_buf + buffer_len)) {
            SDL_Delay(1);
        }
    } while(buffer_len != 0);

    SDL_CloseAudio();

    ret = 0;

__FAIL:
    if (audio_buf) {
        free(audio_buf);
    }

    if (audio_fd) {
        fclose(audio_fd);
    }

    SDL_Quit();

    return ret;
}

