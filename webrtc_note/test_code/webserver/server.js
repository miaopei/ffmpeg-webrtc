'use strict'

var http = require('http');
var https = require('https');
var fs = require('fs');

var express = require('express');
var serverIndex = require('serve-index');

var socketIo = require('socket.io');

var log4js = require('log4js');

log4js.configure({
    appenders: {
        file: {
            type: 'file',
            filename: 'app.log',
            layout: {
                type: 'pattern',
                pattern: '%r %p - %m',
            }
        }
    },
    categories: {
        default: {
            appenders: ['file'],
            level: 'debug'
        }
    }
});

var logger = log4js.getLogger();

var app = express();
app.use(serverIndex('./public'));
app.use(express.static('./public'));

var http_server = http.createServer(app);
http_server.listen(80, '0.0.0.0');

var options = {
    key  : fs.readFileSync('./cert/server.key'),
    cert : fs.readFileSync('./cert/server.pem')
}

var https_server = https.createServer(options, app);

// bind socket.io with https_server
var io = socketIo.listen(https_server);

// connection
io.sockets.on('connection', (socket)=>{
    socket.on('join', (room)=>{
        socket.join(room);
        var myRoom = io.sockets.adapter.rooms[room];
        var users = Object.keys(myRoom.sockets).length;
        logger.log('The number of user in room is:' + users);
        //socket.emit('joined', room, socket.id);
        //socket.to(room).emit('joined', room, socket.id); // 除自己之外
        //io.in(room).emit('joined', room, socket.id); // 房间内所有人
        socket.broadcast.emit('joined', room, socket.id); // 除自己，全部站点
    });

    socket.on('leave', (room)=>{
        var myRoom = io.sockets.adapter.rooms[room];
        var users = Object.keys(myRoom.sockets).length;
        // users - 1

        socket.leave(room);
        logger.log('The number of user in room is:' + (users-1));
        //socket.emit('leaved', room, socket.id);
        //socket.to(room).emit('leaved', room, socket.id); // 除自己之外
        //io.in(room).emit('leaved', room, socket.id); // 房间内所有人
        socket.broadcast.emit('leaved', room, socket.id); // 除自己，全部站点
    })

})

https_server.listen(443, '0.0.0.0');

