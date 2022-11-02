/* ------------------------------------------------------------------------
 * Created by: Tauseef Ahmad
 * Created on: 12 May, 2022
 *  
 * Tutorial: https://youtu.be/B_5nn7URZTk
 * ------------------------------------------------------------------------*/

#include<SoftwareSerial.h>
#define RESET_PIN D5
unsigned long previousMillis = 0;
SoftwareSerial sim800l(D3,D4);
void SendMessage()
{
  Serial.println("Setting the GSM in text mode");
  // Serial2.println("AT+CMGF=1\r");
  sim800l.println("AT+CMGF=1\r");
 
  delay(2000);
  Serial.println("Sending SMS to the desired phone number!");
  // Serial2.println("AT+CSMP=17,167,0,0\r");
  sim800l.println("AT+CSMP=17,167,0,0\r");

  delay(2000);
  Serial.println("Sending SMS to the desired phone number!");
  // Serial2.println("AT+CMGS=\"+919372391056\"\r");
  sim800l.println("AT+CMGS=\"+919372391056\"\r");

  delay(2000);
  // Serial2.println("Hello from A9G");
  sim800l.println("Hello from serial");
  delay(2000);

  // Serial2.println((char)26);
  sim800l.println((char)26);
//  Serial.flush();
  // delay(10000);
}



void setup() {
  pinMode(RESET_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  sim800l.begin(9600);

  Serial.begin(115200);


  delay(20000);
  SendMessage();
}

void loop() {

//  while(sim800l.available()){
//    String response = sim800l.readString();
//    response.trim();
//    Serial.println(response);
//  }
  while(Serial.available()){
    sim800l.println(Serial.readString());
  }
//  if(millis() - previousMillis > 10000) { //15 Minutes interval
//    sim800l.println("AT");
//    previousMillis = millis();
//  }

}
