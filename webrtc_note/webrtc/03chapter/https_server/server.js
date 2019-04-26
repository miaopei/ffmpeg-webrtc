'use strict'

var https = require('https');
var fs = require('fs');

var options = {
  key  : fs.readFileSync('./cert/1557605_www.learningrtc.cn.key'),
  cert : fs.readFileSync('./cert/1557605_www.learningrtc.cn.pem')
}

var app = https.createServer(options, function(req, res){
	res.writeHead(200, {'Content-Type': 'text/plain'});
	res.end('HTTPS:Hello World!\n');


}).listen(443, '0.0.0.0');
