#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif 
#include <PubSubClient.h>
WiFiClient espClient;
PubSubClient client(espClient);

#define LedPin      19                        

const char* ssid = "aim";                   
const char* password = "123456abc";         
const char* mqtt_server = "broker.hivemq.com";

//=========================================== WiFi & MQTT SETUP BEGIN ===========================================
char tx2buf[100];
void setup_wifi()
{ delay(10);
  sprintf(tx2buf,"Connecting to %s",ssid);
  Serial.println(tx2buf);
  WiFi.begin(ssid, password);

  while(WiFi.status()!=WL_CONNECTED) 
  { delay(500);
    Serial.print(".");
  }
  sprintf(tx2buf,"WiFi connected.\nIP address: %s\n\n",WiFi.localIP());
  Serial.println(tx2buf);
}

void callback(char* topic, byte* payload, unsigned int length) 
{ char msg=0;
  sprintf(tx2buf,"Message arrived [%s]: ",topic);
  Serial.print(tx2buf);

  for(int i=0;i<length;i++) 
    msg=(char)payload[i];
  Serial.println(msg);

  if(msg=='1') digitalWrite(LedPin,LOW);
  if(msg=='2') digitalWrite(LedPin,HIGH);
}

void reconnect() 
{ while(!client.connected()) 
  { Serial.println("Attempting MQTT connection...");
    if(client.connect("ESPClient")) 
    { Serial.println("Connected");
      client.subscribe("/LedControl");
    } 
    else 
    { sprintf(tx2buf,"Failed, rc=%s  try again in 5 seconds",client.state());
      Serial.println(tx2buf);
      delay(5000);
    }
  }
}
//============================================ WiFi & MQTT SETUP END ============================================

void setup()
{ Serial.begin(115200);
  setup_wifi(); 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(LedPin, OUTPUT);
}
void loop()
{ if(!client.connected()) 
    reconnect(); 
  client.loop();
}
