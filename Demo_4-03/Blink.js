var express = require('express.io')
  , fs = require('fs')
  , SerialPort = require('serialport').SerialPort;
/*
  SerialPort.list(function (err, ports) {
    ports.forEach(function(port) {
      console.log(port.comName);
      console.log(port.pnpId);
      console.log(port.manufacturer);
    });
  });
*/
var app = express().http().io();

app.get('/', function(req, res) {
    res.sendfile(__dirname + '/index.html')
});
//app.use(express.static(__dirname + '/tabmenu'));

app.listen(80);

var sp = new SerialPort("/dev/ttyACM0", {
  baudrate: 9600
});

sp.on("open", function () {
  console.log('open');
  sp.on('data', function(data) {
    console.log('data received: ' + data);
  });
});


app.io.sockets.on('connection',function(socket){
  console.log('connection established');

  socket.on('message',function(data){
    console.log('text received: '+data);
    sp.write(data);
  });

  socket.on('Rled',function(data){
    console.log('command received: blink');
    sp.write('Red\n');
  });
  socket.on('Gled',function(data){
    console.log('command received: blink');
    sp.write('Green\n');
  });
  socket.on('Bled',function(data){
    console.log('command received: blink');
    sp.write('Blue\n');
  });
  socket.on('game1_control',function(data){
    //send the command to the game's js file
    console.log("game1_control = " + data.value)
    sp.write(0x50,function() {
	sp.write(data.value,function() {
    		sp.write(0x60)
	});
    });
    
  });
});
