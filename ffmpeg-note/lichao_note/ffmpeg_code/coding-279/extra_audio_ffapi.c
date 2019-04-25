#include <stdio.h>
#include <libavutil/log.h>
#include <libavformat/avio.h>
#include <libavformat/avformat.h>

#define ERROR_STR_SIZE 1024

int main(int argc, char *argv[])
{
    int err_code;
    char errors[1024];

    char *src_filename = NULL;
    char *dst_filename = NULL;

    FILE *dst_fd = NULL;

    int audio_stream_index = -1;
    int len;

    AVFormatContext *ofmt_ctx = NULL;
    AVOutputFormat *output_fmt = NULL;

    AVStream *in_stream = NULL;
    AVStream *out_stream = NULL;

    AVFormatContext *fmt_ctx = NULL;
    //AVFrame *frame = NULL;
    AVPacket pkt;

    av_log_set_level(AV_LOG_DEBUG);

    if(argc < 3){
        av_log(NULL, AV_LOG_DEBUG, "the count of parameters should be more than three!\n");
        return -1;
    }

    src_filename = argv[1];
    dst_filename = argv[2];

    if(src_filename == NULL || dst_filename == NULL){
        av_log(NULL, AV_LOG_DEBUG, "src or dts file is null, plz check them!\n");
        return -1;
    }

    /*register all formats and codec*/
    av_register_all();

    /*open input media file, and allocate format context*/
    if((err_code = avformat_open_input(&fmt_ctx, src_filename, NULL, NULL)) < 0){
        av_strerror(err_code, errors, 1024);
        av_log(NULL, AV_LOG_DEBUG, "Could not open source file: %s, %d(%s)\n",
               src_filename,
               err_code,
               errors);
        return -1;
    }

    /*retrieve audio stream*/
    if((err_code = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {
        av_strerror(err_code, errors, 1024);
        av_log(NULL, AV_LOG_DEBUG, "failed to find stream information: %s, %d(%s)\n",
               src_filename,
               err_code,
               errors);
        return -1;
    }

    /*dump input information*/
    av_dump_format(fmt_ctx, 0, src_filename, 0);

    in_stream = fmt_ctx->streams[1];
    AVCodecParameters *in_codecpar = in_stream->codecpar;
    if(in_codecpar->codec_type != AVMEDIA_TYPE_AUDIO){
        av_log(NULL, AV_LOG_ERROR, "The Codec type is invalid!\n");
        exit(1);
    }

    //out file
    ofmt_ctx = avformat_alloc_context();
    output_fmt = av_guess_format(NULL, dst_filename, NULL);
    if(!output_fmt){
        av_log(NULL, AV_LOG_DEBUG, "Cloud not guess file format \n");
        exit(1);
    }

    ofmt_ctx->oformat = output_fmt;

    out_stream = avformat_new_stream(ofmt_ctx, NULL);
    if(!out_stream){
        av_log(NULL, AV_LOG_DEBUG, "Failed to create out stream!\n");
        exit(1);
    }

    if(fmt_ctx->nb_streams<2){
        av_log(NULL, AV_LOG_ERROR, "the number of stream is too less!\n");
        exit(1);
    }


    if((err_code = avcodec_parameters_copy(out_stream->codecpar, in_codecpar)) < 0 ){
        av_strerror(err_code, errors, ERROR_STR_SIZE);
        av_log(NULL, AV_LOG_ERROR,
               "Failed to copy codec parameter, %d(%s)\n",
               err_code, errors);
    }

    out_stream->codecpar->codec_tag = 0;

    if((err_code = avio_open(&ofmt_ctx->pb, dst_filename, AVIO_FLAG_WRITE)) < 0) {
        av_strerror(err_code, errors, 1024);
        av_log(NULL, AV_LOG_DEBUG, "Could not open file %s, %d(%s)\n",
               dst_filename,
               err_code,
               errors);
        exit(1);
    }

    /*
    dst_fd = fopen(dst_filename, "wb");
    if (!dst_fd) {
        av_log(NULL, AV_LOG_DEBUG, "Could not open destination file %s\n", dst_filename);
        return -1;
    }
    */


    /*dump output information*/
    av_dump_format(ofmt_ctx, 0, dst_filename, 1);

    /*
    frame = av_frame_alloc();
    if(!frame){
        av_log(NULL, AV_LOG_DEBUG, "Could not allocate frame\n");
        return AVERROR(ENOMEM);
    }
    */

    /*initialize packet*/
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    /*find best audio stream*/
    audio_stream_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if(audio_stream_index < 0){
        av_log(NULL, AV_LOG_DEBUG, "Could not find %s stream in input file %s\n",
               av_get_media_type_string(AVMEDIA_TYPE_AUDIO),
               src_filename);
        return AVERROR(EINVAL);
    }

    if (avformat_write_header(ofmt_ctx, NULL) < 0) {
        av_log(NULL, AV_LOG_DEBUG, "Error occurred when opening output file");
        exit(1);
    }

    /*read frames from media file*/
    while(av_read_frame(fmt_ctx, &pkt) >=0 ){
        if(pkt.stream_index == audio_stream_index){
            pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
            pkt.dts = pkt.pts;
            pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
            pkt.pos = -1;
            pkt.stream_index = 0;
            av_interleaved_write_frame(ofmt_ctx, &pkt);
            av_packet_unref(&pkt);
        }
    }

    av_write_trailer(ofmt_ctx);

    /*close input media file*/
    avformat_close_input(&fmt_ctx);
    if(dst_fd) {
        fclose(dst_fd);
    }

    avio_close(ofmt_ctx->pb);

    return 0;
}
