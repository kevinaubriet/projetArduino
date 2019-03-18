#include "OneWire.h"
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>

#define mqtt_server "broker.hivemq.com"

#define led_topic "m1/miage/ab/test3" //Topic température

#define temperature_topic "m1/miage/ab/temperature"

const int lumSwitch = 1000;
const float tempSwitch = 24.0;
const int ledPin = 19;

char message_buff[100];
long lastMsg = 0;   //Horodatage du dernier message publié sur MQTT
long lastRecu = 0;
bool debug = false;  //Affiche sur la console si True

WiFiClient espClient;
PubSubClient client(espClient);

int lumValue;
float tempValue;
OneWire oneWire(23) ; // Pour utiliser une entite oneWire sur le port 22
DallasTemperature tempSensor(&oneWire) ; // Cette entite est utilisee par le capteur

void setup(){ 
  Serial.begin(9600);
  while (!Serial); // wait for a serial connection
  
  pinMode(ledPin , OUTPUT) ;
  tempSensor.begin(); // Init du capteur et de l’entite OneWire

  connect_wifi();
      
 
 
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback);
  Serial.println("je suis connecté");

  //client.subscribe(led_topic);
 

  
 //"{ \"valeur\": \"30\", \"_id": \"00000\" }"
    
 

/*
 // Armement du timer en micro sec
esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP ∗ us_TO_S_FACTOR) ;
// Deep sleep mode
esp_deep_sleep_start( ) ;
*/

 
}

void loop(){
  
int32_t period = 5000; // 5 sec
  
  /*--- subscribe to TOPIC_TEMPERATURE if not yet ! */
  /*if (!client.connected()) { 
    mqtt_mysubscribe((char *)(TOPIC_TEMP));
  }*/

  /*--- Publish Temperature periodically   */
  delay(period);
  //temperature = get_Temperature();
  // Convert the value to a char array
  //temperature = temperature + 1;
  /*char tempString[8];
  dtostrf(temperature, 1, 2, tempString);*/

  getTemperature();

  
  String bufTemp;
  bufTemp += F("{ \"valeur\":");
  bufTemp += String(tempValue, 2);
  bufTemp += F(", \"type\": \"temperature\" }");
  

  char copyTemp[50];
  bufTemp.toCharArray(copyTemp, 50);



  //String obj =  "{ \"valeur\":"+ tempValue +", \"_id\": 0 }";
  // MQTT Publish
  mqtt_publish(temperature_topic, copyTemp);
   
  client.loop(); // Process MQTT ... une fois par loop()
  
  /*
  // TEMPERATURE  
  tempSensor.requestTemperaturesByIndex(0); // Le capteur 0 realise une acquisition
  // RMQ : on pourrait avoir plusieurs capteurs
  // sur le port oneWire !
  tempValue = tempSensor.getTempCByIndex(0); // On transfert le float qui correspond a
  // temp acquise
  Serial.print("Temperature: ");
  Serial.print(tempValue);
  Serial.print(" C\n");
  

  // LUMIERE
  lumValue = analogRead(A0) ; // read analog input pin 0
  Serial.print("Lumière : ");
  Serial.print(lumValue,DEC) ; // Prints the value to the serial port
  Serial.print("\n") ;


  // LAMPE PAR RAPPORT A LA LUMIERE
  if (lumValue < lumSwitch) {
    digitalWrite(ledPin , HIGH) ; // turn the LED on (HIGH is the voltage level)
  }else {
    digitalWrite(ledPin , LOW) ; // turn the LED off by making the voltage LOW
  }


/*
  // LAMPE PAR RAPPORT A LA TEMPERATURE
  if (tempValue < tempSwitch) {
    digitalWrite(ledPin , LOW) ; // turn the LED on (HIGH is the voltage level)
  }else {
    digitalWrite(ledPin , HIGH) ; // turn the LED off by making the voltage LOW
  }*/
  
  //delay(1000) ;
}

void getTemperature(){
  tempSensor.requestTemperaturesByIndex(0); // Le capteur 0 realise une acquisition
  // RMQ : on pourrait avoir plusieurs capteurs
  // sur le port oneWire !
  tempValue = tempSensor.getTempCByIndex(0); // On transfert le float qui correspond a
  // temp acquise
  Serial.print("Temperature: ");
  Serial.println(tempValue);

}


// Déclenche les actions à la réception d'un message
// D'après http://m2mio.tumblr.com/post/30048662088/a-simple-example-arduino-mqtt-m2mio
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
  
  /*
  Serial.println("jesuisla");
  int i = 0;
  if ( debug ) {
    Serial.println("Message recu =>  topic: " + String(topic));
    Serial.print(" | longueur: " + String(length,DEC));
  }
  // create character buffer with ending null terminator (string)
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
  String msgString = String(message_buff);
  if ( debug ) {
    Serial.println("Payload: " + msgString);
  }
*/
  /*
  if ( msgString == "ON" ) {
    digitalWrite(D2,HIGH);  
  } else {
    digitalWrite(D2,LOW);  
  }*/

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
 const char* ssid = "Villalucie";
 const char *password= "villaluciejulie"; 
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
