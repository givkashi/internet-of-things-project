#define WIFI_NAME "Red"
#define WIFI_PASSWORD "55322745"
#define DEVICE_ID 1
#define DEVICE_NAME "D1_ESP"
#define TOKEN "~806528_J1GCU88yX3b4NxnhTMp9m0YC"


#include <RemoteMe.h>
#include <RemoteMeSocketConnector.h>
#include <RemoteMeDirectWebSocketConnector.h> // !important library needs WebSockets by Markus Sattler Please install it from Library manager

#include <ESP8266WiFi.h>

#define Relay_1 D7 

RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

//*************** CODE FOR COMFORTABLE VARIABLE SET *********************

inline void setRelay(boolean b) {remoteMe.getVariables()->setBoolean("Relay", b); }

//*************** IMPLEMENT FUNCTIONS BELOW *********************

void onRelayChange(boolean b) {
  Serial.printf("onRelayChange: b: %d\n",b);
 Serial.println(b);
  digitalWrite(Relay_1, b ? HIGH : LOW);
}




void setup() {
  Serial.begin(115200);
  pinMode(Relay_1, OUTPUT);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  Serial.println(WiFi.status());
 digitalWrite(Relay_1, LOW);

  remoteMe.getVariables()->observeBoolean("Relay" ,onRelayChange);

  remoteMe.setConnector(new RemoteMeSocketConnector());
  remoteMe.setDirectConnector(new RemoteMeDirectWebSocketConnector());
  remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
}


void loop() {
  remoteMe.loop();
}
