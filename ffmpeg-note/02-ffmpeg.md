# FFmpeg 音视频核心技术

## 1. 学习大纲

**FFmpeg 常用命令**：

- 视频录制命令
- 多媒体文件的分解/复用命令
- 裁剪与合并命令
- 图片/视频互转命令
- 直播相关命令
- 各种滤镜命令

**FFmpeg 基本开发**：

- C 语言回顾
- FFmpeg 核心概念与常用结构体
- 实战 - 多媒体文件的分解与复用
- 实战 - 多媒体格式的互转
- 实战 - 从 MP4 裁剪一段视频
- 作业 - 实现一个简单的小咖秀

**音视频编解码实战**：

- 实战 - H264 解码
- 实战 - H264 编码
- 实战 - 音频 AAC 解码
- 实战 - 音频 AAC 编码
- 实战 - 视频转图片

**音视频渲染实战**：

- SDL 事件处理
- SDL 视频文理渲染
- SDL 音频渲染
- 实战1 - 实现 YUV 视频播放
- 实战2 - YUV 视频倍数播放
- 实战3 - 实现 PCM 播放器

**FFmpeg 开发播放器核心功能**：

- 实战 - 实现 MP4 文件的视频播放
- 实战 - 实现 MP4 文件的音频播放
- 实战 - 实现一个初级播放器
- 实战 - 音视频同步
- 实战 - 实现播放器内核

**Android 中实战 FFmpeg**：

- 编译 Android 端可以使用的 FFmpeg
- Java 与 C 语言相互调用
- 实战 - Android 调用 FFmpeg

**学习建议**：

- 牢牢抓住音视频的处理机制，了解其本质
- 勤加练习，熟能生巧
- 待着问题去学习，事半功倍

**音视频的广泛应用**：

- 直播类：音视频会议、教育直播、娱乐/游戏直播
- 短视频：抖音、快手、小咖秀
- 网络视频：优酷、腾讯视频、爱奇艺等
- 音视频通话：微信、QQ、Skype等
- 视频监控
- 人工智能：人脸识别，智能音箱等，更关注算法

**播放器架构**：

<img src="_asset/播放器架构.png">

**渲染流程**：

<img src="_asset/渲染流程.png">

**FFmpeg 都能做啥**：

- FFmpeg 是一个非常优秀的多媒体框架
- FFmpeg 可以运行在 Linux、Mac、Windows 等平台上
- 能够解码、编码、转码、复用、解复用、过滤音视频数据

**FFmpeg 下载便于与安装**：

```shell
$ git clone https://git.ffmpeg.org/ffmpeg.git
$ config -- help
$ make && make install
```

## 2. FFmpeg 常用命令实战

我们按使用目的可以将 FFMPEG 命令分成以下几类：

- 基本信息查询命令
- 录制
- 分解 / 复用
- 处理原始数据
- 滤镜
- 切割与合并
- 图／视互转
- 直播相关

除了 FFMPEG 的基本信息查询命令外，其它命令都按下图所示的流程处理音视频。

<img src="_asset/FFmpeg处理音视频流程.png">

<img src="_asset/FFmpeg基本信息查询命令.png">

<img src="_asset/FFmpeg录屏命令.png">

```shell
$ ffplay -s 2560x1600 -pix_fmt uyvy422 out.yuv
```

<img src="_asset/分解与复用-01.png">

<img src="_asset/多媒体格式转换.png">





















<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">





























