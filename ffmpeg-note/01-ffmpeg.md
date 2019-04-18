# 移动端音视频入门

## 1. 万人直播架构讲解

直播产品的种类：

- 泛娱乐化直播

  花椒、映客等娱乐直播，还有斗鱼、熊猫等游戏直播

- 实时互动直播

  音视频会议、教育直播等，像 思科、全时、声网

泛娱乐化直播架构

> 信令服务器：创建房间、聊天、礼物。。。。
>
> 美女主播 --信令--> 信令服务器
>
> 信令服务器--rtmp流地址-->美女主播
>
> 美女主播 --推流--> 流媒体云CDN
>
> 观众 --信令--> 信令服务器：将观众加入到美女主播间
>
> 信令服务器--rmtp流地址--> 观众
>
> 观众 <--拉流--> 流媒体云CDN

泛娱乐化直播架构

基于TCP协议实现

1. 发送信令到信令服务器, 服务器收到\执行后, 返回给共享端一个流媒体云的地址
2. 共享端采集自己音视频数据, 形成rtmp流, 推送到CDN网络(推流)
3. 获取流媒体云地址
4. 拉流

<img src="_asset/泛娱乐化直播架构.png">

实时互动直播架构

基于UDP实现

1. 自有网络: UDP没有自有网络, 需自己搭建
2. 多个节点: 为了保障服务的稳定性以及负载均衡
3. 控制中心: 每个节点定期(心跳)向控制中心报告健康程度, 控制中心根据响应的数据做出决策
4. 内总线: 数据安全性\吞吐量等可靠性得以保障
5. 媒体服务器: 将RTP协议的数据转换成RTMP协议的数据
6. CDN网络: 根据用户需求进行拉流

<img src="_asset/实时互动直播架构.png">

## 2. CDN网络介绍

> CDN网络是为了解决什么问题而出现的？
>
> 总结为一句话：CDN网络是为了解决用户访问网络资源慢而出现的一个技术，两个原因：
>
> 1. 网络链路太长
> 2. 人为因素（南电信北联通，利益相关）

CDN构成：

边缘结点：用户从边缘节点上获取数据

二级节点：主干网节点，主要用于缓存、减转源站压力

源站：CP(内容提供方)将内容放到源站

查找顺序：边缘结点->二级节点->源站

<img src="_asset/CDN网络.png">

## 3. 亲手搭建一套简单的直播系统

安装nginx 配置rtmp

```shell
$ brew install nginx-full --with-rtmp-module

#(这一步卡了我好久，安装nginx提示一直找不到nginx-full,网上相关的教程没更新，原因在于nginx仓库已搬迁)
$ brew tap denji/nginx
$ nginx -s reload 重启
$ nginx  启动
```

```shell
$ vi /usr/local/etc/nginx/nginx.conf
```

<img src="_asset/nginx-trmp配置.png">

```shell
# 推流
$ ffmpeg -re -i out.mp4 -c copy -f flv rtmp://server/live/streamName

# 拉流
$ ffmpeg -i rtmp://server/live/streamName -c copy dump.flv
```

<img src="_asset/FFMPEG直播命令.png">

<img src="_asset/测试流媒体服务器.png">

## 4. 音频基础知识

<img src="_asset/声音三要素.png">

图一音量：甲乙的振动频率相同、振幅不同。图二音调：甲乙振幅相同、频率不同

<img src="_asset/音量与音调.png">

<img src="_asset/音色.png">

<img src="_asset/人类听觉范围.png">

<img src="_asset/听觉-发声范围.png">

## 5. 音频的量化与编码

模拟信号进行采样，采样时分频率的从模拟信号获取数据波形值，采样后，进行数据量化，量化后进行编码，把采样的十进制转化为计算机的二进制，也就是数字信号。

模拟数据——》采样——》量化——》编码——》数字信号

<img src="_asset/音频量化过程.png">

采样大小决定了音频的振幅的高度，采样时指一个采样用多少bit存放，常用的是16bit

```shell
# bit：位     一个二进制数据0或1，是1bit
# byte：字节  存储空间的基本计量单位，如：MySQL中定义 VARCHAR(45) 即是指 45个字节；
# 1 Byte = 8 Bit = 1 字节

# 2^8 = 256, 2^16 = 65535
```

aac通常44.1k采样率

采样率:采样频率8k/秒、16k/秒、32k/秒、44.1k/秒、48k/秒

