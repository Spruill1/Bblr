var app = require('http').createServer(handler)
  , io = require('socket.io').listen(app)
  , fs = require('fs')
  , five = require("johnny-five"),
  board,Rled,Gled,Bled;

board = new five.Board();

board.on("ready",function() {
  Rled = new five.Led(5);
  Gled = new five.Led(6);
  Bled = new five.Led(7);
})

app.listen(80);

// handle web server
function handler (req, res) {
  fs.readFile(__dirname + '/index.html',
  function (err, data) {
    if (err) {
      res.writeHead(500);
      return res.end('Error loading index.html');
    }

    res.writeHead(200);
    res.end(data);
  });
}


io.sockets.on('connection',function(socket){
  console.log('connection established');

  socket.on('Rled',function(data){
    console.log('command received: blink');
    Rled.on();

    board.wait(1000,function(){Rled.off();});
  });
  socket.on('Gled',function(data){
    console.log('command received: blink');
    Gled.on();

    board.wait(1000,function(){Gled.off();});
  });
  socket.on('Bled',function(data){
    console.log('command received: blink');
    Bled.on();

    board.wait(1000,function(){Bled.off();});
  });
});
