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
// bool GPSFixed = false;
// bool ATOK = false;
// bool gpscall = false;

String msg, latitude, longitude;
String senderNumber = "+919372391056,+917499599400,";

void setup()
{
  Serial.begin(115200);
  Serial.println("Arduino serial initialize");
  Serial2.begin(115200);
  Serial.println("A9G software serial initialize");
  // ---------------------------------------------------
  pinMode(RESET_PIN, OUTPUT);
  pinMode(builtin_led, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(TRIGGER_BUTTON, INPUT);
  powerOn();
  setupA9G();
}

// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
void loop()
{
  keepcommunication();
  // statusLED();
}

// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

/*@result
true : triggered,
false : not triggered*/
bool checktriggered()
{
  int read = digitalRead(TRIGGER_BUTTON);
  if (read == HIGH)
  {
    // alertSMS();
    return true;
  }
  return false;
}

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

void alertSMS(String msg)
{
  // TODO Make thsi work
  Serial.println("Inside AlertSMS function");
  int startIndex = 0;
  int endIndex = senderNumber.indexOf(",", startIndex);
  while (endIndex != -1)
  {
    String number = senderNumber.substring(startIndex, endIndex);
    Serial2.println("AT+CMGS=\""+number+"\"\r");
    // Serial2.printf("AT+CMGS=\"%s\"\r\n", number);  
    updateSerial();
    Serial2.println(msg);
    updateSerial();
    Serial2.println((char)26);
    // updateSerial();
    if (getResponse("+CMGS:",5000))
      Serial.println("Message sent to " + number);
    else
      Serial.println("Error occured while sending message to "+number);
    startIndex = endIndex + 1;
    endIndex = senderNumber.indexOf(",", startIndex);
  }
}

// check if the expected response is received
void updateSerial()
{
  while (Serial.available())
  {
    Serial2.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read()); // Forward what Software Serial received to Serial Port
  }
  wait(2000);
}

void wait(int millisec)
{
  unsigned long previous = millis();
  while (millis() - previous < millisec)
    ;
}

void powerOn()
{
  // TODO redefine for failure resting board
  // Resetting the A9G
  digitalWrite(RESET_PIN, LOW);
  wait(100);
  digitalWrite(RESET_PIN, HIGH);
  Serial.println("Initializing powerOn");
  while (!getResponse("READY"))
  {
    Serial.print(".");
  }
  Serial.println("Got READY from A9G");
  tryATcommand("AT\r", "OK", 1000, 20, true);
}

void setupA9G()
{
  Serial.println("Initializing setupSMS");
  tryATcommand("AT+CMGF=1", "OK", 1000, 20);          // change to text mode
  tryATcommand("AT+CSMP=17,167,0,0", "OK", 1000, 20); // ???
  tryATcommand("AT+GPS=1", "OK", 1000, 20);           // Initialize GPS
}

void keepcommunication()
{
  // TODO how to check if board is working properly
  wait(15000);
}

// TODO make so that every reply from A9G is parsed.
void parseData(String replyfromA9G)
{
  Serial.println("[in parseData] Got reply from A9G: ");
  Serial.println(replyfromA9G);
  Serial.println("Print complete");

  unsigned int len, index;
  //---------------------------------------------------------
  // Remove sent "AT Command" from the response string.
  index = replyfromA9G.indexOf("\r");
  replyfromA9G.remove(0, index + 2);
  replyfromA9G.trim();
  //---------------------------------------------------------

  if (replyfromA9G != "OK")
  {
    // reply of location coordinates doesnot include ":"
    index = replyfromA9G.indexOf(":");
    if (index == -1)
    {
      Serial.println("Checking if gps coordinates");
      index = replyfromA9G.indexOf(",");
      latitude = replyfromA9G.substring(0, index);
      longitude = replyfromA9G.substring(index + 1, replyfromA9G.length());
      msg = "StepSafe Shoe has been triggered\nThe last known coordinates tracked by the shoe are:\n";
      msg = msg + "Latitude: " + latitude;
      msg = msg + "\nLongitude: " + longitude;
      msg += "\nhttps://www.google.com/maps/search/?api=1&query=";
      msg += latitude;
      msg += ",";
      msg += longitude;
      // variable msg contains the message to be sent.
      Serial.println("Sending SMS to contact number");
      alertSMS(msg);
    }

    // String cmd = replyfromA9G.substring(0, index);
    // cmd.trim();
    // replyfromA9G.remove(0, index + 1);
    // if (cmd == "+CMT")
    // {
    //   // get newly arrived SMS and store it in temp
    //   index = replyfromA9G.indexOf(",");
    //   String temp = replyfromA9G.substring(index + 1, replyfromA9G.length());
    //   Serial.print("sms temp =");
    //   Serial.println(temp);
    //   // TODO get configuration setting from here
    // }
    // else if (cmd == "+CMGR")
    // {
    //   // TODO configure message parsing
    //   // extractSms(replyfromA9G);
    //   Serial.println("+CMGR");
    //   Serial.println(cmd);
    // }
    // else if (cmd == "+CME ERROR")
    // {
    //   Serial.println("Error has occured");
    //   Serial.println(cmd);
    //   // ATOK = false;
    //   // wait(1000);
    //   // ATOK = true;
    // }
  }
  //---------------------------------------------------------
  else
  {
    // The result of AT Command is "OK"
    Serial.println("Data was not parsed.");
    // ATOK = true;
  }
  //---------------------------------------------------------
}
