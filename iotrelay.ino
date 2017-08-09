/*
 Basic MQTT example
 This sketch demonstrates the basic capabilities of the library.
 It connects to an MQTT server then:
  - subscribes to the topic "relay/onoff"
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function.
 
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// you only need to change the settings here
int RELAYPIN = 4; // D2

const char* ssid = "xxxxxx";
const char* password = "xxxxxx";

const char* mqtt_server = "xxxxxx";
int mqtt_port = xxxxxx;
const char* username = "xxxxxx";
const char* pwd = "xxxxxx";

// setting end

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  Serial.begin(115200);
  
    // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turn the LED off by making the voltage HIGH

  pinMode(RELAYPIN, OUTPUT);

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}



void loop() {

  if (!client.connected()) {
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED off by making the voltage HIGH
    reconnect();
  }
  client.loop();

}

void setup_wifi() {

  delay(1000);
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
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if (client.connect("ESP8266Client",username,pwd)) {
      Serial.println("connected");
      
      client.subscribe("relay/onoff");

      digitalWrite(LED_BUILTIN, LOW); // turn the LED on by making the voltage LOW
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  
  Serial.println();

  if ((char)payload[0] == '1'){
      Serial.println("RELAY ON");
      digitalWrite(RELAYPIN, HIGH);
  
   }

  else {
    Serial.println("RELAY OFF");
    digitalWrite(RELAYPIN, LOW);
    
   }
}
  

