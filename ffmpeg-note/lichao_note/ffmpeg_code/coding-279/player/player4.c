// tutorial04.c
// A pedagogical video player that will stream through every video frame as fast as it can,
// and play audio (out of sync).
//
// Code based on FFplay, Copyright (c) 2003 Fabrice Bellard, 
// and a tutorial by Martin Bohme (boehme@inb.uni-luebeckREMOVETHIS.de)
// Tested on Gentoo, CVS version 5/01/07 compiled with GCC 4.1.1
// With updates from https://github.com/chelyaev/ffmpeg-tutorial
// Updates tested on:
// LAVC 54.59.100, LAVF 54.29.104, LSWS 2.1.101, SDL 1.2.15
// on GCC 4.7.2 in Debian February 2015
// Use
//
// gcc -o tutorial04 tutorial04.c -lavformat -lavcodec -lswscale -lz -lm `sdl-config --cflags --libs`
// to build (assuming libavformat and libavcodec are correctly installed, 
// and assuming you have sdl-config. Please refer to SDL docs for your installation.)
//
// Run using
// tutorial04 myvideofile.mpg
//
// to play the video stream on your screen.

#include <stdio.h>
#include <assert.h>
#include <math.h>

#include <SDL.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

// compatibility with newer API
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif

#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIO_FRAME_SIZE 192000

#define MAX_AUDIOQ_SIZE (5 * 16 * 1024)
#define MAX_VIDEOQ_SIZE (5 * 256 * 1024)

#define FF_REFRESH_EVENT (SDL_USEREVENT)
#define FF_QUIT_EVENT (SDL_USEREVENT + 1)

#define VIDEO_PICTURE_QUEUE_SIZE 1

typedef struct PacketQueue {
  AVPacketList *first_pkt, *last_pkt;
  int nb_packets;
  int size;
  SDL_mutex *mutex;
  SDL_cond *cond;
} PacketQueue;


typedef struct VideoPicture {
  AVPicture *pict;
  int width, height; /* source height & width */
  int allocated;
} VideoPicture;

typedef struct VideoState {

  //for multi-media file
  char            filename[1024];
  AVFormatContext *pFormatCtx;

  int             videoStream, audioStream;

  //for audio
  AVStream        *audio_st;
  AVCodecContext  *audio_ctx;
  PacketQueue     audioq;
  uint8_t         audio_buf[(MAX_AUDIO_FRAME_SIZE * 3) / 2];
  unsigned int    audio_buf_size;
  unsigned int    audio_buf_index;
  AVFrame         audio_frame;
  AVPacket        audio_pkt;
  uint8_t         *audio_pkt_data;
  int             audio_pkt_size;
  struct SwrContext *audio_swr_ctx;

  //for video
  AVStream        *video_st;
  AVCodecContext  *video_ctx;
  PacketQueue     videoq;
  struct SwsContext *sws_ctx;

  VideoPicture    pictq[VIDEO_PICTURE_QUEUE_SIZE];
  int             pictq_size, pictq_rindex, pictq_windex;

  //for thread
  SDL_mutex       *pictq_mutex;
  SDL_cond        *pictq_cond;
  
  SDL_Thread      *parse_tid;
  SDL_Thread      *video_tid;

  int             quit;

} VideoState;

SDL_mutex       *texture_mutex;
SDL_Window      *win;
SDL_Renderer    *renderer;
SDL_Texture     *texture;

FILE            *audiofd = NULL;
FILE            *audiofd1 = NULL;

/* Since we only have one decoding thread, the Big Struct
   can be global in case we need it. */
VideoState *global_video_state;

void packet_queue_init(PacketQueue *q) {
  memset(q, 0, sizeof(PacketQueue));
  q->mutex = SDL_CreateMutex();
  q->cond = SDL_CreateCond();
}

