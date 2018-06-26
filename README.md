# CloudLock
IoT based automated door lock system with access control management functionalities

Please see https://www.youtube.com/watch?v=0LvJdLWOgVI for visual understanding of how the device works.

Author: Justin Choi, Nick Jensen, Reif Virtue

The cloudlock offers real-time access management of any doors (typically hoseholds) all button clicks away from a smartphone. 
The architecture of the system comprises of three sections: 
1. MQTT server: cloud server based on subscriber-publisher messaging protocol which works on top of TCP/IP layer. This is a popular choice among many IoT solutions as it requires small code footprint and small bandwidth requirement. 
2. Mobile Application: used for interactions with the user and the door. It has lock/unlock, view history, add/remove users, add.revmoe doors functionalities. Current version supports android platform only. Source code to be uploaded soon. 
3. Hadrware: includes hardware components mounted on the door. The door is equipped with ESP8266 chip which serves as both wifi-module and the MCU. The chip handles user commands and operate the electric strike accordingly. The chip is programmed using Arduino IDE.

Each layers are connected to each other using TLS/SSL. Note that an attempt to provide SSL connection to ESP8266 has been made however it was discovered that the chip has a limited RAM and therefore could not be implemented. It is recommended for future versions that higher performance chips such as ESP32 used to ensure no bottleneck in the performance. 

Ensure in your arduino IDE, correct MCU board is selected and also PubSubClient library is included in the correct directory. 
PubsubClient libary is for connections to MQTT server. 

refer to the following resporitory for downloading the libraries:

1. https://github.com/knolleary/pubsubclient (PubSubClient)
