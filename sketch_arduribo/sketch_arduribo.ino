#include "OneWire.h"
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>

const int lumSwitch = 1000;
const float tempSwitch = 24.0;
const int ledPin = 19;

int lumValue;
float tempValue;
OneWire oneWire(23) ; // Pour utiliser une entite oneWire sur le port 22
DallasTemperature tempSensor(&oneWire) ; // Cette entite est utilisee par le capteur

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
 const char* ssid = "theLivebox";
 const char *password= "salut2603"; 
 
 Serial.println("\nConnecting Wifi...");
 WiFi.begin(ssid, password);
 while(WiFi.status() != WL_CONNECTED){
   Serial.print("Attempting to connect ..\n");
   delay(1000);
 }
 Serial.print("Connected to local Wifi\n");  
 print_ip_status();
}


void setup(){ 
  Serial.begin(9600);
  while (!Serial); // wait for a serial connection
  
  pinMode(ledPin , OUTPUT) ;
  tempSensor.begin(); // Init du capteur et de l’entite OneWire

  

  tempSensor.requestTemperaturesByIndex(0); // Le capteur 0 realise une acquisition
  // RMQ : on pourrait avoir plusieurs capteurs
  // sur le port oneWire !
  tempValue = tempSensor.getTempCByIndex(0); // On transfert le float qui correspond a
  // temp acquise
  Serial.print("Temperature: ");
  Serial.print(tempValue);

  connect_wifi();
      
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
  
   HTTPClient http;   
   Serial.print("connecté !");
   /*
   http.begin("http://jsonplaceholder.typicode.com/posts");  //Specify destination for HTTP request
   http.addHeader("Content-Type", "text/plain");             //Specify content-type header
 
   int httpResponseCode = http.POST("POSTING from ESP32");   //Send the actual POST request
 
   if(httpResponseCode>0){
 
    String response = http.getString();                       //Get the response to the request

    Serial.print("\n");
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
    */

  /*
   http.begin("http://localhost/temperatures");  //Specify destination for HTTP request
   http.addHeader("Content-Type", "text/plain");             //Specify content-type header
 
   int httpResponseCode = http.POST("POSTING from ESP32");   //Send the actual POST request
 
   if(httpResponseCode>0){
 
    String response = http.getString();                       //Get the response to the request

    Serial.print("\n");
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
    
 
   }else{
 
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
 
   }
 
   http.end();  //Free resources
 */
 }else{
 
    Serial.println("Error in WiFi connection");   
 
 }

/*
 // Armement du timer en micro sec
esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP ∗ us_TO_S_FACTOR) ;
// Deep sleep mode
esp_deep_sleep_start( ) ;
*/

 
}

void loop(){
  
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