int packet_queue_put(PacketQueue *q, AVPacket *pkt) {

  AVPacketList *pkt1;
  if(av_dup_packet(pkt) < 0) {
    return -1;
  }
  pkt1 = av_malloc(sizeof(AVPacketList));
  if (!pkt1)
    return -1;
  pkt1->pkt = *pkt;
  pkt1->next = NULL;
  
  SDL_LockMutex(q->mutex);

  if (!q->last_pkt)
    q->first_pkt = pkt1;
  else
    q->last_pkt->next = pkt1;
  q->last_pkt = pkt1;
  q->nb_packets++;
  q->size += pkt1->pkt.size;
  //fprintf(stderr, "enqueue, packets:%d, send cond signal\n", q->nb_packets);
  SDL_CondSignal(q->cond);
  
  SDL_UnlockMutex(q->mutex);
  return 0;
}

int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block)
{
  AVPacketList *pkt1;
  int ret;

  SDL_LockMutex(q->mutex);
  
  for(;;) {
    
    if(global_video_state->quit) {
      fprintf(stderr, "quit from queue_get\n");
      ret = -1;
      break;
    }

    pkt1 = q->first_pkt;
    if (pkt1) {
      q->first_pkt = pkt1->next;
      if (!q->first_pkt)
	q->last_pkt = NULL;
      q->nb_packets--;
      q->size -= pkt1->pkt.size;
      *pkt = pkt1->pkt;
      av_free(pkt1);
      ret = 1;
      break;
    } else if (!block) {
      ret = 0;
      break;
    } else {
      fprintf(stderr, "queue is empty, so wait a moment and wait a cond signal\n");
      SDL_CondWait(q->cond, q->mutex);
    }
  }
  SDL_UnlockMutex(q->mutex);
  return ret;
}

int audio_decode_frame(VideoState *is, uint8_t *audio_buf, int buf_size) {

  int len1, data_size = 0;
  AVPacket *pkt = &is->audio_pkt;

  for(;;) {
    while(is->audio_pkt_size > 0) {

      int got_frame = 0;
      len1 = avcodec_decode_audio4(is->audio_ctx, &is->audio_frame, &got_frame, pkt);
      if(len1 < 0) {
	/* if error, skip frame */
	fprintf(stderr, "Failed to decode audio ......\n");
	is->audio_pkt_size = 0;
	break;
      }

      data_size = 0;
      if(got_frame) {
	/*
	fprintf(stderr, "auido: channels:%d, nb_samples:%d, sample_fmt:%d\n",
			is->audio_ctx->channels,
			is->audio_frame.nb_samples,
			is->audio_ctx->sample_fmt);

	data_size = av_samples_get_buffer_size(NULL, 
					       is->audio_ctx->channels,
					       is->audio_frame.nb_samples,
					       is->audio_ctx->sample_fmt,
					       1);
	*/
	data_size = 2 * is->audio_frame.nb_samples * 2;
	assert(data_size <= buf_size);
	//memcpy(audio_buf, is->audio_frame.data[0], data_size);
	
	swr_convert(is->audio_swr_ctx,
                        &audio_buf,
                        MAX_AUDIO_FRAME_SIZE*3/2,
                        (const uint8_t **)is->audio_frame.data,
                        is->audio_frame.nb_samples);
	
	
	fwrite(audio_buf, 1, data_size, audiofd);
        fflush(audiofd);
      }

      is->audio_pkt_data += len1;
      is->audio_pkt_size -= len1;
      if(data_size <= 0) {
	/* No data yet, get more frames */
	continue;
      }
      /* We have data, return it and come back for more later */
      return data_size;
    }

    if(pkt->data)
      av_free_packet(pkt);

    if(is->quit) {
      fprintf(stderr, "will quit program......\n");
      return -1;
    }

    /* next packet */
    if(packet_queue_get(&is->audioq, pkt, 1) < 0) {
      return -1;
    }

    is->audio_pkt_data = pkt->data;
    is->audio_pkt_size = pkt->size;
  }
}

