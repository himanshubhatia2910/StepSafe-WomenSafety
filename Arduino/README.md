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
