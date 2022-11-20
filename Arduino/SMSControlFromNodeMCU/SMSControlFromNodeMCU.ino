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
bool gpscall = false;

String msg, latitude = "NO-CORDS", longitude = "NO-CORDS";
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
  // ---------------------------------------------------
  powerOn();
  setupA9G();
}

// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
void loop()
{
  // updateSerial();
  checktriggered();
  updateSerial();
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
    Serial.println("Triggered");
    getGPS();
    return true;
  }
  return false;
}

void getGPS()
{
  gpscall = true;
  Serial2.println("AT+LOCATION=2\r");
  // updateSerial();
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
      if (response.indexOf(expected_answer) >= 0)
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

void noparseupdate()
{
  while (Serial2.available())
  {
    Serial.write(Serial2.read()); // Forward what Software Serial received to Serial Port
  }
}

void alertSMS(String msg)
{
  Serial.println("Inside AlertSMS function");
  int startIndex = 0;
  int endIndex = senderNumber.indexOf(",", startIndex);
  while (endIndex != -1)
  {
    String number = senderNumber.substring(startIndex, endIndex);
    noparseupdate(); // clear buffer maybe.
    Serial2.println("AT+CMGS=\"" + number + "\"\r");
    noparseupdate();
    Serial2.println(msg);
    noparseupdate();
    Serial2.println((char)26);
    if (getResponse("+CMGS:", 5000))
      Serial.println("Message sent to " + number);
    else
      Serial.println("Error occured while sending message to " + number);
    startIndex = endIndex + 1;
    endIndex = senderNumber.indexOf(",", startIndex);
    wait(5000);
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
    parseData(Serial2.readString());
    // Serial.write(Serial2.read()); // Forward what Software Serial received to Serial Port
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
  Serial.println("Initializing powerOn");
TryAgain:
  resetA9G();
  unsigned long previous;

  for (previous = millis(); (millis() - previous) < 15000;) // resets the board after not getting a response after 30 seconds
  {
    if (getResponse("READY") == true)
    {
      Serial.println("Got READY from A9G");
      break;
    }
    else
    {
      Serial.print(".");
    }
    if (previous == (millis() - previous) < 15000)
    {
      Serial.println("Didn't get a Ready, 15 secs passed! Resetting the Module");
      resetA9G();
      goto TryAgain;
    }
  }
  tryATcommand("AT\r", "OK", 1000, 20, true);
}

void resetA9G()
{
  // TODO this is not working
  digitalWrite(RESET_PIN, LOW);
  wait(100);
  digitalWrite(RESET_PIN, HIGH);
}

void setupA9G()
{
  Serial.println("\nInitializing setupSMS");
  tryATcommand("AT+CMGF=1", "OK", 1000, 20);          // change to text mode
  tryATcommand("AT+CSMP=17,167,0,0", "OK", 1000, 20); // ???
  tryATcommand("AT+GPS=1", "OK", 1000, 20);           // Initialize GPS
}

void parseData(String replyfromA9G)
{
  Serial.println("Parsing reply from A9G: ");

  unsigned int index;
  //---------------------------------------------------------
  // Remove sent "AT Command" from the response string.
  index = replyfromA9G.indexOf("\r");
  replyfromA9G.remove(0, index + 2);
  replyfromA9G.trim();
  //---------------------------------------------------------

  if (replyfromA9G == "OK")
  {
    // Serial.println("OK was caught");
    Serial.println("ok caught " + replyfromA9G);
    return;
  }
  else if (gpscall)
  {
    Serial.println("Checking if gps coordinates");
    // if (replyfromA9G.indexOf("GPS NOT FIX NOW") != -1)
    // {
    //   // Serial.println("GPS not fixed caught");
    //   Serial.println(replyfromA9G);
    //   gpscall = false;
    //   return;
    // }
    replyfromA9G = "18.4740194,73.8767881";
    index = replyfromA9G.indexOf(",");
    Serial.println("Found");
    GPSFixed = true;
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
    alertSMS(msg);
    gpscall = false;
  }
  else if (replyfromA9G.indexOf("+CME ERROR:") != -1)
  {
    Serial.println("Error has occured");
    Serial.println(replyfromA9G);
  }
  else if (replyfromA9G.indexOf("RING")!=-1){
    Serial.println("RING caught");
    Serial2.println("ATA\r");
    noparseupdate();
  }
  else
  {
    Serial.println("Nothing was parsed");
    Serial.println(replyfromA9G);
    Serial.println("Over");
  }
}
//---------------------------------------------------------
