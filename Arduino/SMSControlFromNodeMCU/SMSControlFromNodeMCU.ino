// TxRx pins on ESP32
// TX = 17
// RX = 16
#define RESET_PIN 21
#define builtin_led 2
#define TRIGGER_BUTTON 22
#define RED_LED 5
#define GREEN_LED 18
#define BLUE_LED 19

// FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// FLAGS
bool GPSFixed = false;
bool ATOK = false;

String smsStatus, senderNumber, receivedDate, msg;

void setup()
{
  Serial.begin(115200);
  Serial.println("Arduino serial initialize");
  Serial2.begin(115200);
  Serial.println("A9G software serial initialize");
  //---------------------------------------------------------
  // smsStatus = "";
  // senderNumber = "";
  // receivedDate = "";
  // msg = "";
  //---------------------------------------------------------
  pinMode(RESET_PIN, OUTPUT);
  pinMode(builtin_led, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(TRIGGER_BUTTON,INPUT);
  powerOn();
  setupSMS();
}

// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
void loop()
{
  keepcommunication();
  statusLED();
} // main loop ends

// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

void statusLED()
{
  // TODO Blue is ATOK is ON
  // TODO Green is GSM Module is Fixed
  if (GPSFixed)
    digitalWrite(GREEN_LED, HIGH);
  else
    digitalWrite(GREEN_LED, LOW);
  if (ATOK)
    digitalWrite(BLUE_LED, HIGH);
  else
    digitalWrite(BLUE_LED, LOW);
}

/*@result 
true : triggered, 
false : not triggered*/
bool checktriggered()
{
  int read = digitalRead(TRIGGER_BUTTON);
  if (read == HIGH)
  {
    alertSMS();
    return true;
  }
  return false;
}

void alertSMS()
{
  Serial.println("Inside AlertSMS function");
  // TODO send alert message code
}

// check if the expected response is received
boolean getResponse(String expected_answer, int timeout = 1000)
{
  boolean flag = false;
  String response = "";
  unsigned long previous;
  //*************************************************************
  for (previous = millis(); (millis() - previous) < timeout;)
  {
    while (Serial2.available())
    {
      response = Serial2.readString();
      if (response.indexOf(expected_answer) > 0)
      {
        flag = true;
        goto OUTSIDE;
      }
    }
  }
//*************************************************************
OUTSIDE:
  if (response != "")
  {
    Serial.println(response);
  }
  return flag;
}

void wait(int millisec)
{
  unsigned long previous = millis();
  while (millis() - previous < millisec);
}

// Check if the at command was successful
boolean tryATcommand(String cmd, String expected_answer, int timeout = 1000, int total_tries = 1, boolean shouldRESET = false)
{
TryAgain:
  for (int i = 1; i <= total_tries; i++)
  {
    Serial2.println(cmd);
    if (getResponse(expected_answer, timeout) == true)
    {
      Serial.print("Got expected response : ");
      Serial.println(expected_answer);
      break;
    }
    else
    {
      Serial.print(".");
    }
    if (i == total_tries && shouldRESET)
    {
      Serial.println("Failed! Resetting the Module");
      digitalWrite(RESET_PIN, LOW);
      wait(100);
      digitalWrite(RESET_PIN, HIGH);
      goto TryAgain;
    }
  }
  //*************************************************************
}

void powerOn()
{
  Serial.println("Initializing powerOn");
  tryATcommand("AT", "OK", 1000, 20, true);
}

void setupSMS()
{
  Serial.println("Initializing setupSMS");
  tryATcommand("AT+CMGF=1", "OK", 1000, 20);          // change to text mode
  tryATcommand("AT+CSMP=17,167,0,0", "OK", 1000, 20); // ???
  tryATcommand("AT+GPS=1", "OK", 1000, 20);           // Initialize GPS
}

void keepcommunication()
{
  // TODO how to check if board is working properly
  // checktriggered();
  while (Serial.available())
  {
    Serial2.println(Serial.readString());
  }
  while (Serial2.available())
  {
    parseData(Serial2.readString());
  }
}

void parseData(String replyfromA9G)
{
  Serial.print("[in parseData] Got reply from A9G: ");
  Serial.println(replyfromA9G);

  unsigned int len, index;
  //---------------------------------------------------------
  // Remove sent "AT Command" from the response string.
  index = replyfromA9G.indexOf("\r");
  replyfromA9G.remove(0, index + 2);
  replyfromA9G.trim();
  //---------------------------------------------------------

  if (replyfromA9G != "OK")
  {
    index = replyfromA9G.indexOf(":");
    String cmd = replyfromA9G.substring(0, index);
    cmd.trim();

    replyfromA9G.remove(0, index + 2);
    // MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
    if (cmd == "+CMTI")
    {
      // get newly arrived SMS and store it in temp
      index = replyfromA9G.indexOf(",");
      String temp = replyfromA9G.substring(index + 1, replyfromA9G.length());
      Serial.print("sms temp =");
      Serial.println(temp);
    }
    else if (cmd == "+CMGR")
    {
      // TODO configure message parsing
      // extractSms(replyfromA9G);
      Serial.println(cmd);
    }
    else if (cmd == "+CME ERROR")
      Serial.println("Error has occured");
    Serial.println(cmd);
    ATOK = false;
    wait(1000);
    ATOK = true;
  }
  //---------------------------------------------------------
  else
  {
    // The result of AT Command is "OK"
    Serial.println("Data was not parsed.");
    ATOK = true;
  }
  //---------------------------------------------------------
}
