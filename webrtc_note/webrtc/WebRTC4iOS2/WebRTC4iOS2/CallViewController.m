//
//  CallViewController.m
//  WebRTC4iOS2
//
//  Created by garrylea on 2019/4/29.
//  Copyright © 2019年 avdance. All rights reserved.
//

#import "CallViewController.h"
#import "SignalClient.h"

#import <WebRTC/WebRTC.h>
#import <MBProgressHUD/MBProgressHUD.h>

@interface CallViewController() <SignalEventNotify, RTCPeerConnectionDelegate, RTCVideoViewDelegate>
{
    
    NSString* myAddr;
    NSString* myRoom;
    
    NSString* myState;
    
    SignalClient* sigclient;
    
    RTCPeerConnectionFactory* factory;
    RTCCameraVideoCapturer* capture;
    //RTCMediaStream* localStream; //???
    RTCPeerConnection* peerConnection;
    
    RTCVideoTrack* videoTrack;
    RTCAudioTrack* audioTrack;
    
    RTCVideoTrack* remoteVideoTrack;
    CGSize remoteVideoSize;
    
    NSMutableArray* ICEServers;

}

@property (strong, nonatomic) RTCEAGLVideoView *remoteVideoView;
@property (strong, nonatomic) RTCCameraPreviewView *localVideoView;

@property (strong, nonatomic) UIButton* leaveBtn;


@property (strong, nonatomic) dispatch_source_t timer;

@end

@implementation CallViewController

static CGFloat const kLocalVideoViewSize = 120;
static CGFloat const kLocalVideoViewPadding = 8;

