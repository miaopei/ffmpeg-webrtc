#include <stdio.h>
#include <SDL.h>

#define BLOCK_SIZE 4096000

static Uint8 *audio_buf = NULL;
static Uint8 *audio_pos = NULL;
static size_t buffer_len = 0;

//callback function for audio devcie
void read_audio_data(void *udata, Uint8 *stream, int len){

    if(buffer_len == 0){
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

    FILE *audio_fd = NULL;

    SDL_AudioSpec spec;

    char *path = "./test.pcm";

    //SDL initialize
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)){
        fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
        return ret;
    }

    //open pcm file
    audio_fd = fopen(path, "r");
    if(!audio_fd){
        fprintf(stderr, "Failed to open pcm file!\n");
        goto __FAIL;
    }

    //alloc memory for audio
    audio_buf = (Uint8*)malloc(BLOCK_SIZE);
    if(!audio_buf){
        goto __FAIL;
    }

    //SDL_AudioSpec
    spec.freq = 44100;;
    spec.format = AUDIO_S16SYS;
    spec.channels = 2;
    spec.silence = 0;
    spec.samples = 2048;;
    spec.callback = read_audio_data;;
    spec.userdata = NULL;

    //open audio devcie
    if(SDL_OpenAudio(&spec, NULL)){
        fprintf(stderr, "Failed to open audio device, %s\n", SDL_GetError());
        goto __FAIL;
    }

    //play audio
    SDL_PauseAudio(0);

    do{
        //read data from pcm file
        buffer_len = fread(audio_buf, 1, BLOCK_SIZE, audio_fd);
        fprintf(stderr, "block size is %zu\n", buffer_len);

        audio_pos = audio_buf;

        //the main thread wait for a moment
        while(audio_pos < (audio_buf + buffer_len)) {
            SDL_Delay(1);
        }

    }while(buffer_len !=0);

    //close audio device
    SDL_CloseAudio();

    ret = 0;

__FAIL:
    //release some resources
    if(audio_buf){
        free(audio_buf);
    }

    if(audio_fd){
        fclose(audio_fd);
    }

    //quit SDL
    SDL_Quit();

    return ret;
}


