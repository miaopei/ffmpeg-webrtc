# FFMpeg实时美颜直播推流

实战 - 基于ffmpeg，qt5，opencv视频课程

## 1. 基础知识

### 1.1 直播推流流程分析

<img src="_asset/直播.png">

rtmp 延时一般 1-3 秒

<img src="_asset/推流流程.png">

### 1.2 直播 rtmp 协议分析

<img src="_asset/协议.png">

<img src="_asset/传输协议.png">

<img src="_asset/RTMP.png">

<img src="_asset/RTMP优缺点.png">

<img src="_asset/RTMP协议类型.png">

<img src="_asset/握手.png">

<img src="_asset/推流.png">

## 2. 直播服务器讲解和配置

<img src="_asset/流媒体服务器.png">

### 2.1 直播服务器介绍 crtmpserver 编译运行

```shell
$ apt-get install wget cmake
$ apt-get install libssl-dev
$ wget  https://codeload.github.com/j0sh/crtmpserver/zip/centosinit --no-check-certificate
$ unzip centosinit
$ cd builders/cmake
$ cmake . 
$ make
$ ./crtmpserver/crtmpserver ./crtmpserver/crtmpserver.lua

#
$ ffmpeg  -i test.flv  -f flv rtmp://192.168.1.44/live

#ʹc rtmp://192.168.1.44/live
#ʹplay ffplay rtmp://192.168.1.44/live  -fflags nobuffer
```

```shell
# error
CMake Error at cmake_find_modules/Find_openssl.cmake:99 (MESSAGE):
  Looking for openssl headers - not found
Call Stack (most recent call first):
  CMakeLists.txt:46 (INCLUDE)
$ cmake -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl -DOPENSSL_LIBRARIES=/usr/local/opt/openssl/lib
```

### 2.2 下载 ffmpeg 工具推流并使用功能 vlc 拉流播放测试

```shell
$ wget https://nginx.org/download/nginx-1.16.0.tar.gz --no-check-certificate
$ git clone https://github.com/arut/nginx-rtmp-module.git
$ ./configure --add-module=/home/miaopei/workdir/test/ffmpet-test/nginx/nginx-rtmp-module 
$ make
$ make install
```

<img src="_asset/ffmpeg工具推流测试.png">

```shell
# nginx.conf 配置
rtmp {
    server {
        listen 1935;
        chunk_size 4096;
        application live {
            live on;
        }
    }
}
```

```shell
# 推流命令
$ ffmpeg -i test.mp4 -c copy -f flv rtmp://192.168.2.76/live
```

```shell
# 网页查看推流的状态
server {
    listen 8080;
    location /stat{
        rtmp_stat all;
        rtmp_stat_stylesheet stat.xsl;
    }
    location /stat.xsl{
        root /home/miaopei/workdir/test/ffmpet-test/nginx/nginx-rtmp-module;
    }
}
```

```shell
Reload config:
 $ nginx -s reload
Reopen Logfile:
 $ nginx -s reopen
Stop process:
 $ nginx -s stop
Waiting on exit process
 $ nginx -s quit
```



<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

## 3. FFMpeg SDK 解封和推流



## 4. OpencvSDK 基础



## 5. 视频采集编码推流和类封装



## 6. 音频录制编码推流和类封装



## 7. 音视频同步编码推流处理



## 8. XRtmpStreamer 项目完成（界面和美颜）





