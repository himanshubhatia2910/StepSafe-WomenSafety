#define smsbutton D5
int buttonState = 0;
int lastButtonState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(smsbutton,INPUT);
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(smsbutton);
  if(buttonState!=lastButtonState){
    if(buttonState == HIGH){
      // do the thing
      Serial

    }
  }
  lastButtonState = buttonState;
  delay(1000);
}
