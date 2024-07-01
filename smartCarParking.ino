#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

#define SENSOR1_PIN D1
#define SENSOR2_PIN D2
#define SENSOR3_PIN D0
#define LED1_PIN D4
#define LED2_PIN D5
#define LED3_PIN D6
float prev1=0;
float prev2=0;
float prev3=0;
// Replace with your WiFi credentials
const char* ssid = "smart";
const char* password = "123456789";

// Replace with your ThingSpeak Channel ID and API Key
unsigned long channelID = 2096352;
const char* apiKey = "P7K86K9NYGUXWTDZ";

WiFiClient client;

void setup() {
  pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);
  pinMode(SENSOR3_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  ThingSpeak.begin(client);
}

void loop() {
  // Read sensor values
  int sensor1Value = digitalRead(SENSOR1_PIN);
  int sensor2Value = digitalRead(SENSOR2_PIN);
  int sensor3Value = digitalRead(SENSOR3_PIN);


  float field1Value = ThingSpeak.readFloatField(channelID ,1);

  int statusCode = ThingSpeak.getLastReadStatus();
  if(statusCode == 200 and prev1!=field1Value){
    // Serial.print("Field 1 value: ");
    // Serial.println(field1Value);
      sensor1Value = !field1Value;
      prev1=!field1Value;
  }
  // else{
  //   Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
  // }
 
  float field2Value = ThingSpeak.readFloatField(channelID ,2);

  int statusCode2 = ThingSpeak.getLastReadStatus();
  if(statusCode2 == 200 and prev2!=field2Value){
    // Serial.print("Field 2 value: ");
    // Serial.println(field2Value);
      sensor2Value = !field2Value;
      prev2=!field2Value;
  }
  // else{
  //   Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
  // }
 
  float field3Value = ThingSpeak.readFloatField(channelID ,3);

  int statusCode3 = ThingSpeak.getLastReadStatus();
  if(statusCode3 == 200 and prev3!=field3Value){
    // Serial.print("Field 3 value: ");
    // Serial.println(field3Value);
      sensor3Value = !field3Value;
      prev3=!field3Value;
  }
  // else{
  //   Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
  // }
 


  // Print sensor values
  Serial.print("Sensor 1: ");
  Serial.println(!sensor1Value);
  Serial.print("Sensor 2: ");
  Serial.println(!sensor2Value);
  Serial.print("Sensor 3: ");
  Serial.println(!sensor3Value);

  // Turn on/off LEDs based on sensor values
  digitalWrite(LED1_PIN, !sensor1Value);
  digitalWrite(LED2_PIN, !sensor2Value);
  digitalWrite(LED3_PIN, !sensor3Value);

  // Send sensor data to ThingSpeak
  ThingSpeak.setField(1, !sensor1Value);
  ThingSpeak.setField(2, !sensor2Value);
  ThingSpeak.setField(3, !sensor3Value);

    // Debugging statements
  Serial.print("LED 1: ");
  Serial.println(digitalRead(LED1_PIN));
  Serial.print("LED 2: ");
  Serial.println(digitalRead(LED2_PIN));
  Serial.print("LED 3: ");
  Serial.println(digitalRead(LED3_PIN));

  int httpCode = ThingSpeak.writeFields(channelID, apiKey);

  if (httpCode == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.print("Error sending data to ThingSpeak. HTTP error code: ");
    Serial.println(httpCode);
  }

  // delay(1000);
}
