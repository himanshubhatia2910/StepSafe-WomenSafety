/* This code works with Sim800L and a push button
 * Press the button to send a simple SMS/Text to a specified phone number
 * Refer to www.SurtrTech.com for more details
 */

#include <SoftwareSerial.h>

SoftwareSerial a9g(D5,D6); // RX,TX for Arduino and for the module it's TXD RXD, they should be inverted

#define button1 7 // Button pin, on the other pin it's wired with GND

bool button_State; // Button state

void poweron()
{
  int i = 0;
  boolean result = false;
  // digitalWrite(pon, LOW);
  // delay(3000);
  // digitalWrite(pon, HIGH);
  // delay(15000);

  result = a9g.find("OK");
  if (result)
    Serial.println("Please send sms to control your device!");
  else
  {
    for (i = 0; i < 10; i++)
    {
      a9g.println("AT");
      delay(1000);
      result = a9g.find("OK");
      if (result)
      {
        Serial.println("Please send sms to control your device!");
        return;
      }
    }
  }
}
void setup()
{

  pinMode(button1, INPUT_PULLUP); // The button is always on HIGH level, when pressed it goes LOW
  a9g.begin(9600);            // Module baude rate, this is on max, it depends on the version
  Serial.begin(9600);
  delay(1000);
}

void loop()
{

  button_State = digitalRead(button1); // We are constantly reading the button State

  if (button_State == LOW)
  {                                   // And if it's pressed
    Serial.println("Button pressed"); // Shows this message on the serial monitor
    delay(200);                       // Small delay to avoid detecting the button press many times

    SendSMS(); // And this function is called
  }

  if (a9g.available())
  { // Displays on the serial monitor if there's a communication from the module
    Serial.write(a9g.read());
  }
}

void SendSMS()
{
  Serial.println("Sending SMS..."); // Show this message on serial monitor
  a9g.print("AT+CMGF=1\r");     // Set the module to SMS mode
  delay(100);
  a9g.print("AT+CMGS=\"+919372391056\"\r"); // Your phone number don't forget to include your country code, example +212123456789"
  delay(500);
  a9g.print("SIM800l is working"); // This is the text to send to the phone number, don't make it too long or you have to modify the SoftwareSerial buffer
  delay(500);
  a9g.print((char)26); // (required according to the datasheet)
  delay(500);
  a9g.println();
  Serial.println("Text Sent.");
  delay(500);
}
