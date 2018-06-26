#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

const char* ssid = "Justins hotspot";
const char* password =  "justin1997";
const char* mqttServer = "60.226.49.247";
const char* mqtt_username = "lock001";
const char* mqtt_password = "adjgadjgqhu";
const int mqttPort = 8883;

/* PIN MAPOUT
 * D0 = GPIO16
 * D1 = GPIO5
 * D2 = GPIO4
 * D3 = GPIO0
 * D4 = GPIO2
 * D5 = GPIO14
 * D6 = GPIO12
 * D7 = GPIO13
 * D8 = GPIO15
 * RX = GPIO3
 * TX = GPIO1
 * 
 */

 
WiFiClient espClient;
PubSubClient client(espClient);


String user_lists [10]= {"Nick_Jensen", " ", " ", " ", " " ," " ," ", " ", " ", " "};

int LED_UNLOCKED = 0;
int LED_LOCKED = 16;
int DOORLOCK_PIN = 5;
bool door_locked = true;
String command;
String user_name;

void unlocked() {
  digitalWrite(DOORLOCK_PIN,LOW);
  door_locked = false;
}

void locked() {
  digitalWrite(DOORLOCK_PIN,HIGH);
  door_locked=true;
}

void addUser(String user_name) {
  for (int i=0; i<10; i++){
    if (user_lists[i] == " "){
      user_lists[i] = user_name;
      PubSubToAll();
      break;
      }
  }
}

void deleteUser(String user_name) {
  for (int i=0; i<10; i++){
    if (user_lists[i] == user_name){
      user_lists[i] = " ";
      char S[String(user_name+"/lock001").length()+1];
      String(user_name+"/lock001").toCharArray(S, sizeof(S));
      client.unsubscribe(S);
      PubSubToAll();
      break;
      }
  }
}

void PubSubToAll(){
    for (int i=0; i<10; i++){
      if (user_lists[i] != " "){
        char S[String(user_lists[i]+"/lock001").length()+1];
        String(user_lists[i]+"/lock001").toCharArray(S, sizeof(S));
        client.subscribe(S);
        client.publish(S, "Hello from ESP8266");
        
    }
  }
}


void setup() {
 
  Serial.begin(115200);
  pinMode(LED_UNLOCKED, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(LED_LOCKED, OUTPUT);
  pinMode(DOORLOCK_PIN,OUTPUT);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client",mqtt_username,mqtt_password)) {
 
      Serial.println("Correct MQTT login - Now connected to MQTT server");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  PubSubToAll();
}
 
void callback(char* topic, byte* payload, unsigned int length) {
  command = "";
  user_name = "";
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

    
  Serial.print("Message:");
  if ((char)payload[0] == 'A' || (char)payload[0]== 'D'){
      for (int j = 4; j < length; j++) {
      user_name = user_name + (char)payload[j];
      }
  }
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    command = command + (char)payload[i];
    }

  Serial.println();
  Serial.println("-----------------------");
 
}
 
void loop() {
  client.loop();
  
  if (command[0]== 'A'){ 
    addUser(user_name);
    Serial.println("User: " + user_name + " added");
    command = "";  
  }

  if (command[0]== 'H'){ 
    command = "";  
  }

  if (command[0]== 'D'){ 
    deleteUser(user_name);
    Serial.println("User: " + user_name + " deleted");  
    command = "";
  }
   
  if (command == "Lock"){ 
    locked();
    Serial.println("Door is now Locked");
    command = "";
    
  }
  if (command == "Unlock"){ 
    unlocked();
    Serial.println("Door is now Unlocked");  
    command = "";
    
  }
  command = "";
  if(door_locked == false){
    digitalWrite(LED_LOCKED, LOW);  
    digitalWrite(LED_UNLOCKED, HIGH);
  } 
  else{
    digitalWrite(LED_LOCKED,HIGH);
    digitalWrite(LED_UNLOCKED,LOW);
  }
  delay(1500);
}
