'use strict'

var audioInput = document.querySelector('select#audioInput');
var audioOutput = document.querySelector('select#audioOutput');
var videoInput = document.querySelector('select#videoInput');

var filterSelect = document.querySelector('select#filter');

var videoplay = document.querySelector('video#palyer');

var snapshot = document.querySelector('button#snapshot');
var picture = document.querySelector('canvas#picture');
picture.width = 320;
picture.height = 240;

function gotDevices(deviceInfos)
{
    deviceInfos.forEach( function(deviceInfo) {
        console.log(deviceInfo.kind + ": label = "
                    + deviceInfo.label + ": id = "
                    + deviceInfo.deviceId + ": groupId = "
                    + deviceInfo.groupId);

        var option = document.createElement('option');
        option.text = deviceInfo.label;
        option.value = deviceInfo.deviceId;

        if(deviceInfo.kind === 'audioinput') {
            audioInput.appendChild(option);
        } else if (deviceInfo.kind === 'audiooutput') {
            audioOutput.appendChild(option);
        } else if (deviceInfo.kind === 'videoinput') {
            videoInput.appendChild(option);
        }
    });
}

function gotMediaStream(stream)
{
    //console.log('onRemoteStream stream', stream);
    videoplay.srcObject = stream;
    return navigator.mediaDevices.enumrateDevices();
}

function handleError(err)
{
    console.log('getUserMedia error:', err.name);
}

function start()
{
    if(!navigator.mediaDevices || !navigator.mediaDevices.getUserMedia) {
        console.log('enumerateDevices is not supported!');
        return;
    } else {
        var deviceId = videoInput.value;
        var constrants = {
            video: {
                width: 640,
                height: 480,
                frameRate: 30,
                deviceId: deviceId ? deviceId : undefined
            },
            audio: {
                noiseSuppression: true,
                echoCancellation: true
            },
        };

        navigator.mediaDevices.getUserMedia(constrants)
            .then(gotMediaStream)
            .then(gotDevices)
            .catch(handleError);
    }
}

start();

videoInput.onchange = start;

filterSelect.onchange = function() {
    videoplay.className = filterSelect.value;
}

snapshot.onclick = function() {
    picture.getContext('2d').drawImage(videoplay, 
                                       0, 0,
                                       picture.width,
                                       picture.height)
}

