'use strict'

var videoplay = document.querySelector("video#palyer");

function gotMediaStream(stream)
{
    videoplay.srcObject = stream;
}

function handleError(err)
{
    console.log('getUserMedia error:', err);
}

if(!navigator.mediaDevices || !navigator.mediaDevices.getUserMedia) {
    console.log('enumerateDevices is not supported!');
} else {
    var constrants = {
        video : true,
        audio : true
    };

    navigator.mediaDevices.getUserMedia(constrants)
        .then(gotMediaStream)
        .catch(handleError);
}

