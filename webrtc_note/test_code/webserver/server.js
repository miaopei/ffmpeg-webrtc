'use strict'

var http = require('http');
var https = require('https');
var fd = require('fs');

var express = require('express');
var serverIndex = require('server-index');

var app = express();
var http_server = http.createServer(app);
http_server.listen(80, '0.0.0.0');

var options = {
    key  : fs.readFileSync('./cert/server.key'),
    cert : fs.readFileSync('./cert/server.pem')
}

var https_server = https.createServer(options. app);
https_server.listen(443, '0.0.0.0');

