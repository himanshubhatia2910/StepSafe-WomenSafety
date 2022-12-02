#include <Arduino.h>
#line 1 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
// TxRx pins on ESP32
// TX = 17
// RX = 16
#define RESET_PIN 21
#define builtin_led 2
#define TRIGGER_BUTTON 22
#define SAFE_BUTTON 23
#define RED_LED 5
#define GREEN_LED 18
#define BLUE_LED 19

// FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// FLAGS
bool GPSFixed = false;
bool ATOK = false;
bool gpscall = false;
boolean unsafe = false;

String msg, latitude = "0", longitude = "0", temp, lastcoordinates = "";
String senderNumber = "+919372391056,";

int lastTriggerButtonState = LOW;
int lastSafeButtonState = LOW;
boolean unsafeFlag = false;
unsigned long triggertime;
unsigned long dataMillis = 0;
unsigned long lastsmsmillis = 0;
int alertDelay = 60000; // 3600000 hourly
unsigned long sendDataPrevMillis = 0;

// ------------------------------------------------------------
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include "secrets.h"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
// ------------------------------------------------------------

#line 40 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
void setup();
#line 61 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
void loop();
#line 77 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
void checktriggered();
#line 109 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
void getGPS();
#line 209 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
void noparseupdate();
#line 217 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
void alertSMS(String latitude, String longitude);
#line 250 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
void updateSerial();
#line 264 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
void wait(int millisec);
#line 271 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
void powerOn();
#line 300 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
void resetA9G();
#line 308 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
void setupA9G();
#line 316 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
void parseData(String replyfromA9G);
#line 379 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
String getTime();
#line 407 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
void fcsUploadCallback(CFS_UploadStatusInfo info);
#line 431 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
void firebaseSetup();
#line 459 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
boolean fetchEmergencyContact();
#line 486 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
void firebaseSend(String currenttime, String latitude, String longitude);
#line 40 "d:\\StepSafe\\StepSafe-WomenSafety\\Arduino\\SMSControlFromNodeMCU\\SMSControlFromNodeMCU.ino"
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
  pinMode(SAFE_BUTTON, INPUT);
  // ---------------------------------------------------
  powerOn();
  setupA9G();
  firebaseSetup();
}

// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
void loop()
{
  checktriggered();
  if (unsafe == true && ((millis() - lastsmsmillis) > alertDelay || lastsmsmillis == 0))
  {
    getGPS();
    lastsmsmillis = millis();
  }
  updateSerial();
}

// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

/*@result
true : triggered,
false : not triggered*/
void checktriggered()
{
  // TODO check for false triggers by leg
  if (unsafeFlag && millis() - triggertime > 15000)
  {
    Serial.println("Emergency!!!");
    unsafe = true;
    unsafeFlag = false;
  }
  else
  {
    int triggerButtonState = digitalRead(TRIGGER_BUTTON);
    int safeButtonState = digitalRead(SAFE_BUTTON);
    if (triggerButtonState != lastTriggerButtonState || safeButtonState != lastSafeButtonState)
    {
      if (triggerButtonState == HIGH && safeButtonState == LOW)
      {
        Serial.println("TRIGGER Button CAUGHT");
        triggertime = millis();
        unsafeFlag = true;
      }
      else if (safeButtonState == HIGH)
      {
        Serial.println("Safe removal");
        unsafeFlag = false;
      }
      lastTriggerButtonState = triggerButtonState;
      lastSafeButtonState = safeButtonState;
    }
  }
}

