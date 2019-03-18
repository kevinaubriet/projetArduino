var mqtt = require("mqtt");

var mqttPort = 443;
var mqttHost = "r8ovuz.messaging.internetofthings.ibmcloud.com";

var clientId = "d:r8ovuz:my_device_type:my_first_device";
var apiKey = "use-token-auth";
var apiToken = "bSJEO3dL??ufc0Bz@2";

const initMqttUrl = "m1/miage/ab";

var options = {
  hostname: mqttHost,
  port: mqttPort,
  clientId: clientId,
  username: apiKey,
  password: apiToken,
  connectTimeout: 5000
};

var options2 = {
  hostname: "broker.hivemq.com",
  port: 8000,
  path: "/mqtt",
  clientId: "iot-m1miage-ab",
  connectTimeout: 5000
};

var client = mqtt.connect(options2);

console.log(client);

export default {
  publish: (topic, message) => {
    client.publish(topic, JSON.stringify(message));
    console.log("publié");
  },
  subscribe: (topic, options = {}) => {
    client.subscribe(topic, options);
  },
  receive: callback => {
    client.on("message", function(topic, message) {
      callback(topic, JSON.parse(message.toString()));
    });
  }
};
/*

client.on("connect", function() {
  client.subscribe("presence", function(err) {
    if (!err) {
      client.publish("presence", "Hello mqtt");
    }
  });
});

client.on("message", function(topic, message) {
  // message is Buffer
  console.log(message.toString());
  client.end();
});
*/
