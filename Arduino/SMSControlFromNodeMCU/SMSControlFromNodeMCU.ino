#include <SoftwareSerial.h>
// GPS Module RX pin to NodeMCU D3
// GPS Module TX pin to NodeMCU D4
#define rxPin D3
#define txPin D4
#define RESET_PIN D5

SoftwareSerial a9g(rxPin, txPin);
// MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

String smsStatus, senderNumber, receivedDate, msg;
// MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

// check if the expected response is received
boolean getResponse(String expected_answer, unsigned int timeout = 1000){
  boolean flag = false;
  String response = "";
  unsigned long previous;
  //*************************************************************
  for(previous=millis(); (millis() - previous) < timeout;){
    while(a9g.available()){
      response = a9g.readString();
      if(response.indexOf(expected_answer) > 0){
        flag = true;
        goto OUTSIDE;
      }
    }
  }
  //*************************************************************
  OUTSIDE:
  if(response != ""){Serial.println(response);}
  return flag;
}
void wait(int millisec){
  unsigned long previous=millis();
  while(millis()-previous<millisec);
}
// Check if the at command was successful
boolean tryATcommand(String cmd, String expected_answer, int timeout, int total_tries, boolean shouldRESET = false){
  TryAgain:
  for(int i=1; i<=total_tries; i++){
    a9g.println(cmd);
    if(getResponse(expected_answer, timeout) == true){
      digitalWrite(LED_BUILTIN, LOW);
      break;
    }
    else {
      Serial.print(".");
    }    
    if(i == total_tries && shouldRESET){
      Serial.println("Faild! Resetting the Module");
      digitalWrite(RESET_PIN, LOW);
      wait(100);
      digitalWrite(RESET_PIN, HIGH);
      goto TryAgain;
    }
  }
  //*************************************************************
}

void powerOn(){
  tryATcommand("AT","OK",1000,20,true);
}

void replyfroma9g()
{
  Serial.println(a9g.readString());
}

void setup()
{
  //---------------------------------------------------------
  Serial.begin(115200);
  Serial.println("Arduino serial initialize");
  //---------------------------------------------------------
  a9g.begin(115200);
  Serial.println("A9G software serial initialize");
  //---------------------------------------------------------
  smsStatus = "";
  senderNumber = "";
  receivedDate = "";
  msg = "";
  //---------------------------------------------------------
  // TODO check what this is
  // a9g.println("ATE1"); // Modem command echo enable, it's not always required, but it's handy for debugging. Any typing on keyboard will be echoed back by the modem.
  // wait(1000);
  // a9g.println("AT+CMGF=1"); // SMS text mode
  // wait(1000);
  //---------------------------------------------------------
  pinMode(RESET_PIN,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  powerOn();
}
// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
void loop()
{
  //---------------------------------------------------------
  // while (a9g.available())
  // {
    // parseData(a9g.readString());

  // }
  //---------------------------------------------------------
  while (Serial.available())
  {
    a9g.println(Serial.readString());
  }
  replyfroma9g();
  //---------------------------------------------------------
} // main loop ends
// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
void parseData(String buff)
{
  Serial.println(buff);

  unsigned int len, index;
  //---------------------------------------------------------
  // Remove sent "AT Command" from the response string.
  index = buff.indexOf("\r");
  buff.remove(0, index + 2);
  buff.trim();
  //---------------------------------------------------------

  //---------------------------------------------------------
  if (buff != "OK")
  {
    index = buff.indexOf(":");
    String cmd = buff.substring(0, index);
    cmd.trim();

    buff.remove(0, index + 2);
    // MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
    if (cmd == "+CMTI")
    {
      // get newly arrived memory location and store it in temp
      index = buff.indexOf(",");
      String temp = buff.substring(index + 1, buff.length());
      temp = "AT+CMGR=" + temp + "\r";
      // get the message stored at memory location "temp"
      a9g.println(temp);
    }
    // MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
    else if (cmd == "+CMGR")
    {
      extractSms(buff);

      // if received sms have at command
      if (msg.startsWith("at+") || msg.startsWith("at"))
      {
        sendResponseATcommand(msg);
        smsStatus = "";
        senderNumber = "";
        receivedDate = "";
        msg = "";
      }
    }
    // MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  }
  //---------------------------------------------------------
  else
  {
    // The result of AT Command is "OK"
  }
  //---------------------------------------------------------
}
// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
void sendResponseATcommand(String ATcommand)
{
  //---------------------------------------------------------
  if (ATcommand.equals(""))
  {
    Serial.println("No AT Command is found.");
    return;
  }
  //---------------------------------------------------------
  uint8_t answer = 0;
  unsigned int timeout = 3000;
  String response;
  unsigned long previous;
  wait(100);
  //---------------------------------------------------------
  // Clean the input buffer
  while (a9g.available() > 0)
    a9g.read();
  //---------------------------------------------------------
  // Send the AT command
  a9g.println(ATcommand);
  //---------------------------------------------------------
  previous = millis();
  //---------------------------------------------------------
  // this loop waits for the answer with time out
  do
  {
    // if there are data in the UART input buffer, reads it.
    // MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
    if (a9g.available() != 0)
    {
      response = a9g.readString();

      if (response != "")
        answer == 1;
    }
    // MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  } while ((answer == 0) && ((millis() - previous) < timeout));
  //---------------------------------------------------------
  // Remove sent "AT Command" from the response string.
  int index = response.indexOf("\r");
  response.remove(0, index + 2);
  response.trim();
  Serial.println("---------------------");
  Serial.println(response);
  Serial.println("---------------------");
  //---------------------------------------------------------
  // Reply(response);
  //---------------------------------------------------------
}
// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
void extractSms(String buff)
{
  unsigned int index;
  //---------------------------------------------------------
  index = buff.indexOf(",");
  smsStatus = buff.substring(1, index - 1);
  buff.remove(0, index + 2);
  //---------------------------------------------------------
  senderNumber = buff.substring(0, 13);
  buff.remove(0, 19);
  //---------------------------------------------------------
  receivedDate = buff.substring(0, 20);
  buff.remove(0, buff.indexOf("\r"));
  buff.trim();
  //---------------------------------------------------------
  index = buff.indexOf("\n\r");
  buff = buff.substring(0, index);
  buff.trim();
  msg = buff;
  buff = "";
  msg.toLowerCase();
  //---------------------------------------------------------
}
// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
void Reply(String text)
{
  a9g.print("AT+CMGF=1\r");
  wait(1000);
  a9g.print("AT+CMGS=\"" + senderNumber + "\"\r");
  wait(1000);
  a9g.print(text);
  wait(100);
  // ascii code for ctrl-26
   a9g.println((char)26); //ascii code for ctrl-26
//  a9g.write(0x1A);
  wait(1000);
  Serial.println("SMS Sent Successfully.");
}
// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
