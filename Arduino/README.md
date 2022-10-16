# Related to AI-Tinker A9
1. [Documentation of A9](https://ai-thinker-open.github.io/GPRS_C_SDK_DOC/en/hardware/a9.html)
2. [Ai-Thinker Documentation](https://docs.ai-thinker.com/en/gprs)
3. [Development Tool List](https://docs.ai-thinker.com/en/%E5%BC%80%E5%8F%91%E5%B7%A5)
4. [USB Serial Debugger](https://docs.ai-thinker.com/en/%E5%BC%80%E5%8F%91%E5%B7%A5)

Youtube [tutorial](https://www.youtube.com/watch?v=Yi5dgZBiq7g) refered

# Steps to communicate to AI-Thinker A9
1. Make connections as per pic ![connections](https://github.com/himanshubhatia2910/StepSafe-WomenSafety/blob/master/Arduino/images/connections/connections.jpg)
2. Set PORT 
3. Set communication as "Both NL & CR".

## AT Commands used:
### Calling feature
AT
ATI : Info about manufacturer.
AT+COPS=? : Cell towers connected to.
ATD+91\<number>; : Call /<number/> from board.
ATA : Pick up the call.
AT+CHUP : Hang up the call.
### SMS feature
Some one time configs:
AT+CMGF=1 : Receive sms in text format.
AT+CSMP=17,167,0,0 

#### Receive SMS
Send sms to the sim card in the board
#### Send SMS
AT+CMGS="+91\<number>" <br>
\>> \<Enter the message> 
\<press ESC or Ctrl+Z to send>

#### GPS feature
AT+GPS=1
AT+LOCATION=2 : To get the Latitude and Longitude
AT+GPS=0 : TURN OFF GPS
#### Lower power GPS
AT+GPSLP=2

#### Sleep Mode /[GPIO25 TO GND when putting to sleep, remove to put to sleep]
AT+SLEEP=1

# TO DO
- [x] Read from AI Thinker
- [ ] Set baudrate of SI Thinker
- [ ] check if there is I2C or SPI in the A9G board.


message "Hello"
```
+CIEV: "MESSAGE",1
+CMT: ,24
0791191973553620040C9119392793016500002201618154802205C8329BFD06
```
