const http = require('http');
var options = {
  host: '192.168.43.86',
  path: '/'
};

setInterval(function(){
  var req = http.get(options, function(res) {
    //console.log('STATUS: ' + res.statusCode);
    //console.log('HEADERS: ' + JSON.stringify(res.headers));

    // Buffer the body entirely for processing as a whole.
    var stream = [];
    res.on('data', function(byte) {
      stream.push(byte);
    }).on('end', function() {
      var body = Buffer.concat(stream);
      if(res.statusCode ==200) console.log('BODY: ' + body);
    })
  });

  req.on('error', function(e) {
    console.log('ERROR: ' + e.message);
  });
}, 100);