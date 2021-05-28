#define pinoA A0
#define pinoR 8
#define pino5V 7
#define LED1 11
#define LED2 10
 
int ValAnalogIn; 
 
void setup() {
Serial.begin(9600);
pinMode(pinoR, OUTPUT); 
pinMode(pino5V, OUTPUT); 
pinMode(LED1, OUTPUT);
pinMode(LED2, OUTPUT);
digitalWrite(pino5V, HIGH); 
}
 
void loop() {
ValAnalogIn = analogRead(pinoA);
int Porcento = map(ValAnalogIn, 1023, 0, 0, 100);
 
Serial.print(Porcento); 
Serial.println("%");
 
if (Porcento <= 35) {
Serial.println("Irrigando ...");
digitalWrite(pinoR, HIGH); 
digitalWrite(LED1, HIGH);
delay (1000);
digitalWrite(LED1, LOW);

}
 
else { 
Serial.println("Solo esta irrigado");
digitalWrite(pinoRele, LOW); 
digitalWrite(LED2, HIGH);
delay (1000);
digitalWrite(LED2, LOW);
}
delay (1000);
}
