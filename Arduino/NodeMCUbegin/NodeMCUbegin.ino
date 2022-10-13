#define smsbutton D5
int buttonState = 0;
int lastButtonState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(smsbutton,INPUT);
  Serial.begin(115200);
  Serial2.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(Serial.available()){
    int inByte = Serial.read();
//    Serial.println("This is in Serial");
    Serial2.write(inByte);
    
  }

  if(Serial2.available()){
    int inByte = Serial2.read();
//    Serial2.println("This is in Serial2");
    Serial.write(inByte);
  }

  buttonState = digitalRead(smsbutton);
  if(buttonState!=lastButtonState){
    if(buttonState == HIGH){
      // do the thing
      SendMessage();

    }
  }
  lastButtonState = buttonState;
  delay(1000);  
}

void SendMessage(){
  Serial.println("Setting the GSM in text mode");
  Serial2.println("AT+CMGF=1\r");
  Serial.println("AT+CMGF=1\r");

  delay(2000);
  Serial.println("Sending SMS to the desired phone number!");
  Serial2.println("AT+CSMP=17,167,0,0\r");
  Serial.println("AT+CSMP=17,167,0,0\r");

  delay(2000);
  Serial.println("Sending SMS to the desired phone number!");
  Serial2.println("AT+CMGS=\"+917887575773\"\r");
  Serial.println("AT+CMGS=\"+917887575773\"\r");

  delay(2000);
  Serial2.println("Hello from A9G");
  Serial.println("Hello from serial");
  delay(2000);

  Serial2.println((char)26);
  Serial.println((char)26);
  // delay(10000);
}