//static NSString *const RTCSTUNServerURL = @"stun:stun.l.google.com:19302";
static NSString *const RTCSTUNServerURL = @"turn:xxx.xxx.xxx:3478";
static int logY = 0;
- (instancetype) initAddr:(NSString*) addr withRoom:(NSString*) room {
    
    myAddr = addr;
    myRoom = room;

    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    logY = 0;
    CGRect bounds = self.view.bounds;
    
    self.remoteVideoView = [[RTCEAGLVideoView alloc] initWithFrame:self.view.bounds];
    self.remoteVideoView.delegate = self;
    //[self.remoteVideoView set]
    //[self.remoteVideoView setBackgroundColor:[UIColor yellowColor]];
    [self.view addSubview:self.remoteVideoView];
    
    self.localVideoView = [[RTCCameraPreviewView alloc] initWithFrame:CGRectZero];
    [self.view addSubview:self.localVideoView];
    
    // Aspect fit local video view into a square box.
    CGRect localVideoFrame =
    CGRectMake(0, 0, kLocalVideoViewSize, kLocalVideoViewSize);
    // Place the view in the bottom right.
    localVideoFrame.origin.x = CGRectGetMaxX(bounds)
    - localVideoFrame.size.width - kLocalVideoViewPadding;
    localVideoFrame.origin.y = CGRectGetMaxY(bounds)
    - localVideoFrame.size.height - kLocalVideoViewPadding;
    [self.localVideoView setFrame: localVideoFrame];

    self.leaveBtn = [[UIButton alloc] init];
    [self.leaveBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [self.leaveBtn setTintColor:[UIColor whiteColor]];
    [self.leaveBtn setTitle:@"leave" forState:UIControlStateNormal];
    [self.leaveBtn setBackgroundColor:[UIColor greenColor]];
    [self.leaveBtn setShowsTouchWhenHighlighted:YES];
    [self.leaveBtn.layer setCornerRadius:40];
    [self.leaveBtn.layer setBorderWidth:1];
    [self.leaveBtn setClipsToBounds:FALSE];
    [self.leaveBtn setFrame:CGRectMake(self.view.bounds.size.width/2-40,
                                       self.view.bounds.size.height-140,
                                       80,
                                       80)];
    
    [self.leaveBtn addTarget:self
                      action:@selector(leaveRoom:)
            forControlEvents:UIControlEventTouchUpInside];
    
    [self.view addSubview:self.leaveBtn];
    
    [self createPeerConnectionFactory];
    
    //[self startTimer];
    
    //创建本地流
    [self captureLocalMedia];

    sigclient = [SignalClient getInstance];
    sigclient.delegate = self;
    ////[sigclient createConnect:myAddr];
    
    myState = @"init";
    
}

-(void)addLogToScreen:(NSString *)format, ...{
    
    va_list paramList;
    va_start(paramList,format);
    NSString* log = [[NSString alloc]initWithFormat:format arguments:paramList];
    va_end(paramList);
    
    CGRect labelRect = CGRectMake(0, logY++ * 20, 500, 200);
    UILabel *label = [[UILabel alloc] initWithFrame:labelRect];
    label.text = log;
    label.textColor = [UIColor redColor];
    [self.view addSubview:label];
}

//- (void) startTimer {
//    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
//    dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);
//    self.timer = timer;
//    dispatch_source_set_timer(timer, DISPATCH_TIME_NOW, 200 * NSEC_PER_SEC, 0 * NSEC_PER_SEC);
//
//    __weak NSString* weakMyState = myState;
//    __weak NSString* weakMyRoom = myRoom;
//    dispatch_source_set_event_handler(timer, ^{
//
//        dispatch_
//
//        if([weakMyState isEqualToString:@"init"]) {
//            NSLog(@"%@",[NSThread currentThread]);
//            [[SignalClient getInstance] joinRoom:weakMyRoom];
//        }else {
//            [timer ]
//        }
//    });
//    dispatch_resume(timer);
//}

- (void)layoutSubviews {
    CGRect bounds = self.view.bounds;
    if (remoteVideoSize.width > 0 && remoteVideoSize.height > 0) {
        // Aspect fill remote video into bounds.
        CGRect remoteVideoFrame =
        AVMakeRectWithAspectRatioInsideRect(remoteVideoSize, bounds);
        CGFloat scale = 1;
        if (remoteVideoFrame.size.width > remoteVideoFrame.size.height) {
            // Scale by height.
            scale = bounds.size.height / remoteVideoFrame.size.height;
        } else {
            // Scale by width.
            scale = bounds.size.width / remoteVideoFrame.size.width;
        }
        remoteVideoFrame.size.height *= scale;
        remoteVideoFrame.size.width *= scale;
        self.remoteVideoView.frame = remoteVideoFrame;
        self.remoteVideoView.center =
        CGPointMake(CGRectGetMidX(bounds), CGRectGetMidY(bounds));
    } else {
        self.remoteVideoView.frame = bounds;
    }

}

- (void) leaveRoom:(UIButton*) sender {
   
    [self willMoveToParentViewController:nil];
    [self.view removeFromSuperview];
    [self removeFromParentViewController];
    
    if (!sigclient) {
        sigclient = [SignalClient getInstance];
    }
    
    if(![myState isEqualToString:@"leaved"]){
        [sigclient leaveRoom: myRoom];
    }
    
    if(peerConnection){
        [peerConnection close];
        peerConnection = nil;
    }
    
    NSLog(@"leave room(%@)", myRoom);
    [self addLogToScreen: @"leave room(%@)", myRoom];
}

#pragma mark - SignalEventNotify

- (void) leaved:(NSString *)room {
    NSLog(@"leaved room(%@) notify!", room);
    [self addLogToScreen: @"leaved room(%@) notify!", room];
}

- (void) joined:(NSString *)room {
    NSLog(@"joined room(%@) notify!", room);
    [self addLogToScreen: @"joined room(%@) notify!", room];
    
    myState = @"joined";
    
    //这里应该创建PeerConnection
    if (!peerConnection) {
        peerConnection = [self createPeerConnection];
    }
}

- (void) otherjoin:(NSString *)room User:(NSString *)uid {
    NSLog(@"other user(%@) has been joined into room(%@) notify!", uid, room);
    [self addLogToScreen: @"other user(%@) has been joined into room(%@) notify!", uid, room];
    if([myState isEqualToString:@"joined_unbind"]){
        if (!peerConnection) {
            peerConnection = [self createPeerConnection];
        }
    }
    
    myState =@"joined_conn";
    //调用call， 进行媒体协商
    [self doStartCall];
    
}

- (void) full:(NSString *)room {
    NSLog(@"the room(%@) is full notify!", room);
    [self addLogToScreen: @"the room(%@) is full notify!", room];
    myState = @"leaved";
    
    if(peerConnection) {
        [peerConnection close];
        peerConnection = nil;
    }
    
    //弹出提醒添加成功
    MBProgressHUD *hud= [[MBProgressHUD alloc] initWithView:self.view];
    [hud setRemoveFromSuperViewOnHide:YES];
    hud.label.text = @"房间已满";
    UIView* view = [[UIView alloc] initWithFrame:CGRectMake(0,0, 50, 50)];
    [hud setCustomView:view];
    [hud setMode:MBProgressHUDModeCustomView];
    [self.view addSubview:hud];
    [hud showAnimated:YES];
    [hud hideAnimated:YES afterDelay:1.0]; //设置1秒钟后自动消失
    
    if(self.localVideoView) {
        //[self.localVideoView removeFromSuperview];
        //self.localVideoView = nil;
    }
    
    if(self.remoteVideoView) {
        //[self.localVideoView removeFromSuperview];
        //self.remoteVideoView = nil;
    }
    
    if(capture) {
        [capture stopCapture];
        capture = nil;
    }
    
    if(factory) {
        factory = nil;
    }
}

- (void) byeFrom:(NSString *)room User:(NSString *)uid {
    NSLog(@"the user(%@) has leaved from room(%@) notify!", uid, room);
    [self addLogToScreen: @"the user(%@) has leaved from room(%@) notify!", uid, room];
    myState = @"joined_unbind";
    
    [peerConnection close];
    peerConnection = nil;
    
}

- (void) answer:(NSString *)room message:(NSDictionary *)dict {
    NSLog(@"have received a answer message %@", dict);
    
    NSString *remoteAnswerSdp = dict[@"sdp"];
    RTCSessionDescription *remoteSdp = [[RTCSessionDescription alloc]
                                           initWithType:RTCSdpTypeAnswer
                                                    sdp: remoteAnswerSdp];
    [peerConnection setRemoteDescription:remoteSdp
                       completionHandler:^(NSError * _Nullable error) {
        if(!error){
            NSLog(@"Success to set remote Answer SDP");
        }else{
            NSLog(@"Failure to set remote Answer SDP, err=%@", error);
        }
    }];
}

- (void) setLocalAnswer: (RTCPeerConnection*)pc withSdp: (RTCSessionDescription*)sdp {
    
    [pc setLocalDescription:sdp completionHandler:^(NSError * _Nullable error) {
        if(!error){
            NSLog(@"Successed to set local answer!");
        }else {
            NSLog(@"Failed to set local answer, err=%@", error);
        }
    }];
    
    __weak NSString* weakMyRoom = myRoom;
    dispatch_async(dispatch_get_main_queue(), ^{
        
        //send answer sdp
        NSDictionary* dict = [[NSDictionary alloc] initWithObjects:@[@"answer", sdp.sdp]
                                                           forKeys: @[@"type", @"sdp"]];
        
        [[SignalClient getInstance] sendMessage: weakMyRoom withMsg:dict];
    });
}

- (void) getAnswer:(RTCPeerConnection*) pc {
    
    NSLog(@"Success to set remote offer SDP");
    
    [pc answerForConstraints:[self defaultPeerConnContraints]
                           completionHandler:^(RTCSessionDescription * _Nullable sdp, NSError * _Nullable error) {
                               if(!error){
                                   NSLog(@"Success to create local answer sdp!");
                                   __weak RTCPeerConnection* weakPeerConn = pc;
                                   [self setLocalAnswer:weakPeerConn withSdp:sdp];
                                   
                               }else{
                                   NSLog(@"Failure to create local answer sdp!");
                               }
                           }];
}

- (void) offer:(NSString *)room message:(NSDictionary *)dict {
    NSLog(@"have received a offer message %@", dict);
    
    NSString* remoteOfferSdp = dict[@"sdp"];
    RTCSessionDescription* remoteSdp = [[RTCSessionDescription alloc]
                                        initWithType:RTCSdpTypeOffer
                                        sdp: remoteOfferSdp];
    if(!peerConnection){
        peerConnection = [self createPeerConnection];
    }
    
     __weak RTCPeerConnection* weakPeerConnection = peerConnection;
    [weakPeerConnection setRemoteDescription:remoteSdp completionHandler:^(NSError * _Nullable error) {
        if(!error){
            [self getAnswer: weakPeerConnection];
        }else{
            NSLog(@"Failure to set remote offer SDP, err=%@", error);
        }
    }];
}

- (void) candidate:(NSString *)room message:(NSDictionary *)dict {
    NSLog(@"have received a message %@", dict);
    
    NSString* desc = dict[@"sdp"];
    NSString* sdpMLineIndex = dict[@"label"];
    int index = [sdpMLineIndex intValue];
    NSString* sdpMid = dict[@"id"];
    
    
    RTCIceCandidate *candidate = [[RTCIceCandidate alloc] initWithSdp:desc
                                                        sdpMLineIndex:index
                                                               sdpMid:sdpMid];;
    [peerConnection addIceCandidate:candidate];
}

- (void)connected {
    [[SignalClient getInstance]  joinRoom: myRoom];
    [self addLogToScreen: @"socket connect success!"];
    [self addLogToScreen: @"joinRoom: %@", myRoom];
    

}

- (void)connect_error {
    //todo: notfiy UI
    [self addLogToScreen: @"socket connect_error!"];
}

- (void)connect_timeout {
    //todo: notfiy UI
    [self addLogToScreen: @"socket connect_timeout!"];
}

- (void) reconnectAttempt{
    [self addLogToScreen: @"socket reconnectAttempt!"];
}
#pragma mark RTCPeerConnectionDelegate

/** Called when the SignalingState changed. */
- (void)peerConnection:(RTCPeerConnection *)peerConnection
didChangeSignalingState:(RTCSignalingState)stateChanged{
    NSLog(@"%s",__func__);
}

/** Called when media is received on a new stream from remote peer. */
- (void)peerConnection:(RTCPeerConnection *)peerConnection didAddStream:(RTCMediaStream *)stream{
    NSLog(@"%s",__func__);
}

/** Called when a remote peer closes a stream.
 *  This is not called when RTCSdpSemanticsUnifiedPlan is specified.
 */
- (void)peerConnection:(RTCPeerConnection *)peerConnection didRemoveStream:(RTCMediaStream *)stream{
     NSLog(@"%s",__func__);
}

/** Called when negotiation is needed, for example ICE has restarted. */
- (void)peerConnectionShouldNegotiate:(RTCPeerConnection *)peerConnection {
    NSLog(@"%s",__func__);
}

/** Called any time the IceConnectionState changes. */
- (void)peerConnection:(RTCPeerConnection *)peerConnection
didChangeIceConnectionState:(RTCIceConnectionState)newState{
    NSLog(@"%s",__func__);
}

/** Called any time the IceGatheringState changes. */
- (void)peerConnection:(RTCPeerConnection *)peerConnection
didChangeIceGatheringState:(RTCIceGatheringState)newState{
    NSLog(@"%s",__func__);
}

/** New ice candidate has been found. */
- (void)peerConnection:(RTCPeerConnection *)peerConnection
didGenerateIceCandidate:(RTCIceCandidate *)candidate{
    NSLog(@"%s",__func__);
    
    NSString* weakMyRoom = myRoom;
    dispatch_async(dispatch_get_main_queue(), ^{
    
        NSDictionary* dict = [[NSDictionary alloc] initWithObjects:@[@"candidate",
                                                                [NSString stringWithFormat:@"%d", candidate.sdpMLineIndex],
                                                                candidate.sdpMid,
                                                                candidate.sdp]
                                                           forKeys:@[@"type", @"label", @"id", @"candidate"]];
    
        [[SignalClient getInstance] sendMessage: weakMyRoom
                                    withMsg:dict];
    });
}

/** Called when a group of local Ice candidates have been removed. */
- (void)peerConnection:(RTCPeerConnection *)peerConnection
didRemoveIceCandidates:(NSArray<RTCIceCandidate *> *)candidates {
    NSLog(@"%s",__func__);
}

/** New data channel has been opened. */
- (void)peerConnection:(RTCPeerConnection *)peerConnection
    didOpenDataChannel:(RTCDataChannel *)dataChannel {
    NSLog(@"%s",__func__);
}

- (void)peerConnection:(RTCPeerConnection *)peerConnection
        didAddReceiver:(RTCRtpReceiver *)rtpReceiver
               streams:(NSArray<RTCMediaStream *> *)mediaStreams{
    NSLog(@"%s",__func__);
    
    RTCMediaStreamTrack* track = rtpReceiver.track;
    if([track.kind isEqualToString:kRTCMediaStreamTrackKindVideo]){
   
        if(!self.remoteVideoView){
            NSLog(@"error:remoteVideoView have not been created!");
            return;
        }
        
        remoteVideoTrack = (RTCVideoTrack*)track;
        
        //dispatch_async(dispatch_get_main_queue(), ^{

            [remoteVideoTrack addRenderer: self.remoteVideoView];
        //});
        //[remoteVideoTrack setIsEnabled:true];
        
        //[self.view addSubview:self.remoteVideoView];
    }
    
}

#pragma mark webrtc


- (RTCMediaConstraints*) defaultPeerConnContraints {
    RTCMediaConstraints* mediaConstraints =
    [[RTCMediaConstraints alloc] initWithMandatoryConstraints:@{
                                                                kRTCMediaConstraintsOfferToReceiveAudio:kRTCMediaConstraintsValueTrue,
                                                                kRTCMediaConstraintsOfferToReceiveVideo:kRTCMediaConstraintsValueTrue
                                                                }
                                          optionalConstraints:@{ @"DtlsSrtpKeyAgreement" : @"true" }];
    return mediaConstraints;
}


- (void) captureLocalMedia {
    
    NSDictionary* mandatoryConstraints = @{};
    RTCMediaConstraints* constraints =
    [[RTCMediaConstraints alloc] initWithMandatoryConstraints:mandatoryConstraints
                                          optionalConstraints:nil];
    
    RTCAudioSource* audioSource = [factory audioSourceWithConstraints: constraints];
    //self.audioTrack = [factory audioTrackWithTrackId:@"ARDAMSa0"];
    audioTrack = [factory audioTrackWithSource:audioSource trackId:@"ADRAMSa0"];
    
    NSArray<AVCaptureDevice* >* captureDevices = [RTCCameraVideoCapturer captureDevices];
    AVCaptureDevicePosition position = AVCaptureDevicePositionFront;
    AVCaptureDevice* device = captureDevices[0];
    for (AVCaptureDevice* obj in captureDevices) {
        if (obj.position == position) {
            device = obj;
            break;
        }
    }
    
    //检测摄像头权限
    AVAuthorizationStatus authStatus = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    if(authStatus == AVAuthorizationStatusRestricted || authStatus == AVAuthorizationStatusDenied)
    {
        NSLog(@"相机访问受限");
        
        //弹出提醒添加成功
        MBProgressHUD *hud= [[MBProgressHUD alloc] initWithView:self.view];
        [hud setRemoveFromSuperViewOnHide:YES];
        hud.label.text = @"没有权限访问相机";
        UIView* view = [[UIView alloc] initWithFrame:CGRectMake(0,0, 50, 50)];
        [hud setCustomView:view];
        [hud setMode:MBProgressHUDModeCustomView];
        [self.view addSubview:hud];
        [hud showAnimated:YES];
        [hud hideAnimated:YES afterDelay:1.0]; //设置1秒钟后自动消失
        
        return;
    }
    
    if (device)
    {
        RTCVideoSource* videoSource = [factory videoSource];
        capture = [[RTCCameraVideoCapturer alloc] initWithDelegate:videoSource];
        AVCaptureDeviceFormat* format = [[RTCCameraVideoCapturer supportedFormatsForDevice:device] lastObject];
        CGFloat fps = [[format videoSupportedFrameRateRanges] firstObject].maxFrameRate;
        videoTrack = [factory videoTrackWithSource:videoSource trackId:@"ARDAMSv0"];
        self.localVideoView.captureSession = capture.captureSession;
        [capture startCaptureWithDevice:device
                                     format:format
                                        fps:fps];
        
    }
    
}

//初始化STUN Server （ICE Server）
- (RTCIceServer *)defaultSTUNServer {
    return [[RTCIceServer alloc] initWithURLStrings:@[RTCSTUNServerURL]
                                           username:@"xxx"
                                         credential:@"xxx"];
}

- (void) createPeerConnectionFactory {
    
    //设置SSL传输
    [RTCPeerConnectionFactory initialize];
    
    //如果点对点工厂为空
    if (!factory)
    {
        RTCDefaultVideoDecoderFactory* decoderFactory = [[RTCDefaultVideoDecoderFactory alloc] init];
        RTCDefaultVideoEncoderFactory* encoderFactory = [[RTCDefaultVideoEncoderFactory alloc] init];
        NSArray* codecs = [encoderFactory supportedCodecs];
        [encoderFactory setPreferredCodec:codecs[2]];
        
        factory = [[RTCPeerConnectionFactory alloc] initWithEncoderFactory: encoderFactory
                                                            decoderFactory: decoderFactory];
        //factory = [[RTCPeerConnectionFactory alloc] init];
    }
}

- (RTCPeerConnection *)createPeerConnection {
    
    //得到ICEServer
    if (!ICEServers) {
        ICEServers = [NSMutableArray array];
        [ICEServers addObject:[self defaultSTUNServer]];
    }

    //用工厂来创建连接
    RTCConfiguration* configuration = [[RTCConfiguration alloc] init];
    [configuration setIceServers:ICEServers];
    RTCPeerConnection* conn = [factory
                                     peerConnectionWithConfiguration:configuration
                                                         constraints:[self defaultPeerConnContraints]
                                                            delegate:self];

    
    NSArray<NSString*>* mediaStreamLabels = @[@"ARDAMS"];
    [conn addTrack:videoTrack streamIds:mediaStreamLabels];
    [conn addTrack:audioTrack streamIds:mediaStreamLabels];

    return conn;
}

- (void)setLocalOffer:(RTCPeerConnection*)pc withSdp:(RTCSessionDescription*) sdp{
    
    [pc setLocalDescription:sdp completionHandler:^(NSError * _Nullable error) {
        if (!error) {
            NSLog(@"Successed to set local offer sdp!");
        }else{
            NSLog(@"Failed to set local offer sdp, err=%@", error);
        }
    }];
    
    __weak NSString* weakMyRoom = myRoom;
    dispatch_async(dispatch_get_main_queue(), ^{
        
        NSDictionary* dict = [[NSDictionary alloc] initWithObjects:@[@"offer", sdp.sdp]
                                                           forKeys: @[@"type", @"sdp"]];
        
        [[SignalClient getInstance] sendMessage: weakMyRoom
                                        withMsg: dict];
    });
}

- (void) doStartCall {
    NSLog(@"Start Call, Wait ...");
    [self addLogToScreen: @"Start Call, Wait ..."];
    if (!peerConnection) {
        peerConnection = [self createPeerConnection];
    }
    
    [peerConnection offerForConstraints:[self defaultPeerConnContraints]
                      completionHandler:^(RTCSessionDescription * _Nullable sdp, NSError * _Nullable error) {
                          if(error){
                              NSLog(@"Failed to create offer SDP, err=%@", error);
                          } else {
                              __weak RTCPeerConnection* weakPeerConnction = self->peerConnection;
                              [self setLocalOffer: weakPeerConnction withSdp: sdp];
                          }
                      }];
}

#pragma mark - RTCEAGLVideoViewDelegate
- (void)videoView:(RTCEAGLVideoView*)videoView didChangeVideoSize:(CGSize)size {
    if (videoView == self.remoteVideoView) {
        remoteVideoSize = size;
    }
    [self layoutSubviews];
}

@end
