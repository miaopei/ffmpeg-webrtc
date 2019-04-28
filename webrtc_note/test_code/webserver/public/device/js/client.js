'use strict'

var audioInput = document.querySelector("select#audioInput");
var audioOutput  = document.querySelector("select#audioOutput");
var videoInput = document.querySelector("select#videoInput");

if(!navigator.mediaDevices || !navigator.mediaDevices.enumerateDevices) {
    console.log('enumerateDevices is not supported!');
} else {
    navigator.mediaDevices.enumerateDevices()
        .then(gotDevices)
        .catch(handleError);
}

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

function handleError(err) 
{
    console.log(err.name + " : " + err.message);
}
