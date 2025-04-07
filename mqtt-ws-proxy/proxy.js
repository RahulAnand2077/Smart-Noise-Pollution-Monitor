const aedes = require('aedes')();
const net = require('net');
const ws = require('ws');
const http = require('http');

const mqttPort = 1883;
const wsPort = 9001;

// Start MQTT server
const mqttServer = net.createServer(aedes.handle);
mqttServer.listen(mqttPort, () => {
  console.log(`🚀 MQTT server listening on port ${mqttPort}`);
});

// Start WebSocket server
const httpServer = http.createServer();
const wsServer = new ws.Server({ server: httpServer });

wsServer.on('connection', (client) => {
  const duplex = ws.createWebSocketStream(client);
  aedes.handle(duplex);
});

httpServer.listen(wsPort, () => {
  console.log(`🌐 WebSocket server listening on ws://localhost:${wsPort}`);
});
