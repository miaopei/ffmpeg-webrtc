'use strict'

var createOffer = document.querySelector('button#createOffer');

var pc1 = new RTCPeerConnection();
var pc2 = new RTCPeerConnection();

function handleError(err){
	console.log('Failed to create offer', err);
}

function getPcAnswer(desc){
	console.log('Answer', desc.sdp);
	pc2.setLocalDescription(desc);
	pc1.setRemoteDescription(desc);
}

function getPcOffer(desc){
	console.log('Offer', desc.sdp);
	pc1.setLocalDescription(desc);
	pc2.setRemoteDescription(desc);
	pc2.createAnswer().then(getPcAnswer).catch(handleError);
}

function getMediaStream(stream){
	stream.getTracks().forEach((track) => {
		pc1.addTrack(track, stream);	
	});

	var offerConstraints = {
		offerToReceiveAudio: 1,
		offerToRecieveVideo: 1,
		iceRestart: true
	}

	pc1.createOffer(offerConstraints)
		.then(getPcOffer)
		.catch(handleError);
}

function getStream(){
	var constraints = {
		audio: true,
		video: true
	}

	navigator.mediaDevices.getUserMedia(constraints)
				.then(getMediaStream)
				.catch(handleError);
}

function test() {
    if(!pc) {
        console.error('pc is null!');
        return;
    }

    getStream();

    return;
}

createOffer.onclick = test;

