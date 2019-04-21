/*************************************************************************
	> File Name: mediainfo.c
	> Author: Mr.Miaow
	> Mail: miaopei163@163.com 
	> Created Time: 日  4/21 18:31:25 2019
 ************************************************************************/

#include <stdio.h>
#include <libavutil/log.h>
#include <libavformat/avformat.h>


int main(int argc, char *argv[])
{
    int ret;
    int len;
    int audio_index;

    char *src = NULL;
    char *dst = NULL;

    av_log_set_level(AV_LOG_INFO);

    AVPacket pkt;
    AVFormatContext *fmt_ctx = NULL;

    av_register_all();

    // 1. read two params form console
    if (argc < 3) {
        av_log(NULL, AV_LOG_ERROR, "eg: %s in_file out_file\n", argv[0]);
        return -1;
    }
    src = argv[1];
    dst = argv[2];
    if (!src || !dst) {
        av_log(NULL, AV_LOG_ERROR, "src or dst is null\n");
        return -1;
    }

    ret = avformat_open_input(&fmt_ctx, src, NULL, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't open file: %s\n", av_err2str(ret));
        return -1;
    }

    FILE *dst_fd = fopen(dst, "wb");
    if (!dst_fd) {
        av_log(NULL, AV_LOG_ERROR, "Can't open out file!\n");
        avformat_close_input(&fmt_ctx);
        return -1;
    }
    av_dump_format(fmt_ctx, 0, src, 0);

    // 2. get stream
    ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't find the best stream!\n");
        avformat_close_input(&fmt_ctx);
        fclose(dst_fd);
        return -1;
    }

    audio_index = ret;
    av_init_packet(&pkt);
    while(av_read_frame(fmt_ctx, &pkt) >= 0) {
        if (pkt.stream_index == audio_index) {
            // 3. write audio data to aac file.
            len = fwrite(pkt.data, 1, pkt.size, dst_fd);
            if (len != pkt.size) {
                av_log(NULL, AV_LOG_WARNING, "warning, length of data is not equal size of pkt!\n");
            }
        }
        av_packet_unref(&pkt);
    }
    
    avformat_close_input(&fmt_ctx);
    if (dst_fd) {
        fclose(dst_fd);
    }

    return 0;
}

