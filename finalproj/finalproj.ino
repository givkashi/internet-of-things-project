#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(D7,D1,D2,D3,D4,D5,D6);
//LiquidCrystal(rs,r/w, enable, d4, d5, d6, d7)
const char* ssid = "mohammadhosein";
const char* password = "123456_qwe";

int pirpin=D14;
int photocell=D11;
boolean pir=false;

int lastState = LOW;
int buzzerPin=D12;
int duration=1000; //in miliseconds
int rest=2000; //in miliseconds
int light=D10;

int s1=A0;
Ticker T1,T2;
int reading;
int LED=2;
float outTemp=0;
float inTemp=0;


const char* mqtt_server = "192.168.1.39";
WiFiClient esp8266device;
PubSubClient client(esp8266device);



bool flag=true;
//*****
void ReadTemp(){
  reading=analogRead(s1)/4; //0-1023  --> 0-255
  inTemp=reading;
  Serial.println();
  Serial.print("temp is:");
  Serial.println(reading);
   char t [8];
   dtostrf(reading,6,2,t);
  client.publish("room1/sensortemp",t);


}

void ReadPIR(){
      Serial.println(digitalRead(pirpin));
      
     if (digitalRead(pirpin))
    {
      
        Serial.println("in pir publish to node red");
        client.publish("room1/sensorpir","1");
        digitalWrite(buzzerPin,HIGH);
        delay(1000);
        digitalWrite(buzzerPin,LOW);
     
      if (!lastState)
      {
        Serial.println("Somebody is here!");
        lastState = !lastState;
      }
    }
    else
    {
    
      if (lastState )
      {
        Serial.println("Went away!");
        lastState = !lastState;
      }
    }
  
}

void ReadPhotocell(){

  if(! digitalRead(photocell)){
    Serial.println("ledlow");
    digitalWrite(LED, LOW);
    
  }else{
    digitalWrite(LED, HIGH);
  }
  
}

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

  if(topic=="room1/light1"){
    if (messagein == "ON"){ 
      digitalWrite(light, HIGH);
      delay(1000);
    }else{
        digitalWrite(light, LOW);
      } 
  }
 

  if(topic =="room1/pir"){
    if(messagein == "ON"){
      pir=true;
       Serial.println("PIR true");   
    }else{
      pir=false;
      Serial.println("PIR false"); 
    }
  }

  if(topic =="room1/tempout"){
    Serial.println("tempout"); 
    Serial.println(messagein);
    outTemp=messagein.toInt(); 
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
 
    

    if (client.connect("device1")) {

      Serial.println("connected");  
      client.subscribe("room1/tempout");
      client.subscribe("room1/pir");
      client.subscribe("room1/light1");
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
  lcd.begin(16, 2);
  pinMode(light,OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(light, LOW);
  digitalWrite(LED, HIGH);
  pinMode(pirpin, INPUT);
  pinMode(photocell, INPUT);
  pinMode(buzzerPin,OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  T1.attach(40,ReadTemp); 
  T2.attach(50,ReadPhotocell);
  

}

void loop() {
  lcd.print("temp in :");
  lcd.print(inTemp);

  if(pir){
    ReadPIR();
  }
    
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("device1");
  //lcd.setCursor(0, 1);
  //lcd.print("TempIn");
  //lcd.print(inTemp);
  delay(5000);
  
} 
