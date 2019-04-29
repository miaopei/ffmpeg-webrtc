'use strict'

//devices
var audioSource = document.querySelector('select#audioSource');
var audioOutput = document.querySelector('select#audioOutput');
var videoSource = document.querySelector('select#videoSource');

//filter
var filtersSelect = document.querySelector('select#filter');


var snapshot = document.querySelector('button#snapshot');
var picture = document.querySelector('canvas#picture');
picture.width = 640;
picture.height = 480;

var videoplay = document.querySelector('video#player');
function gotDevices(deviceInfos)
{
    deviceInfos.forEach( function(deviceinfo) {
        var option = document.createElement('option');
        option.text = deviceinfo.label;
        option.value = deviceinfo.deviceId;

        if(deviceinfo.kind === 'audioinput') {
			      audioSource.appendChild(option);
        } else if (deviceinfo.kind === 'audiooutput') {
            audioOutput.appendChild(option);
        } else if (deviceinfo.kind === 'videoinput') {
			      videoSource.appendChild(option);
        }
    })
}

function gotMediaStream(stream)
{
    //console.log('onRemoteStream stream', stream);
    videoplay.srcObject = stream;
	return navigator.mediaDevices.enumerateDevices();
}

function handleError(err)
{
    console.log('getUserMedia error:', err.name);
}

function start() 
{
	if(!navigator.mediaDevices ||
		!navigator.mediaDevices.getUserMedia){
		console.log('getUserMedia is not supported!');
		return;
	} else {
		var deviceId = videoSource.value; 
		var constraints = {
			video : {
				width: 640,	
				height: 480,
				frameRate:15,
				facingMode: 'enviroment',
				deviceId : deviceId ? {exact:deviceId} : undefined 
			}, 
            audio : {
                noiseSuppression: true,
                echoCancellation: true
            } 
		}

		navigator.mediaDevices.getUserMedia(constraints)
			.then(gotMediaStream)
			.then(gotDevices)
			.catch(handleError);
	}
}

start();

videoSource.onchange = start;

filtersSelect.onchange = function(){
	videoplay.className = filtersSelect.value;
}

snapshot.onclick = function() {
	picture.className = filtersSelect.value;
	picture.getContext('2d').drawImage(videoplay, 0, 0, picture.width, picture.height);
}

