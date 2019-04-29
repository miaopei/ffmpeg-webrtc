'use strict'

var userName = document.querySelector('input#username');
var inputRoom = document.querySelector('input#room');
var btnConnect = document.querySelector('button#connect');
var outputArea = document.querySelector('textarea#output');
var inputArea = document.querySelector('textarea#input');
var btnSend = document.querySelector('button#send');

var socket;
var room;

btnConnect.onclick = ()=>{
    // connect
    socket = io.connect();

    // recieve message
    socket.on('joined', (room, id) => {
        btnConnect.disabled = true;
        inputArea.disabled = false;
        btnSend.disabled = false;
    });
    socket.on('leaved', (room, id) => {
        btnConnect.disabled = false;
        inputArea.disabled = true;
        btnSend.disabled = true;
    });
    socket.on('message', (room, id, data) => {
        outputArea.value = outputArea.value + data + '\r';
    });

    // send message
    room = inputRoom.value;
    socket.emit('join', room);
}

btnSend.onclick = ()=> {
    var data = inputArea.value;
	data = userName.value + ':' + data;
    socket.emit('message', room, data);
    inputArea.value = '';
}

