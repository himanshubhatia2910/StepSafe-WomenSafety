/* ------------------------------------------------------------------------
 * Created by: Tauseef Ahmad
 * Created on: 12 May, 2022
 *
 * Tutorial: https://youtu.be/B_5nn7URZTk
 * ------------------------------------------------------------------------*/

// #include<SoftwareSerial.h>
#define RESET_PIN 21
unsigned long previousMillis = 0;
// SoftwareSerial Serial2(D3,D4);
void SendMessage()
{
  Serial.println("Setting the GSM in text mode");
  // Serial2.println("AT+CMGF=1\r");
  Serial2.println("AT+CMGF=1\r");

  while (Serial.available())
  {
    Serial2.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read()); // Forward what Software Serial received to Serial Port
  }
  delay(2000);
  
  Serial.println("Sending SMS to the desired phone number!");
  // Serial2.println("AT+CSMP=17,167,0,0\r");
  Serial2.println("AT+CSMP=17,167,0,0\r");

  
  while (Serial.available())
  {
    Serial2.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read()); // Forward what Software Serial received to Serial Port
  }
  // Serial2.println("AT+CMGS=\"+919372391056\"\r");
  delay(2000);
  Serial2.println("AT+CMGS=\"+919372391056\"\r");

  
  while (Serial.available())
  {
    Serial2.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read()); // Forward what Software Serial received to Serial Port
  }
  delay(2000);
  // Serial2.println("Hello from A9G");
  Serial2.println("Hello from serial");
  
  while (Serial.available())
  {
    Serial2.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read()); // Forward what Software Serial received to Serial Port
  }
  delay(2000);
  // Serial2.println((char)26);
  Serial2.println((char)26);
  while (Serial.available())
  {
    Serial2.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read()); // Forward what Software Serial received to Serial Port
  }
  Serial.println("Message sent");
  // delay(10000);
}

void setup()
{
  pinMode(RESET_PIN, OUTPUT);
  // pinMode(LED_BUILTIN, OUTPUT);
  Serial2.begin(115200);

  Serial.begin(115200);

  delay(2000);
  SendMessage();
}

void loop()
{
  while (Serial2.available())
  {
    Serial.println(Serial2.readString());
  }
  while (Serial.available())
  {
    Serial2.println(Serial1.readString());
  }
}
