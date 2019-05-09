const express = require('express');
const app = express();
const server = require('http').Server(app);
const path = require("path");
const websocket = require('ws');
const websocketServer = new websocket.Server({ server });

var packet = 0;

app.use(express.static(path.join(__dirname, '/public')));
app.get('/', function(req, res) {
  res.sendFile(__dirname + '/index.html');
});


websocketServer.on('connection',function(ws,req){
  ws.on('message', function(message){
    //console.log("Received: "+message);
    packet++;
    // websocketServer.clients.forEach(function(client){ //broadcast incoming message to all clients (s.clients)
    //   if(client!=ws && client.readyState ){ //except to the same client (ws) that sent this message
    //     client.send("broadcast: " +message);
    //   }
    // });
    serialPort.write(message + "\r\n");
  });
  ws.on('close', function(){
    console.log("Client disconnected");
  });
  console.log("new client connected");
});
server.listen(3000);

setInterval(function(){
  console.log("pps: " + packet);
  packet = 0;
}, 1000);




const portName = "COM1";
const portBaudRate = 115200;
const SerialPort = require('serialport');
const parsers = SerialPort.parsers;

const serialParser = new parsers.Readline({
  delimiter: '\r\n'
});
const serialPort = new SerialPort(portName, {
  baudRate: portBaudRate
});
serialPort.on('open', serialPort_open);
serialPort.pipe(serialParser);
serialParser.on('data', serialPort_data);
serialPort.on('close', serialPort_close);
serialPort.on('error', serialPort_error);

function serialPort_open() {
  process.stdout.write("SERIAL Port Open. Baud Rate: " + portBaudRate + "\r\n");
}

function serialPort_data(data) {
  console.log("[DATA] " + data);
}

function serialPort_close() {
  console.log('SERIAL Port Closed');
}
function serialPort_error(error) {
  console.log('SERIAL Port ' + error);
}