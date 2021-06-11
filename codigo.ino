#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define pinoA A0
#define pinoR 2
#define pino5V 13
#define LED1 5
#define LED2 4
 
int ValAnalogIn; 

const char* ssid = "Oliveira 2G";
const char* password = "A170498b@";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long ultima_mensagem = 0;
#define tamanho_mensagem  (50)
char msg[tamanho_mensagem];
int value = 0;

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String mesg; 
  
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    mesg +=c;
  }

  if (mesg.equals("irrigar")) {
    digitalWrite(pinoR, HIGH);    
 
  }if (mesg.equals("parar")) {
    digitalWrite(pinoR, LOW); 
  }

}
void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      client.publish("irrigando", "Sistema de irrigacao");
      client.subscribe("irrigado");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(pinoR, OUTPUT); 
  pinMode(pino5V, OUTPUT); 
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(pino5V, HIGH);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); 
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

ValAnalogIn = analogRead(pinoA);
int Porcento = map(ValAnalogIn, 1023, 0, 0, 100);
Serial.print(Porcento); 
Serial.println("%");
if (Porcento <= 40) {
client.publish("irrigando", "Necessita agua");
digitalWrite(LED1, HIGH);
delay (1000);
digitalWrite(LED1, LOW);
}
else { 
client.publish("irrigando", "Solo esta irrigado");
digitalWrite(LED2, HIGH);
delay (1000);
digitalWrite(LED2, LOW); 
}

  unsigned long recente = millis();
  if (recente - ultima_mensagem > 2000) {
    ultima_mensagem = recente;
    ++value;
    snprintf (msg, tamanho_mensagem, "Sistema de irrigacao #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("irrigando", msg);
  }
}
