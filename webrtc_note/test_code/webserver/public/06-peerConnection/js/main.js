'use strict'

// 本机内的 1:1 音视频互通

var localVideo = document.querySelector('video#localvideo');
var remoteVideo = document.querySelector('video#remotevideo');

var btnStart = document.querySelector('button#start');
var btnCall = document.querySelector('button#call');
var btnHangup = document.querySelector('button#hangup');

var offer = document.querySelector('textarea#offer');
var answer = document.querySelector('textarea#answer');

var localStream;
var pc1;
var pc2;

function getMediaStream(stream)
{
    localVideo.srcObject = stream;
    localStream = stream;
}

function handleError(err)
{
    console.error('Failed to get Media Stream: ', err.name);
}

function start()
{
    if(!navigator.mediaDevices ||
		!navigator.mediaDevices.getUserMedia){
		console.error('getUserMedia is not supported!');
		return;
	} else {
		//var deviceId = videoSource.value; 
        var constraints = {
			video : {
				width: 640,	
				height: 480,
				frameRate:15,
				facingMode: 'enviroment'
				//deviceId : deviceId ? {exact:deviceId} : undefined 
			}, 
            audio : {
                noiseSuppression: true,
                echoCancellation: true
            } 
		}

		navigator.mediaDevices.getUserMedia(constraints)
			.then(getMediaStream)
			.catch(handleError);
	}

}

function getRemoteStream(e)
{
    remoteVideo.srcObject = e.streams[0];
}

function getAnswer(desc)
{
    pc2.setLocalDescription(desc);
    answer.value = desc.sdp;

    // send desc to signal
    // receive desc from signal
    
    pc1.setRemoteDescription(desc);
}

function handleAnswerError(err)
{
    console.error('Failed to create answer: ', err);  
}

function getOffer(desc)
{
    pc1.setLocalDescription(desc);
    offer.value = desc.sdp;

    // send desc to signal
    // receive desc from signal
    
    pc2.setRemoteDescription(desc);

    pc2.createAnswer()
            .then(getAnswer)
            .catch(handleAnswerError);
}

function handleOfferError(err)
{
    console.error('Failed to create offer: ', err);  
}

function call()
{
    pc1 = new RTCPeerConnection();
    pc2 = new RTCPeerConnection();

    pc1.onicecandidate = (e)=>{
        pc2.addIceCandidate(e.candidate);
    }

    pc2.onicecandidate = (e)=>{
        pc1.addIceCandidate(e.candidate);
    }

    pc2.ontrack = getRemoteStream;

    // 先添加数据，再做媒体协商，这样对端才能知道你的媒体信息
    localStream.getTracks().forEach((track)=>{
        pc1.addTrack(track, localStream);
    });

    var offerOptions = {
        offerToRecieveAudio: 1,
        offerToRecieveVideo: 1
    }
    pc1.createOffer(offerOptions)
            .then(getOffer)
            .catch(handleOfferError);
}

function hangup()
{
    pc1.close();
    pc2.close();
    pc1 = null;
    pc2 = null;
}

btnStart.onclick = start;
btnCall.onclick = call;
btnHangup.onclick = hangup;

