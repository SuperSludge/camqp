#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <ArduinoJson.h>

#define wifi_ssid "default"
#define wifi_password "johann49"

#define DEBUG
#define DEBUG_ESP_SSL
#define DEBUG_SSL

#define mqtt_server "url"
#define mqtt_server_port 1883
#define mqtt_user "user"
#define mqtt_password "pwd"

//WiFiClientSecure espClient;
WiFiClient espClient;
PubSubClient client(espClient);

float resetAt = 0;


byte relON[] = {0xA0, 0x01, 0x01, 0xA2};  //Hex command to send to serial for open relay
byte relOFF[] = {0xA0, 0x01, 0x00, 0xA1}; //Hex command to send to serial for close relay

int ledState = false;
unsigned long previousMillis = 0;
const long interval = 2000; //  2 seconds



void setup() {

    // initialize serial:
  Serial.begin(9600);
  

  setup_wifi();
  //espClient.allowSelfSignedCerts();
  client.setCallback(callback);
  client.setServer(mqtt_server, mqtt_server_port);
}

void callback(char* topic, byte* payload, unsigned int length)
{

  
  Serial.println("data received");
  
  on();
  delay(2000);
  off();
  //resetAt = millis() + 1000;
  //digitalWrite(2, LOW);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

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
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("V2", mqtt_user, mqtt_password)) {
      Serial.println("connected");

      client.subscribe("doordings",1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

long lastMsg = 0;

void loop() {
  if (resetAt > 0 && resetAt < millis()) {
    off();
  }


  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 60000) {
    lastMsg = now;

    String payload;
    StaticJsonBuffer<200> jsonBuffer;

    JsonObject& root = jsonBuffer.createObject();
    root["blablabla"] = true;

    root.printTo(payload);
    Serial.print("Sending: ");
    Serial.println(payload);
    char payloadChar[payload.length() + 1];

    payload.toCharArray(payloadChar, payload.length() + 1);

    client.publish("doori", payloadChar, true);
  }
}



// the loop function runs over and over again forever
void on()
{
  Serial.write(relON, sizeof(relON));     // turns the relay ON
  ledState = false;
}

void off()
{

  Serial.write(relOFF, sizeof(relOFF));   // turns the relay OFF
  ledState = true;
}

