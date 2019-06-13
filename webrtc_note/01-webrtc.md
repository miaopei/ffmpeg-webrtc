

# WebRTC

> [WebRTC API](<https://developer.mozilla.org/zh-CN/docs/Web/API/WebRTC_API>)
>
> [Node.js v10.15.3 文档](<http://nodejs.cn/api/>)
>
> [廖雪峰 - nodejs](<https://www.liaoxuefeng.com/wiki/1022910821149312/1023025235359040>)

## 1. WebRTC 介绍



- Google 开源
- 跨平台
- 用于浏览器
- 实时传输
  - 100ms 延迟 通话质量非常好
  - 200ms 延迟 通话质量比较优质
  - 500ms 延迟 可以接受
  - 超过1s 非常迟滞
- 音视频引擎

WebRTC 应用：

<img src="_asset/webrtc应用.png">

WebRTC 愿景：

<img src="_asset/webrtc愿景.png">

学习 WebRTC 的难点：

<img src="_asset/学习WebRTC的难点.png">

学习路线：

<img src="_asset/学习路线.png">

学习内容：

<img src="_asset/学习内容.png">

学习收获：

<img src="_asset/学习收获.png">

WebRTC能做啥：

<img src="_asset/WebRTC能做啥.png">

能学到什么：

<img src="_asset/能学到什么.png">

google webrtc 示例：https://appr.tc/

## 2. WebRTC 原理与架构

WebRTC 整体架构：

<img src="_asset/webrtc架构.png">



WebRTC 的目录结构图：

<img src="_asset/WebRTC目录结构-01.png">

<img src="_asset/WebRTC目录结构-02.png">

<img src="_asset/WebRTCModules目录-01.png">

<img src="_asset/WebRTCModules目录-02.png">

WebRTC 两个基本概念：轨与流

- Track
- MediaStream

WebRTC重要类：

- MediaStream
- RTCPeerConnection
- RTCDataChannel

PeerConnection调用过程：

<img src="_asset/PeerConnection调用过程.png">



调用时序图：

<img src="_asset/调用时序图.png">

> [WebRTC之PeerConnection的建立过程](https://www.cnblogs.com/cther/p/myPeerConnection.html)
>
> [WebRTC系列（3）：PeerConnection通信建立流程](<https://www.jianshu.com/p/43957ee18f1a>)

## 3. Web服务器原理与Nodejs搭建

Web服务器选型：

- Nodejs
- Nginx
- Apache

Web服务工作原理：

<img src="_asset/web服务工作原理.png">

Nodejs工作原理：

<img src="_asset/Nodejs工作原理.png">

JavaScript解析：

<img src="_asset/JavaScript解析.png">

Nodejs 事件处理：

<img src="_asset/Nodejs事件处理.png">

两个V8引擎：

<img src="_asset/两个V8引擎.png">

最简单的http服务：

- **require** 引入http模块
- 创建http服务
- 侦听端口

启动Nodejs服务：

- node app.js
- nohub node app.js
- forever start app.js
- pm2 start app.js

Https基本原理：

<img src="_asset/https基本原理.png">

Nodejs 搭建 https 服务：

- 生成 HTTPS证书
- 引入 HTTPS模块
- 指定证书位置，并创建 HTTPS 服务

真正的Web服务：

- 引用 express 模块
- 引入 server-index 模块
- 指定发布目录

## 4. JavaScript 必备知识回顾

基础知识：

- 变量与类型
- 基本运算
- `if/else`
- for循环
- 函数
- 日志打印

变量与类型：

<img src="_asset/变量与类型.png">

基本运算：

<img src="_asset/基本运算.png">

<img src="_asset/ifelse.png">

<img src="_asset/for循环.png">

<img src="_asset/函数.png">

## 5. WebRTC设备管理

enumerateDevices：

<img src="_asset/enumerateDevices.png">

JavaScript中的Promise：

<img src="_asset/JavaScript中的Promise.png">

## 6. WebRTC音视频数据采集

音视频采集API：

<img src="_asset/音视频采集API.png">

getUserMedia的不同实现：

<img src="_asset/getUserMedia的不同实现.png">

适配置不同浏览器的方法：

<img src="_asset/适配置不同浏览器的方法.png">

`https://webrtc.github.io/adapter/adapter-latest.js`

WebRTC音视频采集约束：

约束详解：

- width

- height

  宽高比例：`4:3`  `16:9`

- aspectRatio

- frameRate

- facingMode

  - user - 前置摄像头
  - environment - 后置摄像头
  - left - 前置左侧摄像头
  - right - 前置右侧摄像头

- resizeMode

音频约束详解：

- volume - `范围 0 - 1.0`
- sampleRate
- sampleSize -  一般16位
- echoCancellation - 回音消除
- autoGainControl - 是否在原有声音基础上增加音量
- noiseSuppression - 降噪
- latency - 延迟大小
- channelCount - 声道  乐器一般是双声道
- deviceID - 作用是多个设备切换
- groupID 

WebRTC约束例子：

```json
{
    audio: true,
    video: {
        width: {
            min: 300,
            max: 640,
        },
        height: {
            min: 300,
            max: 480,
        },
        frameRate: {
            min: 15,
            max: 30,
        }
    }
}
```

浏览器视频特效：

- CSS filter，`-webkit-filter/filter`
- 如何将 video 与 filter 关联
- OpenGL/Metal/...

支持的特效种类：

<img src="_asset/支持的特效种类.png">

保存图片是实现滤镜效果，可以对 canvas.data 进行数据修改。

MediaStream API 获取视频约束：

<img src="_asset/MediaStream.png">

<img src="_asset/MediaStream事件.png">

## 7. WebRTC音视频录制实战

### 7.1 WebRTC录制基本知识

MediaRecoder类：

<img src="_asset/MediaRecoder.png">

<img src="_asset/MediaRecorder参数.png">

<img src="_asset/MediaRecorderAPI-01.png">

<img src="_asset/MediaRecorderAPI-02.png">

<img src="_asset/MediaRecorder事件.png">

<img src="_asset/JavaScript几种存储数据的方式.png">

### 7.2 WebRTC 捕获桌面

<img src="_asset/getDisplayMedia.png">

捕获桌面需要设置Chrome，具体 操作：<chrome://flags/#enable-experimental-web-platform-features>

- Experimental Web Platform features 设置为 enable

## 8. WebRTC信令服务器实现

如果没有信令服务器WebRTC之间是不能通信的。

两个client之间通信必须有两个信息通过信令服务器的：

- 媒体信息， SDP
- 网络信息
- 具体的业务

<img src="_asset/信令服务器的作用.png">

<img src="_asset/为什么要使用socketio.png">

<img src="_asset/socketio工作原理.png">

Socket.IO 发送消息：

- 给本次连接发送消息

  ```js
  socket.emit()
  ```

- 给某个房间内所有人发送消息

  ```js
  io.in(room).emit()
  ```

- 除本链接外，给某个房间内所有人发送消息

  ```js
  socket.to(room).emit()
  ```

- 除本链接外，给所有人发送消息

  ```js
  socket.broadcast.emit()
  ```

Socket.IO 客户端处理消息：

- 发送 action 命令

  ```js
  S: socket.emit('action');
  C: socket.on('action', function(){...});
  ```

- 发送了一个 action 命令，还有 data 数据

  ```js
  S: socket.emit('action', data);
  C: socket.on('action', function(data){...});
  ```

- 发送了 action 命令，还有两个数据

  ```js
  S: socket.emit('action', arg1, arg2);
  C: socket.on('action', function(arg1, arg2){...});
  ```

- 发送了一个 action 命令，在 emit 方法中包含回调函数

  ```js
  S: socket.emit('action', data, function(arg1, arg2){...};
  C: socket.on('action', function(data, fn){fn('a', 'b');});
  ```

### 8.1 [实战] 通过 socket.io 实现信令服务器

改造服务端的基本流程：

- 安装 socket.io
- 引入 socket.io
- 处理 connection 消息

## 9. WebRTC网络基础补充：P2P/STUN/TRUN/ICE知识

### 9.1 WebRTC 网络传输基本知识

WebRTC 传输基本知识：

- NAT（Network Address Translator）
- STUN（Simple Traversal of UDP Through NAT）
- TURN（Travelsal Using Relays around NAT）
- ICE（Interactive Connectivity Establishment）

<img src="_asset/NAT.png">

NAT 产生的原因：

- 由于IPv4的地址不够
- 处于网络安全的原因

NAT 的种类：

- 完全锥型 NAT（Full Cone NAT）
- 地址限制锥型 NAT（Address Restricted Cone NAT）
- 端口限制锥型 NAT（Port Restricted Cone NAT）
- 对称型 NAT（Symmetric NAT）

### 9.2 NAT 打洞原理

<img src="_asset/完全锥型NAT.png">

<img src="_asset/地址限制锥型NAT.png">

<img src="_asset/端口限制锥型NAT.png">

<img src="_asset/对称型NAT.png">

NAT 穿越原理：

- C1，C2 向 STUN 发消息
- 交换公网 IP 及 端口
- C1 -> C2，C2 -> C1，甚至是端口猜测

<img src="_asset/NAT穿越组合.png">

### 9.3 NAT 类型检测

<img src="_asset/NAT类型判断.png">

公网 IP：

<img src="_asset/NAT类型检测-01.png">

如果 Client 收到的 IP 和第一次发出去的 IP 是不一样的，则是对称型 NAT，如果是一样的需要进一步判断：

<img src="_asset/NAT类型检测-02.png">

Client 通过 Port2 发送消息到 STUN Port1，STUN Server 通过 Port2 给 Client 回消息，如果 Client 能收到消息，则说明是 IP 限制型的；如果不能收到，则说明是端口限制型的：

<img src="_asset/NAT类型检测-03.png">

### 9.4 【协议规范】STUN 协议一

STUN 介绍：

- STUN 存在的目的就是进行 NAT 穿越
- STUN 是典型的客户端 / 服务器模式。客户端发送请求，服务端进行响应

RFC STUN 规范：

- **RFC3489/STUN**

  SImple Traversal of UDP Trough NAT

- **RFC5389/STUN** — 包含UDP和TCP

  Session Traversal Utilities for NAT

STUN 协议：

- 包括 20 字节的 STUN header
- Body 中可以有 0 个或多个 Attribute

STUN header（RFC3489）：

- 其中 2 个字节（16bit）类型
- 2 个字节（16bit）消息长度，不包括消息头
- 16 个字节（128bit）事物ID，请求与响应事物 ID 相同

STUN header（RFC5389）格式：

<img src="_asset/STUNHeader格式.png">

<img src="_asset/STUNMessageType.png">

M 代表请求值，C 代表分类：

<img src="_asset/STUNMessageType-01.png">

<img src="_asset/C0C1.png">

RFC5389 把私密类型去掉了：

<img src="_asset/STUN消息类型.png">

### 9.5 【协议规范】STUN 协议二

Inter 机子都是小端模式：

<img src="_asset/大小端模式.png">

<img src="_asset/STUNMessageType-02.png">

<img src="_asset/TransactionID.png">

<img src="_asset/STUNMessageBody.png">

<img src="_asset/TLV.png">

<img src="_asset/RFC3489定义的属性.png">

<img src="_asset/Attribute的使用.png">

### 9.6 【协议规范】TURN 协议

TURN 介绍：

- 其目的是解决对称 NAT 无法穿越的问题
- 其建立在 STUN 之上，消息格式使用 STUN 格式消息
- TURN Client 要求服务端分配一个公共 IP 和 Port 用于接受 或 发送数据

<img src="_asset/TURN例子.png">

<img src="_asset/TURN使用的传输协议.png">

<img src="_asset/TURNAllocate.png">

TURN 发送机制：

- Send 和 Data
- Channel

<img src="_asset/TURNSendAndData.png">

<img src="_asset/TURNChannel.png">

<img src="_asset/TURN的使用.png">

### 9.7 【协议规范】ICE 框架

<img src="_asset/ICE.png">

<img src="_asset/ICECandidate.png">

Candidate 类型：

- 主机候选者
- 反射侯选者
- 中继候选者

ICE 具体做些什么：

- 收集 Candidate
- 对 Candidate Pair 排序
- 连通性检查

<img src="_asset/Candidate关系图.png">

收集 Candidate：

- Host Candidate：本机所有 IP 和指定端口
- Reflexive Candidate：STUN/TURN
- Relay Candidate：TURN

什么是 SDP：

- **SDP（Session Description Protocol）** 它只是一种信息格式的描述标准，本身不属于传输协议，但是可以被其他传输协议用来交换必要的信息。

<img src="_asset/SDP例子.png">

形成 Candidate Pair：

- 一方收集到所有候选者后，通过信令传给对方
- 同样，另一方收到候选者后，也做收集工作
- 当双方拿到全部列表后，将侯选者形成匹配对儿

连通性检查：

- 对侯选者进行优先级排序
- 对每个侯选对进行发送检查
- 对每个侯选对进行接收检查

<img src="_asset/连通性过程.png">

### 9.8 网络协议分析方法 tcpdump 与 wireshark讲解

常用工具：

- Linux 服务端用 tcpdump
- 其它端 WireShark

<img src="_asset/tcpdump.png">

### 9.9 网络协议分析方法 tcpdump 与 wireshark 实战

vim 打开二进制数据：

```shell
：%！xxd
```

WireShark 中的逻辑运算：

- 与：and 或 &&
- 或：or 或 ||
- 非：not 或 ！

WireShark 中判断语句：

- 等于：eq 或 ==
- 小于：lt 或 <
- 大于：gt 或 >
- 小于等于：le 或 <=
- 大于等于：ge 或 >=
- 不等于：ne 或 !=

WireShark 按协议过滤：

- stun
- tcp
- udp

模拟STUN数据可以使用这个网站中的工具：<https://webrtc.github.io/samples>

Wireshark 按 IP 过滤：

```
ip.dst == 192.168.1.2
ip.src == 192.168.1.2
ip.addr == 192.168.1.2
```

WireShark 按 port 过滤：

```
tcp.port == 8080
udp.port == 3478
udp.dstport == 3478
udp.srcport == 3478
```

WireShark 过滤长度：

```
udp.length < 30
tcp.length < 30
http.content_length < 30
```

WireShark 过滤内容：

TODO

## 10. 端对端1V1传输基本流程

### 10.1 媒体能力协商过程

WebRTC 端对端连接：

**RTCPeerConnection**：

- 基本格式

  ```js
  pc = new RTCPeerConnection([configuration]);
  ```

**RTCPeerConnection 方法分类**：

- 媒体协商
- Stream/Track
- 传输相关方法
- 统计相关方法

<img src="_asset/媒体协商过程.png">

<img src="_asset/协商状态变化.png">

**媒体协商方法**：

- createOffer
- createAnswer
- setLocakDescription
- setRemoteDescription

**createOffer**：

- 基本格式

  ```js
  aPromise = myPeerConnection.createOffer([options]);
  ```

**createAnswer**：

- 基本格式

  ```js
  aPromise = myPeerConnection.createAnswer([options]);
  ```

**setLocakDescription**：

- 基本格式

  ```js
  aPromise = myPc.setLocalDescription(sessionDescription);
  ```

**setRemoteDescription**：

- 基本格式

  ```js
  aPromise = myPc.setRemoteDescription(sessionDescription);
  ```

**Track 方法**：

- addTrack
- removeTrack

**addTrack**：

- 基本格式

  ```js
  rtpSender = myPc.addTrack(track, stream...);
  ```

- Parameters

  <img src="_asset/addTrackParameters.png">

**removeTrack**：

- 基本格式

  ```js
  myPc.remoteTrack(rtpSender);
  ```

**重要事件**：

- onnegotiationneeded  - 协商的时候触发这个事件
- onicecandidate - 当收到 ICE 候选者的时候触发这个事件

### 10.2 1:1 连接的基本流程

<img src="_asset/端对端连接的基本流程.png">

**A 与 B 通信，大的方向分为三部分**：

- 媒体协商部分
- ICE 候选者的交换、连接、检测部分
- 媒体数据流的通信部分

### 10.3 【实战】WebRTC 视频传输

TODO

### 10.4 【实战】显示通讯双方的 SDP 内容

TODO

## 11. WebRTC核心之SDP详解

### 11.1 【协议规范】SDP 规范

**SDP 规范**：

- 会话层
- 媒体层

可以把会话层看做树根，媒体层看成树干。

**会话层**：

- 会话的名称与目的
- 会话的存活时间
- 会话中包含多个媒体信息

**SDP 媒体信息**：

- 媒体格式
- 传输协议
- 传输 IP 和 端口
- 媒体负载类型

**SDP 格式**：

- 由多个 `<type>=<value>` 组成
- 一个会话级描述
- 多个媒体级描述

**SDP 结构**：

- Session Description
- Time Description
- Media Description

<img src="_asset/SessionDescription.png">

<img src="_asset/TimeDescription.png">

<img src="_asset/MediaDescription.png">

<img src="_asset/字段含义-01.png">

<img src="_asset/字段含义-02.png">

<img src="_asset/字段含义-03.png">

<img src="_asset/字段含义-04.png">

<img src="_asset/字段含义-05.png">

<img src="_asset/字段含义-06.png">

<img src="_asset/字段含义-07.png">

### 11.2 【协议规范】WebRTC 中的 SDP

<img src="_asset/WebRTC中的SDP.png">

### 11.3 【详解】WebRTC 中 Offer_Answer SDP

```

```

## 12. 实现1V1音视频实时互动直播系统

### 12.1 STUN/TURN 服务器搭建

coTurn Download Address：<https://github.com/coturn/coturn>

ICE 测试地址：<https://webrtc.github.io/samples>

```shell
# 启动 turn server
$ turnserver -c /usr/local/coturn/etc/turnserver.conf
```

<img src="_asset/STUNTURN服务器选型.png">

<img src="_asset/coTurn服务器搭建与部署.png">

<img src="_asset/coTurn服务器配置.png">

<img src="_asset/测试turn服务.png">

### 12.2 【参数介绍】再论 RTCPeerConnection

<img src="_asset/RTCPeerConnection-01.png">

<img src="_asset/Configurations-01.png">

<img src="_asset/Configurations-02.png">

<img src="_asset/Configurations-03.png">

<img src="_asset/Configurations-04.png">

<img src="_asset/addIceCandidate.png">

### 12.3 直播系统中的信令及其逻辑关系

【实战】真正的音视频传输

**客户端信令消息**：

- join 加入房间
- leave 离开房间
- message 端到端消息

**端到端信令消息**：

- Offer 消息
- Answer 消息
- Candidate 消息

**服务端信令消息**：

- joined 已加入房间
- otherjoin 其它用户加入房间
- full 房间人数已满
- leaved 已离开房间
- bye 对方离开房间

<img src="_asset/直播系统消息处理流程.png">

### 12.4 实现 1：1 音视频实时互动信令服务器

信令服务器改造

TODO

### 12.5 再论CreateOffer

<img src="_asset/createOffer.png">

**CreateOffer 实战**：

- 接收远端音频
- 接收远端视频
- 静音检测
- ICE restart

### 12.6 WebRTC 客户端状态机及处理逻辑

直播客户端的实现：

<img src="_asset/客户端状态机.png">

<img src="_asset/客户端流程图.png">

<img src="_asset/客户端流程图-01.png">

<img src="_asset/端对端连接的基本流程.png">

### 12.7 WebRTC 客户端的实现

<img src="_asset/注意要点.png">

### 12.8 共享远程桌面

<img src="_asset/getDisplayMedia-01.png">

<img src="_asset/需要注意的点.png">

## 13. WebRTC核心之RTP媒体控制与数据统计

### 13.1 RTPPReceiver 发送器

RTP Media

<img src="_asset/Receiver和Sender.png">

<img src="_asset/RTCRtpSender属性.png">

<img src="_asset/RTCRtpReceiver.png">

### 13.2 RTPSender 发送器

<img src="_asset/RTCRtpSender.png">

<img src="_asset/RTPMedia.png">

<img src="_asset/RTCRtpTransceiver.png">

### 13.3 传输速率的控制

<img src="_asset/RTPMedia-01.png">

<img src="_asset/chromeWebRTC-internals.png">

chrome WebRTC 状态查询地址：<chrome://webrtc-internals>

### 13.4 【实战】WebRTC统计信息

TODO

## 14. WebRTC非音视频数据传输

### 14.1 传输非音视频数据基础知识

<img src="_asset/createDataChannel.png">

<img src="_asset/option-01.png">

<img src="_asset/option-02.png">

<img src="_asset/使用Options.png">

<img src="_asset/DataChannel事件.png">

<img src="_asset/创建RTCDataChannel.png">

<img src="_asset/非音视频数据传输方式.png">

- Reliability：可靠性
- Delivery：可达性
- Transmission：传输方式
- Flow control：流控
- Congestion control：拥塞控制

### 14.2 端到端文本聊天

TODO

### 14.3 文件实时传输

<img src="_asset/知识点.png">

## 15. WebRTC实时数据传输网络协议详解

### 15.1 【协议规范】RTP-SRTP协议头详解

<img src="_asset/协议栈.png">

<img src="_asset/传输协议.png">

<img src="_asset/RTP协议.png">

<img src="_asset/RTP字段说明.png">

### 15.2 【协议规范】RTCP 中的 SR 与 RR 报文

<img src="_asset/RTCP包.png">

<img src="_asset/RTCPPayloadType.png">

<img src="_asset/RTCPHeader.png">

<img src="_asset/RTCPHeader说明.png">

<img src="_asset/RTCPSenderReport.png">

<img src="_asset/SenderInfomationBlock.png">

<img src="_asset/SenderInfo说明.png">

<img src="_asset/ReportBlock.png">

<img src="_asset/ReceiveReportBlock.png">

<img src="_asset/RTCPReceiverReport.png">

<img src="_asset/RTCPSR-RR发送时机.png">

<img src="_asset/RTCPSDES.png">

<img src="_asset/SDESitem.png">

<img src="_asset/SDES说明.png">

<img src="_asset/RTCPBYE.png">

<img src="_asset/RTCPAPP.png">

<img src="_asset/RTCPAPP字段说明.png">

### 15.3 【协议规范】DTSL

<img src="_asset/DTLS.png">

<img src="_asset/SRTP.png">

### 15.4 wireshark 分析 rtp-rtcp 包

TODO

## 16. Android端与浏览器互通

### 16.1 Android 与浏览器互通

<img src="_asset/主要内容.png">

<img src="_asset/需要权限.png">

<img src="_asset/Android权限管理.png">

<img src="_asset/引入库.png">

<img src="_asset/信令处理.png">

<img src="_asset/AndroidSocketio.png">

<img src="_asset/socketio接收消息.png">

### 16.2 WebRTCNative 开发逻辑

<img src="_asset/结构图.png">

<img src="_asset/呼叫端时序图.png">

<img src="_asset/被叫端时序图.png">

<img src="_asset/关闭时序图.png">

<img src="_asset/webrtc处理流程.png">

<img src="_asset/重要类-01.png">

<img src="_asset/重要类-02.png">

<img src="_asset/两个观察者.png">

### 16.3 实战-权限申请-库的引入与界面

<img src="_asset/权限库界面.png">

### 16.4 实战-通过 socket.io 实现信令收发

<img src="_asset/收发信令.png">

### 16.5 实战-Android 与浏览器互通

创建 PeerConnection：

- 音视频数据采集
- 创建 PeerConnection

媒体能力协商：

- 协商媒体能力
- Candidate 连通
- 视频渲染







## 17. iOS端与浏览器互通

### 17.1 IOS权限获取

<img src="_asset/主要内容-01.png">

<img src="_asset/主要内容-02.png">

### 17.2 IOS引入WebRTC库

<img src="_asset/引入WebRTC库的方式.png">

<img src="_asset/引入WebRTC库.png">

<img src="_asset/Podfile.png">

### 17.3 IOS端SocketIO的使用

<img src="_asset/socketio的使用.png">

<img src="_asset/连接服务器.png">

<img src="_asset/发送消息.png">

<img src="_asset/注册侦听消息.png">

### 17.4 IOS界面布局

TODO

### 17.5 IOS本地视频采集与展示

TODO

### 17.6 IOS端RTCPeerConnection

TODO

### 17.7 IOS媒体协商

<img src="_asset/媒体协商.png">

<img src="_asset/信令时序图.png">

### 17.8 IOS远端视频渲染

<img src="_asset/RTCPeerConnection委托.png">

## 18. 课程总结

<img src="_asset/小结.png">

<img src="_asset/信令服务器.png">

<img src="_asset/JS客户端实现.png">

<img src="_asset/JS客户端实现-01.png">

<img src="_asset/进阶.png">

<img src="_asset/进阶-01.png">

<img src="_asset/行业痛点.png">

## Reference

> [JavaScript 是如何工作的:WebRTC 和对等网络的机制！](<https://juejin.im/post/5c511219e51d45522c3066c9>)
>
> [深入理解WebRTC](https://segmentfault.com/a/1190000011403597)
>
> [WebRTC架构简介](https://blog.csdn.net/fishmai/article/details/69681595)
>
> [HTTPS证书生成原理和部署细节](<https://www.barretlee.com/blog/2015/10/05/how-to-build-a-https-server/>)
>
> [SSL证书生成流程](<https://www.jianshu.com/p/9091ebd439a0>)

> [Gradle官网](<https://gradle.org/install/>)
>
> [Gradle 包](<http://tools.android-studio.org/index.php/9-tools/109-android-tools-download>)
>
> [Mac下AndroidStudio中手动配置Gradle](<https://www.jianshu.com/p/36e569c1bb12>)



WebRTC的分层协议图：

<img src="_asset/webrtc分层协议图.png">

信令，会话和协议：

<img src="_asset/信令会话协议.png">

<img src="_asset/">

<img src="_asset/">

## 问题解决里程

node 启动 server 报错：

```shell
events.js:141
      throw er; // Unhandled 'error' event
      ^

Error: listen EACCES 0.0.0.0:443
    at Object.exports._errnoException (util.js:870:11)
    at exports._exceptionWithHostPort (util.js:893:20)
    at Server._listen2 (net.js:1224:19)
    at listen (net.js:1273:10)
    at net.js:1382:9
    at nextTickCallbackWith3Args (node.js:452:9)
    at process._tickCallback (node.js:358:17)
    at Function.Module.runMain (module.js:444:11)
    at startup (node.js:136:18)
    at node.js:966:3
[Solve]$ sudo setcap 'cap_net_bind_service=+ep' $(readlink -f $(which node))
```

查询端口是否别占用：

```shell
$ netstat -ntpl | grep 443
```

## VIM 快捷键温习

> [Vim 插件 tern_for_vim Javascript 自动补全](<https://wxnacy.com/2017/09/22/vim-plugin-tern/>)
>
> [最全的vim快捷键](<http://www.cnblogs.com/jiqingwu/archive/2012/06/14/vim_notes.html#id107>)

vim 格式化文本，调整缩进：

> = 是格式化文本的快捷方法， 当你发现代码缩进的不整齐的，可以用这个快速对齐它们。
>
> 直接按＝号就可以，不能进入命令模式（以 “:" 冒号开始的命令行）。
>
> gg=G ：从头格到尾，爽。
>
> ＝＝ 格式化一行
>
> 如要格式化一段代码，可以先选中这些代码，再按＝号。
>
> 调整缩进还有个比较慢的办法，按＞＞，向右缩进一格，＜＜向左缩进一格。

vim常用快捷键总结：

- 光标移动到行首：0

- 光标移动到行尾：$

- 光标移动到文件开始：GG

- 光标移动到文件末尾：shift +G

- 先前翻页：Ctrl+f

- 向后翻页：Ctrl+b

- 删至行首：d0

- 删至行尾：d$

- 删除当前行及其后面n-1行：ndd

- 删除当前字符：x

- 删除当前字符的前一个字符：X

- 删除当前字符：dl

- 删除到第三个字符的结尾位置：d3w

- 删除到某个单词的末尾：dw

- 删除到某个单词的开始：db

- 删除当前行到文件的末尾：dG

- 删除当前行到文件第一行：dH

- 删除知道屏幕上最后一行：dL

- 替换当前行所有temp为hehe：:s/temp/hehe/g

- 替换每行中第一个#include为hehe：:%s/#include/hehe/

- 替换每行中所有的#include为hehe：:%s/#include/hehe/g

- 替换第n行开始到最后一行中每一行的第一个#include为hehe：:n,$s/#include/hehe/

- 替换第n行开始到最后一行中每一行的所有#include为hehe：:n,$s/#include/hehe/g

- 替换当前行到末尾的所有#include为hehe：:.,$s/#include/hehe/g

- 替换正文中所有出现的#include为hehe：:1,$s/#include/hehe/g

- 回复上一步操作：u

- 全部回复操作：shift + u

- 重做上一步操作：Ctrl + r

- 把下一行合并到当前行尾：J

- 选中当前行及其后面的n-1行：nV

- 复制当前光标到此单词末尾：yw

- 批量添加注释：Ctrl+v可视模式，上、下、左、右移动光标选择若干行开头；Shift+i进入插入模式；输入// 者；按Esc

- 批量去掉注释：Ctrl+v可视模式，上、下、左、右移动关闭选择要删除的注释符，如//或#；按d删除

- 横向打开另一个窗口：:sp 文件名

- 竖向打开另一个窗口：:vsp 文件名

- 关闭当前打开的所有窗口：:qa

- 选择当前字符所在的单词：Ctrl+v可视模式，然后a+w或i+w

- 选择当前字符所在的段落：Ctrl+v可视模视，然后i+p或a+p



