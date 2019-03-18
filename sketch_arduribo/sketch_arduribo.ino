#include "OneWire.h"
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>

#define mqtt_server "broker.hivemq.com"

#define led_topic "m1/miage/ab/controleLed" //Topic led

#define temperature_topic "m1/miage/ab/temperature" //Topic température
#define lumiere_topic "m1/miage/ab/lumiere" //Topic lumiere

#define identifiant_wifi "HUAWEI P20"
#define mdp_wifi "12345678"

const char* url_serv_name = "http://localhost:8081/api";
const char* url_serv_name_test = "http://httpbin.org/ip";

const int ledPin = 19;
const int TempPin = 23;

char message_buff[100];

  //HTTP
  HTTPClient serv;
WiFiClient espClient;
PubSubClient client(espClient);

int lumValue;
float tempValue;
OneWire oneWire(TempPin) ; // Pour utiliser une entite oneWire sur le port 22
DallasTemperature tempSensor(&oneWire) ; // Cette entite est utilisee par le capteur

void setup(){ 
  Serial.begin(9600);
  while (!Serial); // wait for a serial connection
  
  pinMode(ledPin , OUTPUT) ;
  tempSensor.begin(); // Init du capteur et de l’entite OneWire

  connect_wifi();

    // MQTT
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback); 

  /*
  serv.begin(url_serv_name);
  int httpCode = serv.GET();
  if (httpCode > 0) { 

      Serial.print("Received data ...");
      String Contents = serv.getString();
      
      Serial.print(httpCode);//200
      Serial.print("\n");
      
      Serial.print(Contents);//json objet
      Serial.print("\n");

      serv.end(); // End connection
   }
   else {
      Serial.println("Error on HTTP request");
      Serial.println(httpCode);
      String Contents = serv.getString();
      Serial.print(Contents);//json objet
      Serial.print("\n");
      
   }*/
}

void loop(){
 
  int32_t period = 5000; // 5 sec

  /*--- Publish Datas periodically   */
  delay(period);


  //récup valeur des capteurs
  getTemperature();
  getLumiere();

  //conversion en objet JSON
  String bufTemp;
  bufTemp += F("{ \"valeur\":");
  bufTemp += String(tempValue, 2);
  bufTemp += F(", \"type\": \"temperature\" }"); 

  char copyTemp[50];
  bufTemp.toCharArray(copyTemp, 50);
  
  String bufLum;
  bufLum += F("{ \"valeur\":");
  bufLum += lumValue;
  bufLum += F(", \"type\": \"lumiere\" }");
  
  char copyLum[50];
  bufLum.toCharArray(copyLum, 50);

  if (!client.connected()) { 
    mqtt_mysubscribe((char *)(led_topic));
  }
  
  // MQTT Publish
  mqtt_publish(temperature_topic, copyTemp);
  mqtt_publish(lumiere_topic, copyLum);

  client.loop(); // Process MQTT ... une fois par loop()
  

}

void getTemperature(){
    tempSensor.requestTemperaturesByIndex(0); // Le capteur 0 realise une acquisition
    tempValue = tempSensor.getTempCByIndex(0); // On transfert le float qui correspond a temp acquise
}

void getLumiere(){
    lumValue = analogRead(A0) ; // read analog input pin 0
}


// Déclenche les actions à la réception d'un message
void callback(char* topic, byte* payload, unsigned int length) {

  // Callback if a message is published on this topic.
  
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");

  // Byte list to String and print to Serial
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic,
  // you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == led_topic) {
    Serial.print("Changing output to ");
    if (messageTemp == "true") {
      Serial.println("on");
      set_LED(HIGH);
    }
    else if (messageTemp == "false") {
      Serial.println("off");
      set_LED(LOW);
    }
  }
}

void set_LED(int v){
  digitalWrite(ledPin,v);
}

void print_ip_status(){
  Serial.print("WiFi connected \n");
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.print("\n");
  Serial.print("MAC address: ");
  Serial.print(WiFi.macAddress());
  Serial.print("\n");  
}

void connect_wifi(){
 const char* ssid = identifiant_wifi;
 const char *password= mdp_wifi; 
 Serial.println("--------------------------------------------");
 Serial.println("\nConnecting Wifi...");
 WiFi.begin(ssid, password);
 while(WiFi.status() != WL_CONNECTED){
   Serial.print("Attempting to connect ..\n");
   delay(1000);
 }
 Serial.print("Connected to local Wifi\n");  
 print_ip_status();
 Serial.println("--------------------------------------------");
}

void mqtt_publish(char *topic, char *message){

  if(client.connect("esp32", "try", "try")){
    client.publish(topic, message);
    // Serial info
    Serial.print("Published datas : "); 
    Serial.println(message);
  }else{
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    // Wait 5 seconds before retrying
    delay(5000);
  }
}

void mqtt_mysubscribe(char *topic) {
  while (!client.connected()) { // Loop until we're reconnected
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("esp32", "try", "try")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