void getGPS()
{
  String replyfromA9G;
  gpscall = true;
  noparseupdate();
  Serial2.println("AT+LOCATION=2\r");
  Serial.println("Get gps called");
  wait(2000);
  while (Serial2.available())
  {
    replyfromA9G = Serial2.readString();
  }
  Serial.println("\n\n------------------------------\n" + replyfromA9G + "\n------------------------------");
  int index = replyfromA9G.indexOf("\r");
  replyfromA9G.remove(0, index + 2);
  replyfromA9G.trim();
  //---------------------------------------------------------
  Serial.println("Checking if gps coordinates");
  if (replyfromA9G.indexOf("GPS NOT FIX NOW") != -1 || replyfromA9G.indexOf("+CME") != -1)
  {
    Serial.println("GPS not fixed caught");
    noparseupdate();
    // Serial.println(replyfromA9G);
  }
  else
  {
    index = replyfromA9G.indexOf(",");
    Serial.println("Found");
    lastcoordinates = latitude + "," + longitude;
    latitude = replyfromA9G.substring(0, index);
    longitude = replyfromA9G.substring(index + 1, replyfromA9G.length());
    Serial.println((String)latitude + (String)longitude);
  }
  alertSMS(latitude, longitude);
  temp = latitude + "," + longitude;
  if (!temp.equals(lastcoordinates))
  {
    Serial.println("Co-Ordinates changed!");
    firebaseSend(getTime(), latitude, longitude);
  }
  gpscall = false;
  Serial.println("--------------------End getGPS-------------------------");
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

void alertSMS(String latitude, String longitude)
{
  Serial.println("Inside AlertSMS function");
  String msg = "!!!!!!!!!! SOS !!!!!!!!!!\nI am in an emergency. Here's my location:\n";
  msg = msg + "Latitude: " + latitude;
  msg = msg + "\nLongitude: " + longitude;
  msg += "\nhttps://www.google.com/maps/search/?api=1&query=";
  msg += latitude;
  msg += ",";
  msg += longitude;

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
  // TO CARRY FORWARD THE COMMANDS FROM SERIAL
  while (Serial.available())
  {
    Serial2.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())
  {
    parseData(Serial2.readString());
  }
  // wait(2000);
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
  // ---------------------------------------------------------
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
  // else if (gpscall)
  // {
  //   Serial.println("Checking if gps coordinates");
  //   if (replyfromA9G.indexOf("GPS NOT FIX NOW") != -1)
  //   {
  //     Serial.println("GPS not fixed caught");
  //     noparseupdate();
  //     Serial.println(replyfromA9G);
  //     //      gpscall = false;
  //     //      return;
  //   }
  //   index = replyfromA9G.indexOf(",");
  //   Serial.println("Found");
  //   GPSFixed = true;
  //   latitude = replyfromA9G.substring(0, index);
  //   longitude = replyfromA9G.substring(index + 1, replyfromA9G.length());
  //   // variable msg contains the message to be sent.
  //   alertSMS(latitude, longitude);
  //   temp = latitude + "," + longitude;
  //   if (!temp.equals(replyfromA9G))
  //   {
  //     firebaseSend(getTime(), latitude, longitude);
  //   }
  //   Serial.println("Co-Ordinates didnot change!");
  //   gpscall = false;
  // }
  else if (replyfromA9G.indexOf("+CME ERROR:") != -1)
  {
    Serial.println("Error has occured");
    Serial.println(replyfromA9G);
  }
  else if (replyfromA9G.indexOf("RING") != -1)
  {
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
String getTime()
{
  String currenttime = "", replyfromA9G;
  noparseupdate();
  Serial2.println("AT+CCLK?");
  Serial.println("gettime");
  wait(2000);
  while (Serial2.available())
  {
    replyfromA9G = Serial2.readString();
  }
  replyfromA9G.trim();
  if (replyfromA9G.indexOf("+CCLK:") >= 0)
  {
    currenttime = replyfromA9G;
    int index = currenttime.indexOf(":");
    currenttime.remove(0, index + 1);
    currenttime.remove(0, currenttime.indexOf('"') + 1);
    currenttime.remove(currenttime.indexOf('"'), currenttime.length());
    for (int i = 0; i < 2; i++)
    {
      currenttime.replace('/', '-');
    }
    Serial.println("Captured time: " + currenttime);
  }
  return currenttime;
}

void fcsUploadCallback(CFS_UploadStatusInfo info)
{
  if (info.status == fb_esp_cfs_upload_status_init)
  {
    Serial.printf("\nUploading data (%d)...\n", info.size);
  }
  else if (info.status == fb_esp_cfs_upload_status_upload)
  {
    Serial.printf("Uploaded %d%s\n", (int)info.progress, "%");
  }
  else if (info.status == fb_esp_cfs_upload_status_complete)
  {
    Serial.println("Upload completed ");
  }
  else if (info.status == fb_esp_cfs_upload_status_process_response)
  {
    Serial.print("Processing the response... ");
  }
  else if (info.status == fb_esp_cfs_upload_status_error)
  {
    Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
  }
}

void firebaseSetup()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    wait(300);
  }
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  fbdo.setResponseSize(2048);
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  config.timeout.serverResponse = 10 * 1000;
  // Firebase.setDoubleDigits(5);
  while (true)
  {
    if (fetchEmergencyContact())
      break;
    else
      Serial.println("~");
  }
}

// TODO fetch emergency contacts
boolean fetchEmergencyContact()
{
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    Serial.printf("Get string... %s\n", Firebase.RTDB.getString(&fbdo, F("/test/string")) ? fbdo.to<const char *>() : fbdo.errorReason().c_str());

    if (Firebase.RTDB.getString(&fbdo, F("/Users/7a5Jza5gO3bB4OBP0rAMRzfCXJB3/emergency_contact")))
    {
      String fetchedcontacts = fbdo.to<const char *>();
      if (senderNumber.indexOf(fetchedcontacts) == -1)
      {
        senderNumber.concat("+91" + fetchedcontacts + ",");
        Serial.println(senderNumber);
        return true;
      }
      else
      {
        Serial.println("Contact already in list!");
      }
    }
    else
      Serial.println(fbdo.errorReason().c_str());
  }
  return false;
}

void firebaseSend(String currenttime, String latitude, String longitude)
{
  while (Firebase.ready() && gpscall)
  {
    if (millis() - dataMillis > 30000 || dataMillis == 0)
    {
      dataMillis = millis();
      FirebaseJson content;
      String documentPath = "Location/History/up8ERAvfOlWkxNXp4vZ53fdL6eF2/" + currenttime;
      String doc_path = "projects/";
      doc_path += FIREBASE_PROJECT_ID;
      doc_path += "/databases/(default)/documents/coll_id/doc_id"; // coll_id and doc_id are your collection id and document id
      content.set("fields/myLatLng/geoPointValue/latitude", latitude.toDouble());
      content.set("fields/myLatLng/geoPointValue/longitude", longitude.toDouble());
      if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw()))
      {
        Serial.println("Updated in Firebase!");
        lastcoordinates = latitude + "," + longitude;
        break;
      }
      else
        Serial.println(fbdo.errorReason() + "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }
  }
}

