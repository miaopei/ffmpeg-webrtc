'use strict'

var localVideo = document.querySelector('video#localVideo');
var remoteVideo = document.querySelector('video#remoteVideo');
var btnStart = document.querySelector('button#start');
var btnCall = document.querySelector('button#call');
var btnHangUp= document.querySelector('button#hangup');


var localStream;
var pc1;
var pc2;

function gotMediaStream(stream){
	localVideo.srcObject = stream;
	localStream = stream;
}

function handleError(err){
	console.log("Failed to call getUserMedia", err);
}

function start(){
	var constraints = {
		video: true,
		audio: false 
	}

	if(!navigator.mediaDevices ||
		!navigator.mediaDevices.getUserMedia){
		return;
	}else {
		navigator.mediaDevices.getUserMedia(constraints)
					.then(gotMediaStream)
					.catch(handleError);
	}

}

function gotAnswerDescription(desc){
	pc2.setLocalDescription(desc);

	//send sdp to caller
	//recieve sdp from callee
	
	pc1.setRemoteDescription(desc);

}

function gotLocalDescription(desc){
	pc1.setLocalDescription(desc);

	//send sdp to callee
	
	//receive sdp from caller 
 	pc2.setRemoteDescription(desc);	
	pc2.createAnswer().then(gotAnswerDescription)
			 .catch(handleError);
}

function gotRemoteStream(e){

	if(remoteVideo.srcObject !== e.streams[0]){
		remoteVideo.srcObject = e.streams[0];
	}
}

function call(){
	var offerOptions = {
		offerToReceiveAudio: 0,
		offerToReceiveVideo: 1 
	}

	pc1 = new RTCPeerConnection();
	pc1.onicecandidate = (e) => {
	
		// send candidate to peer
		// receive candidate from peer

		pc2.addIceCandidate(e.candidate)
			.catch(handleError);
		console.log('pc1 ICE candidate:', e.candidate);
	}

	pc1.iceconnectionstatechange = (e) => {
		console.log(`pc1 ICE state: ${pc.iceConnectionState}`);
		console.log('ICE state change event: ', e);
	}


	pc2 = new RTCPeerConnection();
	pc2.onicecandidate = (e)=> {
	
		// send candidate to peer
		// receive candidate from peer

		pc1.addIceCandidate(e.candidate)
			.catch(handleError);
		console.log('pc2 ICE candidate:', e.candidate);
	}

	pc2.iceconnectionstatechange = (e) => {
		console.log(`pc2 ICE state: ${pc.iceConnectionState}`);
		console.log('ICE state change event: ', e);
	}

	pc2.ontrack = gotRemoteStream;

	//add Stream to caller
	localStream.getTracks().forEach((track)=>{
		pc1.addTrack(track, localStream);
	});

	pc1.createOffer(offerOptions)
		.then(gotLocalDescription)
		.catch(handleError);

}


function hangup(){
	pc1.close();
	pc2.close();
	pc1 = null;
	pc2 = null;

}

btnStart.onclick = start;
btnCall.onclick = call;
btnHangUp.onclick = hangup;

