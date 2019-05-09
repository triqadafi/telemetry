const express = require('express');
const app = express();
const bodyParser = require('body-parser');
const server = require('http').Server(app);
const path = require('path');
const os = require('os');


// app.use(bodyParser.json())
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname, '/public')));

var interfaces = os.networkInterfaces();
var addresses = [];
for (var k in interfaces) {
    for (var k2 in interfaces[k]) {
        var address = interfaces[k][k2];
        if (address.family === 'IPv4' && !address.internal) {
            addresses.push(address.address);
        }
    }
}
console.log(addresses);


server.listen(80, function() {
	var host = server.address().address
	var port = server.address().port
	console.log("Server listening on %s:%s...", host, port);
});

app.get('/', function (req, res) {
  res.sendFile(__dirname + '/index.html');
});

app.post('/accel', function(req, res) {
  var accelX = req.body.x;
  res.end(null);

  serialPort.write(accelX + "\r\n");
});



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

