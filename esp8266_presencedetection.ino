#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15

//Wlan Zugangsdaten
char ssid[] = "";
char pass[] = "";

//MQTT Zugangsdaten

const char* mqttServer = "IP";
const char* mqttUser = "user";
const int mqttPort = 1883;
const char* mqttPassword = "Password"; //if needed

//Globale Variablen
WiFiClient espClient;
PubSubClient client(espClient);
const char* publish_topic = "MQTT-Topic"; //MQTT Topic to publish to
const char* state_topic ="MQTT-Topic";

void setup() {
  // Mit Wlan verbinden
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected successfully");
  //Mit MQTT Verbinden
  reconnect();
  //Pins zum lesen setzen
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
}

void reconnect() {
  // Loop until we're reconnected
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("Anwesenheit", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT");
    }
    else {
      Serial.print("Failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }
  client.publish(state_topic, "Presence detection connected");
}

int measure(int pin) { //Measure the analogue value
  digitalWrite(pin, HIGH); //set Pin HIGH so mosfet is closed
  delay(200); //wait 200 microseconds to be sure voltage is reached you can fiddle with time
  int sensorValue = analogRead(A0); //read analog voltage
  delay(200); //wait 200 microseconds to be sure measurment is taken with full voltage
  digitalWrite(pin, LOW); //set Pin LOW so mosfet is opened
  return sensorValue;
}

void measurementprint(int pin, int measurment) {
  Serial.print("Messwert an Pin: ");
  switch (pin) {
    case D1:
      Serial.println("D1");
      break;
    case D2:
      Serial.println("D2");
      break;
    case D3:
      Serial.println("D3");
      break;
    case D4:
      Serial.println("D4");
      break;
    case D5:
      Serial.println("D5");
      break;
    case D6:
      Serial.println("D6");
      break;
    case D7:
      Serial.println("D7");
      break;
    case D8:
      Serial.println("D8");
      break;
  }
  Serial.println(measurment);

}
int measurementToValue(int measurement) { //Assign numbers to measured voltage within a certain range and return that number. Numbers are corresponding to certain bits so I can use bitewiseoperands for decoding
  if (measurement == 1024) {
    Serial.println("Kein Stecker erkannt"); 
    int returnvalue = 0;
    return returnvalue;
  }
  else if (900 < measurement && measurement < 980) {
    Serial.println("Creme erkannt");
    int returnvalue = 1;
    return returnvalue;
  }
  else if (770 < measurement && measurement < 850) {
    Serial.println("Weiß erkannt");
    int returnvalue = 2;
    return returnvalue;
  }
  else if (550 < measurement && measurement < 690) {
    Serial.println("Schwarz2 erkannt");
    int returnvalue = 64;
    return returnvalue;
  }
  else if (440 < measurement && measurement < 520) {
    Serial.println("Schwarz erkannt");
    int returnvalue = 4;
    return returnvalue;
  }
  else if (300 < measurement && measurement < 390) {
    Serial.println("Rot erkannt");
    int returnvalue = 8;
    return returnvalue;
  }
  else if (100 < measurement && measurement < 250) {
    Serial.println("Blau erkannt");
    int returnvalue = 16;
    return returnvalue;
  }
  else if (measurement < 50) {
    Serial.println("Silber erkannt");
    int returnvalue = 32;
    return returnvalue;
  }
  else {
    int returnvalue = 0;
    return returnvalue;
  }
}
void loop() {
  int message = 0;
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //Measuring each pin one after another and adding up the corresponding numbers
  int measurement = measure(D1);
 measurementprint(D1, measurement);
  message = message + measurementToValue(measurement);
  measurement = measure(D2);
 measurementprint(D2, measurement);
  message = message + measurementToValue(measurement);
  measurement = measure(D3);
 measurementprint(D3, measurement);
  message = message + measurementToValue(measurement);
  measurement = measure(D4);
 measurementprint(D4, measurement);
  message = message + measurementToValue(measurement);
  measurement = measure(D5);
 measurementprint(D5, measurement);
  message = message + measurementToValue(measurement);
  measurement = measure(D6);
 measurementprint(D6, measurement);
  message = message + measurementToValue(measurement);
  measurement = measure(D7);
 measurementprint(D7, measurement);
  message = message + measurementToValue(measurement);
  measurement = measure(D8);
 measurementprint(D8, measurement);
  message = message + measurementToValue(measurement);
  Serial.print("Message für MQTT ");
  Serial.println(message);
  Serial.println("--------------");
  Serial.println("Waiting for next loop");
  client.publish(publish_topic, String(message).c_str()); //Pbulish the sum of all corresponding numbers to a mqtt Topic
  delay(300); //give the ESP time to publish the MQTT message
  ESP.deepSleep(0); //go to deepsleep forever until reset
}
