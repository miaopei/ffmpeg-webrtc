'use strict'

var http = require('http');

var app = http.createServer(function(req, res) {
    res.writeHead(200, {'Content-Type':'text/plain'});
    res.end('Hello Nodejs\n');
}).listen(8888, '0.0.0.0');