> [什么是音频的采样率？采样率和音质有没有关系？](<https://www.zhihu.com/question/20351692>) - 知乎

<img src="_asset/量化基本概念.png">

人能听到的声音范围是20hz-2whz

码率 = 采样率 x 采样大小 x 声道数

```shell
# 宽带速率的单位用 bps(或b/s)表示
# 1 B = 8 b   1 B/s = 8 b/s
```

<img src="_asset/码率计算.png">

原始的wav文件，大小是1411.2Kb/s

做完aaclc的编码，大小是128Kb/s

如果是aache-vr这种编码，大小是32Kb/s

## 6. 音频压缩技术讲解

音频压缩技术

1、消除冗余数据（有损压缩技术）。

压缩的主要方法是去除采集到的音频冗余信息，所谓冗余信息包括人耳听觉范围外的音频信号以及被掩蔽掉的音频信号

信号的掩蔽可分为频域掩蔽和时域掩蔽

频域掩蔽：一个强纯音会掩蔽在其附近同时发声的弱纯音。也称同时掩蔽

时域掩蔽：在时间上相邻的声音之间也有掩蔽现象，主要原因是人的大脑处理信息需要花费时间。

同步掩蔽效应和不同频率声音的频率和相对竟是有关，而时间掩蔽则仅仅和时间有关。如果两个声音在时间上特别接近，分辨会有困难（如两个声音音量相差较大且两个声音间隔时间低于5毫秒，则其中弱的那个声音会听不到）。

2、哈夫曼无损编码

<img src="_asset/音频压缩技术.png">

音频压缩：频域，时域。

- 频域: 截取人耳能听到的频率范围，滤掉响度低的声音，去掉某个高频周围低频的声音；

- 时域: 滤掉某个长时间说话中的低音

<img src="_asset/音频冗余信息.png">

<img src="_asset/频域掩蔽效应.png">

<img src="_asset/时域掩蔽效应.png">

<img src="_asset/音频编码过程.png">

## 7. 音频编解码器选型

网上测评结果：音频编解码器 opus > aac > vorbis 

音频编解码器：

1：opus，

- 口模型：实时互动，对实时性要求非常高 
- 耳模型：高保真，对质量要求非常高

至于什么时候使用那个模型，由opus自己内部来决定，同时，他是性能最好的，压缩率最好。

2：AAC，经常用于泛娱乐化直播，因为其对实时性要求不是很高但是对音质要求可能较高，所以，选用AAC，当然也可以选用opus的耳模型

3：sppex，最大的特点就是不仅可以编码音频，还可以对音频进行降噪，优化，尽可能的获取原音频数据

4：G.711(722)，主要用于音视频会议，为了和固话进行相应的融合

<img src="_asset/常见的音频编码器.png">

<img src="_asset/音频编码器性能对比.png">

## 8. AAC 讲解

cdn，rtmp  支持 aac

AAC 产生的目的是取代 MP3 格式：

AAC 相对优点：压缩率高，损耗低

<img src="_asset/AAC介绍.png">

<img src="_asset/AAC规格.png">

aac 三种类型
aac
aacv1: aac+sbr(频率复用-高频部分采样率高，低频部分采样率低)
aacv2: aac+sbr+ps(声道关联，一个声道采集全部，一个声道只采集相对不同的声音)

AAC规格描述（AAC、AAC HE、AAC HE V2）--> AAC+SBR=AAC HE V1, AAC + SBR + PS = AAC HE V2

<img src="_asset/AAC规格描述.png">

AAC格式：

1、ADIF(Audio Data Interchange Format):只能从头开始解码，常用在磁盘文件中。

2、ADTS(Audio Data Transport Stream)：这种格式每一帧都有一个同步字，可以在音频流的任何位置开始解码，它似于数据流格式（缺点：文件比ADIF大，优点:每个帧都可以被单独解码播放）

<img src="_asset/AAC格式.png">

aac 编码库 ffmpeg AAC，libfdk AAC

<img src="_asset/AAC编码库那个好.png">

## 9. 视频基本知识

I帧：关键帧，采用帧内压缩技术

P帧：向前参考帧，压缩时只参考前一个帧，属于帧间压缩技术

B帧：双向参考帧，压缩时即参考前一帧也参考后一帧，属于帧间压缩技术

一般实时互动都不会使用 B 帧

<img src="_asset/H264基本概念.png">

GOF(group of frame): 一组帧，可以将一段时间内画面变化不大的所有帧划为一组帧

<img src="_asset/GOF.png">

SPS与PPS（这两种都划为 I 帧）：

- SPS(Sequence Parameter Set): 

  序列参数集，存放帧数、参考帧数目、解码图像尺寸、帧场编码模式选择标识等。

- PPS(Picture Parameter Set):

  图像参数集，存放熵编码模式选择标识、片组数目、初始量化参数和去方块滤波系统数调整标识等

<img src="_asset/SPS与PPS.png">

视频花屏/卡顿原因：

1、如果 GOP 分组中的 P 帧丢失会造成解码端的图像发生错误（于是形成了花屏）。

2、为了避免花屏问题的发生，一般如果发现 P 帧或者I帧丢失，就不显示本 GOP 内的所有帧，直到下一个 I 帧来后重新刷新图像（因为丢了一组数据，所以形成了卡顿）

<img src="_asset/视频花屏卡顿的原因.png">

视频编码器：

1、x264/x265。

2、openH264(支持 SVC（分层传输） 技术)。

3、vp8/vp9

<img src="_asset/视频都有哪些视频编码器.png">

## 10. H264 宏块的划分与帧分组

H264压缩技术

1. 帧内预测压缩，解决的是空域数据冗余问题（将一幅图里的人眼不是很敏感的色彩、光亮等数据剔除）
2. 帧间预测压缩，解决的是时域数据冗余问题（将一组图里面连续的重复性高的帧剔除）
3. 整数离散余弦变换(DCT)，将空间上的相关性变为频域上无关的数据然后进行量化
4. CABAC压缩，也叫上下文适应无损压缩

<img src="_asset/H264压缩技术.png">

宏块的划分与分组：

H264宏块划分与子块划分：宏块里面可以再包含很多子块

<img src="_asset/H264宏块划分.png">

<img src="_asset/宏块划分完成.png">

子块划分：

<img src="_asset/子块划分.png">

帧分组(一组连续的图片，一幅图片为一帧)

<img src="_asset/帧分组.png">

## 11. 视频压缩技术详解

- 帧间预测: 

  解决时间数据冗余，比较相邻两帧不同给出运动矢量 + 残差值

- 帧内预测: 

  解决空间数据冗余，每一个宏块有一个预测模式，然后讲预测后的图像与原图比较算差值，最后存储预测模式和差值即可。帧内压缩是针对于 I 帧的

### 11.1 帧间预测

组内宏块查找：

<img src="_asset/组内宏块查找.png">

<img src="_asset/运动估算.png">

<img src="_asset/运动矢量与补偿压缩.png">

### 11.2 帧内预测

<img src="_asset/帧内预测.png">

<img src="_asset/计算帧内预测残差值.png">

<img src="_asset/预测模式与残差值压缩.png">

### 11.3 DCT 压缩

<img src="_asset/DCT压缩.png">

<img src="_asset/压缩后的结果.png">

### 11.4 VLC 压缩

<img src="_asset/VLC压缩.png">

### 11.5 CABAC 压缩

<img src="_asset/CABAC压缩.png">

## 12. H264 结构与码流

H264编码分层：

1、NAL层（Network Abstraction Layer）, 视频数据网络抽象层。

2、VCL层（Video Coding Layer），视频数据编码层，对原始数据进行压缩

码流基本概念：

1、SODB（String Of Data Bits）,原始数据比特流，长度不一定是8的倍数，它是由VCL层产生的。

2、RBSP（Raw Byte Sequence Payload,SODB+trailing bits），算法是在SODB最后一位补1，不按字节对齐则补0。

3、EBSP(Encapsulate Byte Sequence Payload)，需到两个连续的0x00就增加一个0x03。

4、NALU，NAL Header(1B)+EBSP

<img src="_asset/H264结构图.png">

以太网最大传输字节 1500 字节。

<img src="_asset/H264编码分层.png">

<img src="_asset/码流基本概念一.png">

<img src="_asset/码流基本概念二.png">

一个H264帧最少要有一个切片(NAL Unit)

<img src="_asset/NALUnit.png">

切片与宏块的关系：

- 每个切片都包括切片头和切片数据，
- 每个切片数据又包括了很多宏块，
- 每个宏块又包括了宏块的类型、宏块的预测、编码的残渣数据等

<img src="_asset/切片与宏.png">

<img src="_asset/H264切片.png">

<img src="_asset/H264码流分层.png">

## 13. NAL 单元详解

<img src="_asset/NALHeader.png">

5 - 关键帧

7- SPS 序列参数集

8- PPS 图像参数集

<img src="_asset/NALType一.png">

<img src="_asset/NALType二.png">

<img src="_asset/NAL类型介绍.png">

<img src="_asset/单一NALU的RTP包.png">

<img src="_asset/组合NALU的RTP包.png">

如：

P帧B帧很多都是单一类型。

SPS和PPS这两个NAL单元一般放在同一个RTP包里头

<img src="_asset/分片NALU的RTP包.png">

<img src="_asset/FUHeader.png">

## 14. YUV 讲解

<img src="_asset/图像除了RGB还是有YUV.png">

<img src="_asset/YUV.png">

YUV常见格式：YUV4:2:0、YUV4:2:2、YUV4:4:4

RGB8:8:8

<img src="_asset/YUV常见格式.png">

<img src="_asset/YUV420.png">

- UV 混存则为packed(打包存储)，
- UV分开存则为planar(平面存储) 

<img src="_asset/YUV存储格式.png">

## 15. 总结

<img src="_asset/小结.png">

<img src="_asset/音频小结.png">

<img src="_asset/视频小结.png">

rtmp 实时消息传输: tcp/ip 应用层协议  推送/直播  基本数据单元为消息

1B 消息类型  2B 长度  3B 时间 4B  流id 消息体

传输时 消息回被拆分成消息块 chunk chunk header + chunk data

flv: 大块音视频 加入标记头信息   延迟表现和大规模并发成熟 

HLS：分成5-10s 用m3u8索引管理 用于朋友圈分享  

m3u8索引： 直播信号源--视频编码器（后台视频处理）--流切片器--各种ts媒体文件（分发模块）--索引文件（数据库）--客户端



cdn网络 为了解决用户访问资源慢出现的技术

边缘节点  二级节点（大城市） 源站

搭建流媒体服务：

准备流媒体服务器 linux max 编译安装nginx服务  配置rtmp服务并启动nginx服务



声音三要素：音调 音量 音色

音频量化(模数转换)：模拟数据 采样  量化 编码  数字信号  == 0101001110

码率 = 采样率（1.6w/44.1/48k）x 采样大小(8位-电话/16位-常见) x 声道数（单/双）

音频压缩： 有损消除冗余数据   哈夫曼无损编码

音频编码： 时域转频域---心里声学模型---量化编码---比特流格式化---比特流

音频编解码 ： opus（口 耳 实时互动 最快）  aac(直播用 次快)  speed(回音 降噪等)   g.711（固话）

aac : 取代mp3 加入 sir ps 技术  

aac lc 128k / aac he v2 64k /  aac he v2 32k/

aac 格式 ： adif 从头开始解码，用在磁盘文件中  adts 每一帧都有一个同步字，可以在任何位置解码

aac 编码库 ： libfdk_aac > ffmpeg aac >libfaac> libvo_aacenc



H264： I帧 关键 帧内压缩  / p帧 向前参考1帧 / B帧 双向参考帧

sps: 序列参数集/pps:图像参数集 

GOF： 一组帧数  p帧丢失 会花屏卡顿

视频编码器： x264/x265 /open h264(svc)/vp8/vp9

h264 压缩技术-编码原理： 帧内预测压缩，空域冗余数据/帧间预测压缩，时域冗余数据/dcp整数离散余炫变换，傅立叶变换/cabac压缩

h264结构：视频序列--图像--片--宏块--子快

h264编码分层：nal 视频数据网络抽象层--vcl 视频数据编码层

码率：sodb 原始比特流 / rbsp sodb最后补1 / ebsp 起始码增加一个起始位0x03 /  nalu nal+ebsp

nal unit = nalu 头部 + 一个切片（头/数据） 切片 

yuv格式：4：4:4/4:4:2/4：2:0 （平坦编码 /半平坦编码）

<img src="_asset/后续.png">

<img src="_asset/音视频知识01.png">

<img src="_asset/音视频知识02.png">

<img src="_asset/音视频知识03.png">

<img src="_asset/行业痛点-01.png">

<img src="_asset/行业痛点-02.png">



