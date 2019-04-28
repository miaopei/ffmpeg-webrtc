# WebRTC

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



















## 7. WebRTC音视频录制实战



## 8. WebRTC信令服务器实现



## 9. WebRTC网络基础补充：P2P/STUN/TRUN/ICE知识



## 10. 端对端1V1传输基本流程



## 11. WebRTC核心之SDP详解



## 12. 实现1V1音视频实时互动直播系统



## 13. WebRTC核心之RTP媒体控制与数据统计



## 14. WebRTC非音视频数据传输



## 15. WebRTC实时数据传输网络协议详解



## 16. Android端与浏览器互通



## 17. iOS端与浏览器互通



## 18. 课程总结



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



WebRTC的分层协议图：

<img src="_asset/webrtc分层协议图.png">

信令，会话和协议：

<img src="_asset/信令会话协议.png">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

<img src="_asset/">

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





