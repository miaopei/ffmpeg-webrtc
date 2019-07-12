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

//div
var divContraints = document.querySelector('div#constraints')

//recored
var recVideo = document.querySelector('video#recplayer')
var btnRecord = document.querySelector('button#record')
var btnPlay = document.querySelector('button#recplay')
var btnDownload = document.querySelector('button#download')

var buffer;
var mediaRecorder;

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
    var videoTrack = stream.getVideoTracks()[0];
    var videoConstraints = videoTrack.getSettings();
    divContraints.textContent = JSON.stringify(videoConstraints, null, 2);

    window.stream = stream;
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

function handleDataAvailable(e)
{
    if(e && e.data && e.data.size > 0) {
        buffer.push(e.data);
    }
}

function startRecord()
{
    buffer = [];

    var options = {
        mimeType: 'video/webm;codecs=vp8'
    }

    if(!MediaRecorder.isTypeSupported(options.mimeType)) {
        console.log(`${options.mimeType} is not supported!`);
        return;
    }
    
    try{
        mediaRecorder = new MediaRecorder(window.stream, options);
    } catch(e){
        console.error('Failed to create MediaRecorder:', e);
        return;
    }

    mediaRecorder.ondataavailable = handleDataAvailable;
    mediaRecorder.start(10);
}

function stopRecord()
{
    mediaRecorder.stop();   
}

btnRecord.onclick = ()=> {
    if(btnRecord.textContent === 'Start Record') {
        startRecord();
        btnRecord.textContent = 'Stop Record';
        btnPlay.disabled = true;
        btnDownload.disabled = true;
    } else {
        stopRecord();
        btnRecord.textContent = 'Start Record';
        btnPlay.disabled = false;
        btnDownload.disabled = false;
    }
}

btnPlay.onclick = ()=> {
    var blob = new Blob(buffer, {type: 'video/webm'});
    recVideo.src = window.URL.createObjectURL(blob);
    recVideo.srcObject = null;
    recVideo.controls = true;
    recVideo.play();
}

btnDownload.onclick = ()=> {
    var blob = new Blob(buffer, {type: 'video/webm'});
    var url = window.URL.createObjectURL(blob);
    var a = document.createElement('a');

    a.href = url;
    a.style.display = 'none';
    a.download = 'aaa.webm';
    a.click();
}


