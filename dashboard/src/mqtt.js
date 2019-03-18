var mqtt = require("mqtt");

var options = {
  hostname: "broker.hivemq.com",
  port: 8000,
  path: "/mqtt",
  clientId: "iot-m1miage-ab",
  connectTimeout: 5000
};

var client = mqtt.connect(options);

console.log(client);

export default {
  publish: (topic, message) => {
    client.publish(topic, message);
    console.log("Message publié sur le topic " + topic);
    console.log(message);
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