void audio_callback(void *userdata, Uint8 *stream, int len) {

  VideoState *is = (VideoState *)userdata;
  int len1, audio_size;

  SDL_memset(stream, 0, len);

  while(len > 0) {
    if(is->audio_buf_index >= is->audio_buf_size) {
      /* We have already sent all our data; get more */
      audio_size = audio_decode_frame(is, is->audio_buf, sizeof(is->audio_buf));
      if(audio_size < 0) {
	/* If error, output silence */
	is->audio_buf_size = 1024*2*2;
	memset(is->audio_buf, 0, is->audio_buf_size);
      } else {
	is->audio_buf_size = audio_size;
      }
      is->audio_buf_index = 0;
    }
    len1 = is->audio_buf_size - is->audio_buf_index;
    fprintf(stderr, "stream addr:%p, audio_buf_index:%d, len1:%d, len:%d\n",
		    stream,
	  	    is->audio_buf_index, 
		    len1, 
		    len);
    if(len1 > len)
      len1 = len;
    //memcpy(stream, (uint8_t *)is->audio_buf + is->audio_buf_index, len1);
    fwrite(is->audio_buf, 1, len1, audiofd1);
    fflush(audiofd1);
    SDL_MixAudio(stream,(uint8_t *)is->audio_buf, len1, SDL_MIX_MAXVOLUME);
    len -= len1;
    stream += len1;
    is->audio_buf_index += len1;
  }
}

static Uint32 sdl_refresh_timer_cb(Uint32 interval, void *opaque) {
  SDL_Event event;
  event.type = FF_REFRESH_EVENT;
  event.user.data1 = opaque;
  SDL_PushEvent(&event);
  return 0; /* 0 means stop timer */
}

/* schedule a video refresh in 'delay' ms */
static void schedule_refresh(VideoState *is, int delay) {
  SDL_AddTimer(delay, sdl_refresh_timer_cb, is);
}

void video_display(VideoState *is) {

  SDL_Rect rect;
  VideoPicture *vp;
  float aspect_ratio;
  int w, h, x, y;
  int i;

  vp = &is->pictq[is->pictq_rindex];
  if(vp->pict) {

    if(is->video_ctx->sample_aspect_ratio.num == 0) {
      aspect_ratio = 0;
    } else {
      aspect_ratio = av_q2d(is->video_ctx->sample_aspect_ratio) *
	is->video_ctx->width / is->video_ctx->height;
    }

    if(aspect_ratio <= 0.0) {
      aspect_ratio = (float)is->video_ctx->width /
	(float)is->video_ctx->height;
    }

    /*
    h = screen->h;
    w = ((int)rint(h * aspect_ratio)) & -3;
    if(w > screen->w) {
      w = screen->w;
      h = ((int)rint(w / aspect_ratio)) & -3;
    }
    x = (screen->w - w) / 2;
    y = (screen->h - h) / 2;
    */

    SDL_UpdateYUVTexture( texture, NULL,
                          vp->pict->data[0], vp->pict->linesize[0],
                          vp->pict->data[1], vp->pict->linesize[1],
                          vp->pict->data[2], vp->pict->linesize[2]);
    
    rect.x = 0;
    rect.y = 0;
    rect.w = is->video_ctx->width;
    rect.h = is->video_ctx->height;

    SDL_LockMutex(texture_mutex);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);
    SDL_UnlockMutex(texture_mutex);

  }
}

void video_refresh_timer(void *userdata) {

  VideoState *is = (VideoState *)userdata;
  VideoPicture *vp;
  
  if(is->video_st) {
    if(is->pictq_size == 0) {
      schedule_refresh(is, 1); //if the queue is empty, so we shoud be as fast as checking queue of picture
    } else {
      vp = &is->pictq[is->pictq_rindex];
      /* Now, normally here goes a ton of code
	 about timing, etc. we're just going to
	 guess at a delay for now. You can
	 increase and decrease this value and hard code
	 the timing - but I don't suggest that ;)
	 We'll learn how to do it for real later.
      */
      schedule_refresh(is, 40);
      
      /* show the picture! */
      video_display(is);
      
      /* update queue for next picture! */
      if(++is->pictq_rindex == VIDEO_PICTURE_QUEUE_SIZE) {
	is->pictq_rindex = 0;
      }
      SDL_LockMutex(is->pictq_mutex);
      is->pictq_size--;
      SDL_CondSignal(is->pictq_cond);
      SDL_UnlockMutex(is->pictq_mutex);
    }
  } else {
    schedule_refresh(is, 100);
  }
}
      
