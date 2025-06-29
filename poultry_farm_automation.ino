#define BLYNK_TEMPLATE_ID "TMPL349dz64r8"
#define BLYNK_TEMPLATE_NAME "Poultryfarm"
#define BLYNK_AUTH_TOKEN "On0tj6Nc2l0J38bf2DRa0gdSvHvHEYic"
#define BLYNK_PRINT Serial
#define sensorPin 33
#define Led1 15
#define Led2 2
#define Led3 4
#define Spray 13
#include <WiFi.h>
//#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp32.h>
#include <DFRobot_DHT11.h>
#include <DHT.h>
#include <SoftwareSerial.h>
SoftwareSerial gsmSerial(3, 1);
#define DHTPIN 32 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "OPPO A38";  // type your wifi name
char pass[] ="anbu0256"; // type your wifi password

BlynkTimer timer;
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  int sensorValue = analogRead(sensorPin);// or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
    Blynk.virtualWrite(V2, sensorValue);
     Serial.print("Gas:");
  Serial.println(sensorValue);
  if(sensorValue>2700)
  {
    digitalWrite(26,HIGH);
    digitalWrite(Spray,HIGH);
    if (gsmSerial.available()) {
    Serial.write(gsmSerial.read());
  }
     // Initialize the GSM module
  gsmSerial.println("AT"); // Test communication with the GSM module
  delay(1000);
  gsmSerial.println("AT+CMGF=1"); // Set SMS mode to text mode
  delay(1000);
  gsmSerial.println("AT+CMGS=\"+917604937305\""); // Replace with your phone number
  delay(1000);
  gsmSerial.println("alret ammonia level is increased !");
  delay(1000);
  gsmSerial.write(26); 
    
  }
  else
  {
     digitalWrite(26,LOW);
  }
  if(sensorValue <= 2650)
  {
    digitalWrite(Led1,HIGH);
    digitalWrite(Led2,LOW);
    digitalWrite(Led3,LOW);
  }
  else if(sensorValue <= 2750)
  {
    digitalWrite(Led2,HIGH);
    digitalWrite(Led1,LOW);
    digitalWrite(Led3,LOW);
  }
  else if(sensorValue <= 2850)
  {
    digitalWrite(Led3,HIGH);
    digitalWrite(Led1,LOW);
    digitalWrite(Led2,LOW);
  }
  else{
    digitalWrite(Led1,LOW);
    digitalWrite(Led2,LOW);
    digitalWrite(Led3,LOW);
  }
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);
}

void setup()
{   
  
  Serial.begin(115200);
  pinMode(26,OUTPUT);  
   digitalWrite(26,LOW);
  pinMode(Led1,OUTPUT);
  pinMode(Led2,OUTPUT);
  pinMode(Led3,OUTPUT);
  pinMode(Spray,OUTPUT);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(100L, sendSensor);
  gsmSerial.begin(9600);
  delay(1000);
 
  }

void loop()
{
  Blynk.run();
  timer.run();
 }
