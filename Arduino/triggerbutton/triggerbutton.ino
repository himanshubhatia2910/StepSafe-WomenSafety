int lastTriggerButtonState = LOW;
int lastSafeButtonState = LOW;
boolean unsafe = false,unsafeFlag = false;
unsigned long triggertime;
#define TRIGGER_BUTTON 22
#define SAFE_BUTTON 23
void setup()
{
    Serial.begin(115200);
    pinMode(TRIGGER_BUTTON, INPUT);
    pinMode(SAFE_BUTTON, INPUT);
}
void loop()
{
    Serial.println(unsafe);
    if(unsafeFlag && millis()-triggertime>15000){
      unsafe = true;
      unsafeFlag = false;
    }
    

    int triggerButtonState = digitalRead(TRIGGER_BUTTON);
    int safeButtonState = digitalRead(SAFE_BUTTON);
    if (triggerButtonState != lastTriggerButtonState || safeButtonState!=lastSafeButtonState)
    {
        if (triggerButtonState == HIGH && safeButtonState == LOW)
        {
            Serial.println("TRIGGER Button CAUGHT");
            triggertime = millis();
            unsafeFlag = true;
        }
        else if (triggerButtonState == LOW && safeButtonState == HIGH)
        {
            Serial.println("Safe removal");
            unsafe = false;
        }
        lastTriggerButtonState = triggerButtonState;
        lastSafeButtonState = safeButtonState;
    }
    delay(100);
}