void alloc_picture(void *userdata) {

  VideoState *is = (VideoState *)userdata;
  VideoPicture *vp;

  vp = &is->pictq[is->pictq_windex];
  if(vp->pict) {//free space if vp->pict is not NULL
    avpicture_free(vp->pict);
    free(vp->pict);
  }

  // Allocate a place to put our YUV image on that screen
  SDL_LockMutex(texture_mutex);
  vp->pict = (AVPicture*)malloc(sizeof(AVPicture));
  if(vp->pict){
    avpicture_alloc(vp->pict,
		    AV_PIX_FMT_YUV420P, 
		    is->video_ctx->width,
		    is->video_ctx->height);
  }
  SDL_UnlockMutex(texture_mutex);

  vp->width = is->video_ctx->width;
  vp->height = is->video_ctx->height;
  vp->allocated = 1;

}

int queue_picture(VideoState *is, AVFrame *pFrame) {

  VideoPicture *vp;
  int dst_pix_fmt;
  AVPicture pict;

  /* wait until we have space for a new pic */
  SDL_LockMutex(is->pictq_mutex);
  while(is->pictq_size >= VIDEO_PICTURE_QUEUE_SIZE &&
	!is->quit) {
    SDL_CondWait(is->pictq_cond, is->pictq_mutex);
  }
  SDL_UnlockMutex(is->pictq_mutex);

  if(is->quit){
    fprintf(stderr, "quit from queue_picture....\n");
    return -1;
  }

  // windex is set to 0 initially
  vp = &is->pictq[is->pictq_windex];

  /*
  fprintf(stderr, "vp.width=%d, vp.height=%d, video_ctx.width=%d, video_ctx.height=%d\n", 
		  vp->width, 
		  vp->height, 
		  is->video_ctx->width,
		  is->video_ctx->height);
  */

  /* allocate or resize the buffer! */
  if(!vp->pict ||
     vp->width != is->video_ctx->width ||
     vp->height != is->video_ctx->height) {

    vp->allocated = 0;
    alloc_picture(is);
    if(is->quit) {
      fprintf(stderr, "quit from queue_picture2....\n");
      return -1;
    }
  }

  /* We have a place to put our picture on the queue */

  if(vp->pict) {

    // Convert the image into YUV format that SDL uses
    sws_scale(is->sws_ctx, (uint8_t const * const *)pFrame->data,
	      pFrame->linesize, 0, is->video_ctx->height,
	      vp->pict->data, vp->pict->linesize);
    
    /* now we inform our display thread that we have a pic ready */
    if(++is->pictq_windex == VIDEO_PICTURE_QUEUE_SIZE) {
      is->pictq_windex = 0;
    }
    SDL_LockMutex(is->pictq_mutex);
    is->pictq_size++;
    SDL_UnlockMutex(is->pictq_mutex);
  }
  return 0;
}

int video_thread(void *arg) {
  VideoState *is = (VideoState *)arg;
  AVPacket pkt1, *packet = &pkt1;
  int frameFinished;
  AVFrame *pFrame;

  pFrame = av_frame_alloc();

  for(;;) {
    if(packet_queue_get(&is->videoq, packet, 1) < 0) {
      // means we quit getting packets
      break;
    }

    // Decode video frame
    avcodec_decode_video2(is->video_ctx, pFrame, &frameFinished, packet);

    // Did we get a video frame?
    if(frameFinished) {
      if(queue_picture(is, pFrame) < 0) {
	break;
      }      
    }

    av_free_packet(packet);
  }
  av_frame_free(&pFrame);
  return 0;
}

