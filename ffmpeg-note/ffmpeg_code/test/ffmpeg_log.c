/*************************************************************************
	> File Name: ffmpeg_log.c
	> Author: Mr.Miaow
	> Mail: miaopei163@163.com 
	> Created Time: 日  4/21 16:57:56 2019
 ************************************************************************/

#include <stdio.h>
#include <libavutil/log.h>

int main(int argc, char *argv[])
{
    av_log_set_level(AV_LOG_DEBUG);
    
    av_log(NULL, AV_LOG_INFO, "hello world: %s!\n", "aaa");

    return 0;
}

