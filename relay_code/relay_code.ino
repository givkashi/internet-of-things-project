#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid = "mohammadhosein";
const char* password = "123456_qwe";



const char* mqtt_server = "192.168.1.39";
WiFiClient esp8266device;
PubSubClient client(esp8266device);

int LED = 2;

#define Lamp  D7 // Define LED as an Integer (whole numbers) and pin D8 on Wemos D1 
//************888




void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messagein;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messagein += (char)message[i];
  }

  if(topic=="room1/lamp"){
    if (messagein == "ON"){ 
      digitalWrite(Lamp, HIGH); 
      digitalWrite(LED, LOW);
      Serial.println("/.lamp ON");
      delay(1000);
      }else{
       digitalWrite(Lamp, LOW); 
       digitalWrite(LED, HIGH);
       Serial.println("");
      }
    
      
  }
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
 
    

    if (client.connect("device1")) {

      Serial.println("connected");  
      client.subscribe("room1/lamp");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {

  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(Lamp, OUTPUT); //Set the LED (D2) as an output
  pinMode(LED, OUTPUT);
  digitalWrite(Lamp, LOW); 
  digitalWrite(LED, HIGH);


}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("device1");
 

  delay(1000);
  
} 