int stream_component_open(VideoState *is, int stream_index) {

  int64_t in_channel_layout, out_channel_layout;

  AVFormatContext *pFormatCtx = is->pFormatCtx;
  AVCodecContext *codecCtx = NULL;
  AVCodec *codec = NULL;
  SDL_AudioSpec wanted_spec, spec;

  if(stream_index < 0 || stream_index >= pFormatCtx->nb_streams) {
    return -1;
  }

  codec = avcodec_find_decoder(pFormatCtx->streams[stream_index]->codec->codec_id);
  if(!codec) {
    fprintf(stderr, "Unsupported codec!\n");
    return -1;
  }

  codecCtx = avcodec_alloc_context3(codec);
  if(avcodec_copy_context(codecCtx, pFormatCtx->streams[stream_index]->codec) != 0) {
    fprintf(stderr, "Couldn't copy codec context");
    return -1; // Error copying codec context
  }


  if(codecCtx->codec_type == AVMEDIA_TYPE_AUDIO) {
    // Set audio settings from codec info
    wanted_spec.freq = codecCtx->sample_rate;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = codecCtx->channels;
    wanted_spec.silence = 0;
    wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
    wanted_spec.callback = audio_callback;
    wanted_spec.userdata = is;
    
    if(SDL_OpenAudio(&wanted_spec, &spec) < 0) {
      fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
      return -1;
    }
  }

  if(avcodec_open2(codecCtx, codec, NULL) < 0) {
    fprintf(stderr, "Unsupported codec!\n");
    return -1;
  }

  switch(codecCtx->codec_type) {
  case AVMEDIA_TYPE_AUDIO:
    is->audioStream = stream_index;
    is->audio_st = pFormatCtx->streams[stream_index];
    is->audio_ctx = codecCtx;
    is->audio_buf_size = 0;
    is->audio_buf_index = 0;
    memset(&is->audio_pkt, 0, sizeof(is->audio_pkt));
    packet_queue_init(&is->audioq);
    SDL_PauseAudio(0);

    in_channel_layout=av_get_default_channel_layout(is->audio_ctx->channels);
    out_channel_layout = in_channel_layout;

    is->audio_swr_ctx = swr_alloc();
    swr_alloc_set_opts(is->audio_swr_ctx,
                       out_channel_layout,
                       AV_SAMPLE_FMT_S16,
                       is->audio_ctx->sample_rate,
                       in_channel_layout,
                       is->audio_ctx->sample_fmt,
                       is->audio_ctx->sample_rate,
                       0,
                       NULL);

    fprintf(stderr, "swr opts: out_channel_layout:%lld, out_sample_fmt:%d, out_sample_rate:%d, in_channel_layout:%lld, in_sample_fmt:%d, in_sample_rate:%d",
                    out_channel_layout, 
		    AV_SAMPLE_FMT_S16, 
		    is->audio_ctx->sample_rate, 
		    in_channel_layout, 
		    is->audio_ctx->sample_fmt, 
		    is->audio_ctx->sample_rate);

    swr_init(is->audio_swr_ctx);

    break;

  case AVMEDIA_TYPE_VIDEO:
    is->videoStream = stream_index;
    is->video_st = pFormatCtx->streams[stream_index];
    is->video_ctx = codecCtx;
    packet_queue_init(&is->videoq);
    is->video_tid = SDL_CreateThread(video_thread, "video_thread", is);
    is->sws_ctx = sws_getContext(is->video_ctx->width, 
				 is->video_ctx->height,
				 is->video_ctx->pix_fmt, 
				 is->video_ctx->width,
				 is->video_ctx->height, 
				 AV_PIX_FMT_YUV420P,
				 SWS_BILINEAR, 
				 NULL, NULL, NULL);
    break;
  default:
    break;
  }

  return 0;
}

