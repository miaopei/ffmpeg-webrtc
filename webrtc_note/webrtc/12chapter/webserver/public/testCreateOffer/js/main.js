'use strict'


var start = document.querySelector('button#start');
var restart = document.querySelector('button#restart');

var pc1 = new RTCPeerConnection();
var pc2 = new RTCPeerConnection();

function handleError(err){

	console.log('Failed to create offer', err);
}

function getPc1Answer(desc){

	console.log('getPc1Answer', desc.sdp);
	pc2.setLocalDescription(desc);
	pc1.setRemoteDescription(desc);

	/*
	pc2.createOffer({offerToRecieveAudio:1, offerToReceiveVideo:1})
		.then(getPc2Offer)
		.catch(handleError);
		*/
}

function getPc1Offer(desc){

	console.log('getPc1Offer', desc.sdp);
	pc1.setLocalDescription(desc);
	pc2.setRemoteDescription(desc);
	pc2.createAnswer().then(getPc1Answer).catch(handleError);

}

function getPc2Answer(desc){
	console.log('getPc2Answer');

	pc1.setLocalDescription(desc);
	pc2.setRemoteDescription(desc);
}

function getPc2Offer(desc){

	console.log('getPc2Offer');
	pc2.setLocalDescription(desc);
	pc1.setRemoteDescription(desc);
	pc1.createAnswer().then(getPc2Answer).catch(handleError);

}

function startTest(){


	pc1.createOffer({offerToReceiveAudio:1, offerToRecieveVideo:1})
		.then(getPc1Offer)
		.catch(handleError);




}

function getMediaStream(stream){

	stream.getTracks().forEach((track) => {
		pc1.addTrack(track, stream);	
	});

	var offerConstraints = {
		offerToReceiveAudio: 1,
		offerToRecieveVideo: 1,
		iceRestart:false 
	}

	pc1.createOffer(offerConstraints)
		.then(getPc1Offer)
		.catch(handleError);

}

function startICE(){

	var constraints = {
		audio: true,
		video: true
	}

	navigator.mediaDevices.getUserMedia(constraints)
				.then(getMediaStream)
				.catch(handleError);
}

start.onclick = startTest;
restart.onclick = startICE;
