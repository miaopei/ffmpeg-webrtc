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
    av_log_set_level(AV_LOG_INFO);

    AVFormatContext *fmt_ctx = NULL;

    av_register_all();

    ret = avformat_open_input(&fmt_ctx, "./test.mp4", NULL, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't open file: %s\n", av_err2str(ret));
        return -1;
    }

    av_dump_format(fmt_ctx, 0, "./test.mp4", 0);

    avformat_close_input(&fmt_ctx);

    return 0;
}

