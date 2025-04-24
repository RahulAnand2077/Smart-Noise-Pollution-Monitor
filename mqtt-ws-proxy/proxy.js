const aedes = require('aedes')(); // MQTT broker
const net = require('net'); // create tcp server
const ws = require('ws'); // websocket 
const http = require('http'); // host wss

const mqttPort = 1883;
const wsPort = 9001;

// Start MQTT server
const mqttServer = net.createServer(aedes.handle);
mqttServer.listen(mqttPort, () => {
  console.log(`🚀 MQTT server listening on port ${mqttPort}`); // esp32 connected it mangaes
});

// Start WebSocket server
const httpServer = http.createServer();
const wsServer = new ws.Server({ server: httpServer });

wsServer.on('connection', (client) => {
  const duplex = ws.createWebSocketStream(client);
  aedes.handle(duplex); // duplex created which tells mqtt to work like a tcp
});

httpServer.listen(wsPort, () => {
  console.log(`🌐 WebSocket server listening on ws://localhost:${wsPort}`);
});
