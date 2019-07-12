//
//  ViewController.m
//  WebRTC4iOS2
//
//  Created by garrylea on 2019/4/28.
//  Copyright © 2019年 avdance. All rights reserved.
//

#import "ViewController.h"
#import "CallViewController.h"
#import "SignalClient.h"

@interface ViewController ()
{
    SignalClient *sigclient;
}

@property (strong, nonatomic) UILabel* addrLabel;
@property (strong, nonatomic) UITextField* addr;

@property (strong, nonatomic) UILabel* roomLabel;
@property (strong, nonatomic) UITextField* room;

@property (strong, nonatomic) UIButton* joinBtn;

@property (strong, nonatomic) CallViewController* call;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.

    CGFloat width = self.view.bounds.size.width;
    
    self.addrLabel = [[UILabel alloc] init];
    [self.addrLabel setText:@"ADDR:"];
    [self.addrLabel setFrame:CGRectMake(20, 100, 60, 40)];
    [self.view addSubview:self.addrLabel];
    
    self.addr = [[UITextField alloc] init];
    [self.addr setText:@"https://learningrtc.cn"];
    [self.addr setFrame:CGRectMake(80, 100, width-100, 40)];
    [self.addr setTextColor:[UIColor blackColor]];
    [self.addr setBorderStyle:UITextBorderStyleRoundedRect];
    [self.addr setEnabled:TRUE];
    [self.view addSubview:self.addr];
    
    self.roomLabel = [[UILabel alloc] init];
    [self.roomLabel setText:@"ROOM:"];
    [self.roomLabel setFrame:CGRectMake(20, 150, 60, 40)];
    [self.view addSubview:self.roomLabel];
    
    self.room = [[UITextField alloc] init];
    [self.room setText:@"111111"];
    [self.room setFrame:CGRectMake(80, 150, width-100, 40)];
    [self.room setTextColor:[UIColor blackColor]];
    [self.room setBorderStyle:UITextBorderStyleRoundedRect];
    [self.room setEnabled:TRUE];
    [self.view addSubview:self.room];
    
    self.joinBtn = [[UIButton alloc] init];
    [self.joinBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [self.joinBtn setTintColor:[UIColor whiteColor]];
    [self.joinBtn setTitle:@"join" forState:UIControlStateNormal];
    [self.joinBtn setBackgroundColor:[UIColor grayColor]];
    [self.joinBtn setShowsTouchWhenHighlighted:YES];
    [self.joinBtn setFrame:CGRectMake(40, 200, width-80, 40)];
    
    [self.joinBtn addTarget:self action:@selector(btnClick:) forControlEvents:UIControlEventTouchUpInside];
    
    [self.view addSubview:self.joinBtn];
    
//    self.call = [[CallViewController alloc] initAddr:self.addr.text withRoom: self.room.text];
//    [self.call.view setFrame:self.view.bounds];
//    [self.call.view setBackgroundColor:[UIColor whiteColor]];
    
//    NSURL* url = [[NSURL alloc] initWithString:@"http://localhost:8080"];
//    SocketManager* manager = [[SocketManager alloc] initWithSocketURL:url config:@{@"log": @YES, @"compress": @YES}];
//    SocketIOClient* socket = manager.defaultSocket;
//
//    [socket on:@"connect" callback:^(NSArray* data, SocketAckEmitter* ack) {
//        NSLog(@"socket connected");
//    }];
//
//    [socket on:@"currentAmount" callback:^(NSArray* data, SocketAckEmitter* ack) {
//        double cur = [[data objectAtIndex:0] floatValue];
//
//        [[socket emitWithAck:@"canUpdate" with:@[@(cur)]] timingOutAfter:0 callback:^(NSArray* data) {
//            [socket emit:@"update" with:@[@{@"amount": @(cur + 2.50)}]];
//        }];
//
//        [ack with:@[@"Got your currentAmount, ", @"dude"]];
//    }];
//
//    [socket connect];
}

/** 点击空白处回收键盘 */
- (void)touchesBegan:(NSSet<UITouch *> *)touches   withEvent:(UIEvent *)event
{
    [self.view endEditing:YES];
}

- (void)btnClick:(UIButton*)sender{
    NSLog(@"on click!");

    self.call = [[CallViewController alloc] initAddr:self.addr.text withRoom: self.room.text];
    [self.call.view setFrame:self.view.bounds];
    [self.call.view setBackgroundColor:[UIColor whiteColor]];

    [self addChildViewController:self.call];
    [self.call didMoveToParentViewController:self];
    
    [self.view addSubview:self.call.view];
    
    [[SignalClient getInstance] createConnect: self.addr.text];
}

#pragma mark protocal EventNotify
- (void) leave {
//    [self.view removeFromSuperview];
    
}


@end
