#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif 
#include <PubSubClient.h>
WiFiClient espClient;
PubSubClient client(espClient);

#define LedPin1      19                        
#define LedPin2      18                        
#define LedPin3      5                        
#define LedPin4      17                        
#define LedPin5      16                        
#define LedPin6      4                        
#define LedPin7      2                        
#define LedPin8      15                        

const char* ssid = "aim";                   
const char* password = "123456abc";         
const char* mqtt_server = "broker.emqx.io";

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
{ String msg="";

  for(int i=0;i<length;i++) msg+=(char)payload[i];
  sprintf(tx2buf,"Message arrived [%s]: %s\n",topic,msg.c_str());
  Serial.print(tx2buf);

  if(msg=="led1_1") digitalWrite(LedPin1,LOW);
  if(msg=="led1_0") digitalWrite(LedPin1,HIGH);
  if(msg=="led2_1") digitalWrite(LedPin2,LOW);
  if(msg=="led2_0") digitalWrite(LedPin2,HIGH);
  if(msg=="led3_1") digitalWrite(LedPin3,LOW);
  if(msg=="led3_0") digitalWrite(LedPin3,HIGH);
  if(msg=="led4_1") digitalWrite(LedPin4,LOW);
  if(msg=="led4_0") digitalWrite(LedPin4,HIGH);
  if(msg=="led5_1") digitalWrite(LedPin5,LOW);
  if(msg=="led5_0") digitalWrite(LedPin5,HIGH);
  if(msg=="led6_1") digitalWrite(LedPin6,LOW);
  if(msg=="led6_0") digitalWrite(LedPin6,HIGH);
  if(msg=="led7_1") digitalWrite(LedPin7,LOW);
  if(msg=="led7_0") digitalWrite(LedPin7,HIGH);
  if(msg=="led8_1") digitalWrite(LedPin8,LOW);
  if(msg=="led8_0") digitalWrite(LedPin8,HIGH);
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

  pinMode(LedPin1, OUTPUT);  pinMode(LedPin2, OUTPUT);  pinMode(LedPin3, OUTPUT);  pinMode(LedPin4, OUTPUT);
  pinMode(LedPin5, OUTPUT);  pinMode(LedPin6, OUTPUT);  pinMode(LedPin7, OUTPUT);  pinMode(LedPin8, OUTPUT);

  digitalWrite(LedPin1,HIGH);  digitalWrite(LedPin2,HIGH);  
  digitalWrite(LedPin3,HIGH);  digitalWrite(LedPin4,HIGH);  
  digitalWrite(LedPin5,HIGH);  digitalWrite(LedPin6,HIGH);
  digitalWrite(LedPin7,HIGH);  digitalWrite(LedPin8,HIGH);
}
void loop()
{ if(!client.connected()) 
    reconnect(); 
  client.loop();
}