int decode_thread(void *arg) {

  Uint32 pixformat;

  VideoState *is = (VideoState *)arg;
  AVFormatContext *pFormatCtx;
  AVPacket pkt1, *packet = &pkt1;

  int i;
  int video_index = -1;
  int audio_index = -1;

  is->videoStream = -1;
  is->audioStream = -1;

  global_video_state = is;

  // Open video file
  if(avformat_open_input(&pFormatCtx, is->filename, NULL, NULL)!=0)
    return -1; // Couldn't open file

  is->pFormatCtx = pFormatCtx;
  
  // Retrieve stream information
  if(avformat_find_stream_info(pFormatCtx, NULL)<0)
    return -1; // Couldn't find stream information
  
  // Dump information about file onto standard error
  av_dump_format(pFormatCtx, 0, is->filename, 0);
  
  // Find the first video stream
  for(i=0; i<pFormatCtx->nb_streams; i++) {
    if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO &&
       video_index < 0) {
      video_index=i;
    }
    if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO &&
       audio_index < 0) {
      audio_index=i;
    }
  }

  if(audio_index >= 0) {
    stream_component_open(is, audio_index);
  }
  if(video_index >= 0) {
    stream_component_open(is, video_index);
  }   

  if(is->videoStream < 0 || is->audioStream < 0) {
    fprintf(stderr, "%s: could not open codecs\n", is->filename);
    goto fail;
  }

  fprintf(stderr, "video context: width=%d, height=%d\n", is->video_ctx->width, is->video_ctx->height);
  win = SDL_CreateWindow("Media Player",
     		   SDL_WINDOWPOS_UNDEFINED,
		   SDL_WINDOWPOS_UNDEFINED,
		   is->video_ctx->width, is->video_ctx->height,
		   SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  
  renderer = SDL_CreateRenderer(win, -1, 0);

  pixformat = SDL_PIXELFORMAT_IYUV;
  texture = SDL_CreateTexture(renderer,
			      pixformat, 
			      SDL_TEXTUREACCESS_STREAMING,
			      is->video_ctx->width,
			      is->video_ctx->height);

  // main decode loop
  for(;;) {

    if(is->quit) {
      SDL_CondSignal(is->videoq.cond);
      SDL_CondSignal(is->audioq.cond);
      break;
    }

    // seek stuff goes here
    if(is->audioq.size > MAX_AUDIOQ_SIZE ||
       is->videoq.size > MAX_VIDEOQ_SIZE) {
      SDL_Delay(10);
      continue;
    }

    if(av_read_frame(is->pFormatCtx, packet) < 0) {
      if(is->pFormatCtx->pb->error == 0) {
	SDL_Delay(100); /* no error; wait for user input */
	continue;
      } else {
	break;
      }
    }

    // Is this a packet from the video stream?
    if(packet->stream_index == is->videoStream) {
      packet_queue_put(&is->videoq, packet);
      fprintf(stderr, "put video queue, size :%d\n", is->videoq.nb_packets);
    } else if(packet->stream_index == is->audioStream) {
      packet_queue_put(&is->audioq, packet);
      fprintf(stderr, "put audio queue, size :%d\n", is->audioq.nb_packets);
    } else {
      av_free_packet(packet);
    }

  }

  /* all done - wait for it */
  while(!is->quit) {
    SDL_Delay(100);
  }

 fail:
  if(1){
    SDL_Event event;
    event.type = FF_QUIT_EVENT;
    event.user.data1 = is;
    SDL_PushEvent(&event);
  }

  return 0;
}

int main(int argc, char *argv[]) {

  int 		  ret = -1;

  SDL_Event       event;

  VideoState      *is;

  if(argc < 2) {
    fprintf(stderr, "Usage: test <file>\n");
    exit(1);
  }

  audiofd = fopen("testout.pcm", "wb+");
  audiofd1 = fopen("testout1.pcm", "wb+");

  //big struct, it's core
  is = av_mallocz(sizeof(VideoState));

  // Register all formats and codecs
  av_register_all();
  
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
    fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
    exit(1);
  }

  texture_mutex = SDL_CreateMutex();

  av_strlcpy(is->filename, argv[1], sizeof(is->filename));

  is->pictq_mutex = SDL_CreateMutex();
  is->pictq_cond = SDL_CreateCond();

  //set timer
  schedule_refresh(is, 40);

  is->parse_tid = SDL_CreateThread(decode_thread, "decode_thread", is);
  if(!is->parse_tid) {
    av_free(is);
    goto __FAIL;
  }

  for(;;) {

    SDL_WaitEvent(&event);
    switch(event.type) {
    case FF_QUIT_EVENT:
    case SDL_QUIT:
      fprintf(stderr, "receive a QUIT event: %d\n", event.type);
      is->quit = 1;
      //SDL_Quit();
      //return 0;
      goto __QUIT;
      break;
    case FF_REFRESH_EVENT:
      //fprintf(stderr, "receive a refresh event: %d\n", event.type);
      video_refresh_timer(event.user.data1);
      break;
    default:
      break;
    }
  }

__QUIT:
  ret = 0;
  

__FAIL:
  SDL_Quit();
  if(audiofd){
    fclose(audiofd);
  }
  if(audiofd1){
    fclose(audiofd1);
  }
  return ret;

}
