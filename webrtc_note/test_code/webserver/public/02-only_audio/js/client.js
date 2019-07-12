'use strict'

//devices
var audioSource = document.querySelector('select#audioSource');
var audioOutput = document.querySelector('select#audioOutput');
var videoSource = document.querySelector('select#videoSource');

var audioplay = document.querySelector('audio#audioplayer');

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
    audioplay.srcObject = stream;
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
		var constraints = {
			video : false,
            audio : true
        }

		navigator.mediaDevices.getUserMedia(constraints)
			.then(gotMediaStream)
			.then(gotDevices)
			.catch(handleError);
	}
}

start();

