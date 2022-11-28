# Related to AI-Tinker A9

1. [Documentation of A9](https://ai-thinker-open.github.io/GPRS_C_SDK_DOC/en/hardware/a9.html)
2. [Ai-Thinker Documentation](https://docs.ai-thinker.com/en/gprs)
3. [Development Tool List](https://docs.ai-thinker.com/en/%E5%BC%80%E5%8F%91%E5%B7%A5)
4. [USB Serial Debugger](https://docs.ai-thinker.com/en/%E5%BC%80%E5%8F%91%E5%B7%A5)

Youtube [tutorial](https://www.youtube.com/watch?v=Yi5dgZBiq7g) refered

# Steps to communicate to AI-Thinker A9

1. Make connections as per pic
   <img src="https://github.com/himanshubhatia2910/StepSafe-WomenSafety/blob/master/Arduino/images/connections/connections.jpg" alt="connections" width="200"/>
   <img src="https://github.com/himanshubhatia2910/StepSafe-WomenSafety/blob/master/Arduino/images/connections/ESP32-Pinout.png" alt="esp32pindiagram"/>
   TX = 17 (TX2)
   RX = 16 (RX2)
2. Set PORT
3. Set communication as "Both NL & CR".

## AT Commands used:

`AT+CSQ` : Get signal strength<br>
`AT+CCID` : read the ICCID from the SIM.<br>
`AT+RST=X `<br>
X=1 means soft restart,
X=2 means shutdown <br>

```
+CREG: 2 +CTZV: 18/03/30, 06:59:43, +08
+CREG: 1 +CTZV: 18/03/30, 06:59:44, +08
OK
A9/A9G V02.00.20180327RC
Ai_Thinker_Co.LTD
READY
```

## ===== Phone reception ====

Related instructions `ATD\<xxxxx>`
xxx is the phone number

```
OK
+CIEV: “CALL”,1 +CIEV: “SOUNDER”,1 Bell
+CIEV: “SOUNDER”,0 Hang up +CIEV: “CALL”,0 BUSY
```

Display BUSY
Receive a short message (If you want to use the call reminder function,
Please use `AT+CLIP=1`; note that this function must require your SIM card to bring the call reminder function) RING RING

## ===== SMS settings =====

`AT+CPMS=“SM”,“SM”,“SM”` Set the SMS storage unit <br>
`AT+CMGF=0/1` Set the message format, 1 is text format to read, 0 is pud format to read <br>
`AT+CMGR=x` Read the message content, x is the first few messages<br>
`AT+CMGL=4/“ALL”` View the list of short messages <br>
Read parameter 4 in PUD format, send `AT+CMGL=“ALL”` in txt format
`AT+CMGD=1` Delete SMS. <br>
If `AT+CMGD=1,4` ​​is used to delete all SMS <br>
Example To `AT+CPMS=“SM”,“SM”,“SM”` Set the SMS storage unit, and you can also check the SMS capacity

```
+CPMS: 0,50,0,50,0,50 OK
```

To `AT+CMGF=1` Set to TXT format to read `OK`<br>
Format : `+CIEV: “MESSAGE”,1 SMS received, including phone number, time and content `

```
+CMT: “+86xxxxxxxxxxx”,,“2017/10/09,09:14:52+08” 111
+CIEV: “MESSAGE”,1
+CMT: “+86xxxxxxxxxxx”,,“2017/10/09,09:16:25+08” 222
```

`AT+CMGF=0` Set to PUD format to accept OK To
`Format: +CIEV: “MESSAGE”,1 Receive SMS with PUD, the content includes phone number, time and content'

```
+CMT: ,23 0891683110602305F0240D91687186935218F500007101909061452303B3D90C To +CIEV: “MESSAGE”,1
+CMT: ,23 0891683110602305F0240D91687186935218F50000710190
```

## List SMS

`AT+CMGL=4` List all SMS → pud format, use this command with parameter 4

```
+CMGL: 1,1,,23 0891683110602305F0240D91687186935218F500007101909041252303B1580C
+CMGL: 2,1,,23 0891683110602305F0240D91687186935218F50000710190906152230332990C
+CMGL: 3,1,,23 0891683110602305F0240D91687186935218F500007101909061452303B3D90C
+CMGL: 4,1,,23 0891683110602305F0240D91687186935218F500007101909071202303341A0D OK
```

## Read SMS

`AT+CMGF=1` Set to TXT format to read. <br>
`AT+CMGL=“ALL”` List all short messages → txt format, use this command parameter as ALL <br>

```
+CMGL: 1,“REC READ”,“+86xxxxxxxxxxx”,,“2017/10/09,09:14:52+08” 111
+CMGL: 2,“REC READ”,“+86xxxxxxxxxxx”,,“2017/10/09,09:16:25+08” 222
+CMGL: 3,“REC READ”,“+86xxxxxxxxxxx”,,“2017/10/09,09:16:54+08” 333
+CMGL: 4,“REC READ”,“+86xxxxxxxxxxx”,,“2017/10/09,09:17:02+08” 444
OK
```

`AT+CMGR=1` Read the first SMS

```
+CMGR: “REC READ”,“+86xxxxxxxxxxx”,,“2017/10/09,09:14:52+08” 111
OK
```

`AT+CMGR=2` Read the second message

```
+CMGR: “REC READ”,“+86xxxxxxxxxxx”,,“2017/10/09,09:16:25+08” 222
OK
```

`AT+CMGR=3` Read the third message

```
+CMGR: “REC READ”,“+86xxxxxxxxxxx”,,“2017/10/09,09:16:54+08” 333
```

## Delete Message

`AT+CMGD=1` Delete the first SMS `OK`<br>
`AT+CMGD=2` Delete the second message. <br>

Remember that after deleting the first message, the content of the second message will not automatically go to the first message, but still occupy the second message, but when all messages are deleted, After receiving the SMS, it will be automatically arranged to the position of the first message `OK`

`AT+CMGD=3` Delete the third SMS `OK`<br>
`AT+CPMS=“SM”,“SM”,“SM”` Check the SMS capacity <br>

```
+CPMS: 1,50,1,50,1,50
OK
```

`AT+CMGL=“ALL”` After deleting 3 messages, check the message list, only 1 message remains

```
+CMGL: 4,“REC READ”,“+86xxxxxxxxxxx”,,“2017/10/09,09:17:02+08” 444
OK
```

`AT+CMGD=1,4` ​​Delete all SMS `OK`

## Send messages

`AT+CMGF=1` Send in text mode<br>
`AT+CMGS=“+<countrycode><telephone number>” <Enter>`, then enter the content of the txt to be sent, send the data, and then cancel the “send new line” option, check the “HEX (hexadecimal sending)” option, Finally, send `0x1a` (Ctrl+Z).<br>
Send SMS in Chinese<br>
In addition, you can use PUD encoding to send information:
`AT+CMGS=XX` The number of bytes to be sent

## ====== Location ======

`At+LOCATION=X` (simplified version)<br>
Note that before using the base station positioning function, you must first <br>
`AT+CGATT=1;`<br>
`AT+CGDCONT=1,“IP”,“cmnet”;`<br>
`AT+CGACT=1,1;`<br>
The three commands can be executed after the network is attached and activated.

Example: Pre-optimization instructions → `AT+CREG=1` =1 is to query the network status, the default value is 1 when the module is powered on `OK` `AT+CREG?`

```
+CREG: 1,1
```

`AT+CREG=2` =2 is the base station positioning `OK` `AT+CREG?`

```
+CREG: 2,1,“xxxx”,“xxxx”,2
```

The intermediate parameter is hexadecimal, the first string is LAC, and the second string is CI.

Use `AT+LOCATION=1` alone, no need to set <br>`AT+CREG=2`, <br>`AT+LOCATION=1`

```
(return:<latitude>,<longitude>OK)
xx.xxxxx, xxx.xxxxxx
(fixed to 6 digits after the decimal point)
OK
```

Return latitude and longitude directly without conversion The command →`at+location` needs to be connected to the Internet to obtain geographic location information, so the length of time depends on the quality of the network, and if there is no manual attachment and activation before use, it will automatically do these two things. It will take longer. If you want to be reliable and fast, it is recommended to use <br>`at+cgatt=1;` <br>`at+cgdcont=1,“IP”,“cmnet”;at+cgact=1,1;` <br>
Attach activation and ensure the ability to connect to the Internet

## ===== Get the time issued by the base station =====

Related command `AT+CCLK?` <br>
Instruction Interpretation <br>
The instruction is processed internally. When the module is initialized, after the module obtains the time issued by the base station, the time is stored in the RTC register of the chip, and the time can be read by using AT+CCLK?.
Note that there is a problem with the time obtained with a Unicom card.
Example

```
AT+CCLK?
+CCLK: “17/10/07,02:50:55+08”
```

The time is zero time zone time, at 2:50:55 on October 7, 2017.
+08 is the time zone, we are here in Dongba District, so the local time is 10:50:55 `OK`

## ====== HTTP REQUEST ======

`AT+HTTPGET=<url>` Uniform resource identifier, which can be a domain name or IP address <br>
`AT+HTTPPOST=<url>,<content_type>,<body_content> <content_type>` : network file type and web page encoding content type `<body_content>:` body text <br>
Example 
`AT+CGATT=1` Attach to the network, if you need to go online, this command is required

```
+CGATT:1 OK
```

`AT+CGDCONT=1,“IP”,“CMNET”` Set PDP parameters

```
OK
```

`AT+CGACT=1,1` Activate PDP, you can go online after it is activated correctly <br>

```
OK
```

`AT+HTTPGET=“http://www.google.com”` Connect to the website and request website resources

```
OK
```

The next message received is the response from the server

## ===== MQTT =====

Example: `AT+CGATT=1` Attach to the network

```
OK
```

`AT+CGDCONT=1,"IP","CMNET"` //Set PDP parameters

```
OK
```

`AT+CGACT=1,1` //Activate the PDP, you can go online after the correct activation

```
OK
```

`AT+MQTTCONN="www.anthinkerwx.com",1883,"12345",120,0,"Ai-thinker","123456"` //The client waits and connects to the server, and sends CONNECT at the same time

```
OK
```

// Note: (MQTT server needs to be built by yourself)
`AT+MQTTPUB="test","124563",0,0,0` //The client transmits an application message to the server

```
+MQTTPUBLISH: 1, test, 6, 124563
OK
```

`AT+MQTTSUB="test",1,0` //The client sends a SUB message to the server to create a subscription

```
OK
```

`AT+MQTTDISCONN` //The DISCONNECT control message sent by the client to the server indicates that the client is normally disconnected

## ====== GPS Settings ======

Related instructions `AT+GPS=1`, turn on GPS <br>
`AT+GPS=0`, turn off GPS <br>
`AT+GPS?`, query GPS status <br>
`AT+GPSRD=N`, N is a number, it means output a NEMA message from AT serial port in N seconds. <br>
`AT+LOCATION=X` <br>
`X=1` is the address of the base station, `X=2` is the address of the GPS <br>
To Note that after the GPS is turned on, the GPS_TXD of the module will output positioning information by default at a baud rate of 9600. This does not affect the baud rate of the entire module.
For example, the working baud rate of the A9G module is 115200, and the AT command is sent to turn on the GPS. At this time, GPS_TXD will output information at 9600 baud rate, and the AT serial port is still 115200. To In addition, please note that the positioning information will output GNGGA GNRMC instead of GPGGA and GPRMC. The module uses a hybrid positioning system, including GPS, BDS and GNNS (Global Navigation System). Explain the composition of the output information header, for example, GPGGA, split into GP+GGA→ positioning system + output data type, GP is the GPS positioning system, GGA is the output information of time, position, and number of satellites, similarly GNGGA is GN +GGA→Positioning system + output data type, GN is the global navigation system, GGA is the output information of time, position, and number of satellites, other instructions please follow this order. Finally, the module defaults to GPS+BDS mixed mode output, then the output information is the data at the beginning of GN, if it is set to single mode, it is the data output at the beginning of the mode, for example, if it is set to GPS mode, the output information is GPGGA.<br>
Example `AT+GPS=1` Enable GPS function `OK` <br>
`AT+GPSRD=N` Output a NEMA message in N seconds from AT serial port

```
+GPSRD:
$GNGGA,000840.261,2236.3719,N,11350.4081,E,0,0,,153.2,M,-3.2,M,,*4F $GPGSA,A,1,,,,,,,,,,,,,,,,*1E
$BDGSA,A,1,,,,,,,,,,,,,,,,*0F $GPGSV,1,1,00*79
$BDGSV,1,1,00*68
$GNRMC,000840.261,V,2236.3719,N,11350.4081,E,0.000,0.00,060180,,,N*5C
$GNVTG,0.00,T,,M,0.000,N,0.000,K,N*2C
```

`AT+LOCATION = 2` Get GPS address information, the premise is that GPS can see the satellites before returning, otherwise it will return `GPS NOT FIX NOW` To

Related instructions `AT+GPSMD=1` GPS mode (standalone BD mode is not supported temporarily, bd system only covers Asia temporarily)<br>
`AT+GPSMD=2` GPS+BD mode The module defaults to GPS+BDS mixed mode output, then the output information is the data at the beginning of GN, if it is set to single mode, it is the data output at the beginning of the mode, for example, if it is set to GPS mode, the output information is GPGGA.

Example `AT+GPS=1` Turn on GPS first<br>
`AT+GPSMD=x` x=1 or 2, choose which mode to turn on, the default is GPS+BDS mode<br>
`At+GPSRD=x` Number x means x seconds, output a NEMA message from AT serial port

Related instructions `AT+GPSLP=x` GPS low power settings, the value of x is 0, 1, 2 <br>
`AT+GPSLP=0`
<br>Normal mode —- The current value is tested around 0.063A<br>
`AT+GPSLP=1` Serial port low power consumption mode<br>
`AT+GPSLP=2` GPS low power consumption mode —- The current value is tested at about 0.034A
<br>The default default mode of the module GPS is the normal mode. When the GPS mode is set, it will not take effect immediately, and it will take a while to take effect.

```
Example:
AT+GPS=1 Enable GPS
AT+GPSRD=10 output a NEMA message in 10 seconds
AT+GPSLP=2 Set GPS mode to low power consumption mode
```

`AT+SLEEP=x` Set the low power consumption mode of the module, the value of x is 0,1,2 respectively<br>
`AT+SLEEP=0` Turn off low power consumption mode — The current value into low power consumption is 00.213A <br>
`AT+SLEEP=1` Enter GPIO low power consumption mode, pull GPIO25 pin low to enter module low power consumption, pull high pin to exit low power consumption, GPIO25 defaults to high level<br>
`AT+SLEEP=2` Serial port low power consumption mode, you can wake up through the serial port —- there is another problem

```
Note: Enter the serial port low power consumption mode, in which the serial port can receive and send data, and the module will normally switch between low power consumption and normal mode.
Enter the low power consumption mode of the GPIO port and receive data.
SMS, the phone will not be woken up, need to control the pin to pull high, exit low power consumption mode.
GPIO26 is used to notify GPIO in low-power mode (if you enter GPIO low-power mode, connect the GPIO26 pin with an oscilloscope to detect the level of the GPIO26 pin to determine whether data is received)
```

Example 1 (GPIO low power consumption)
`AT+SLEEP=1` GPIO low power consumption mode `OK` After sending the command successfully, connect the GPIO25 pin to the GND pin, and then continue to send the AT command without any effect:

Example 2 (Serial port low power consumption)
`AT+SLEEP=2` //Serial port low power consumption mode
OK
//When entering the serial port with low power consumption, the current consumed by the module will be reduced, but data can still be sent

```
AT
OK
//When the module sends data, it will automatically exit low power consumption, and the current consumption will increase again

```

## Calling feature

`AT+COPS=?` : Cell towers connected to.<br>
`ATD+91\<number>;` : Call /<number/> from board.
`ATA` : Pick up the call.<br>
`AT+CHUP` : Hang up the call.<br>

### SMS feature

Some one time configs:<br>
`AT+CMGF=1` : Receive sms in text format.<br>
`AT+CSMP=17,167,0,0` : ??? <br>

### Errors

`500` : CTRL + Z not send and unknown error<br>
`58` : Unknown command <br>
`52` : GPS not Fixed <br>

`+CMGS: n` : SMS has been sent.
`+CIEV: "SMSFULL",2` : Inbox is full.

# module init
AT
AT+CPIN?
wait for +CPIN: READY

AT+CFUN=1
AT+CMEE=2
AT+CBATCHK=1
AT+CREG?
wait for +CREG: 0,

AT+CMGF=1
wait for OK

# HTTP init
AT+SAPBR=0,1
AT+SAPBR=3,1,"Contype","GPRS"
AT+SAPBR=3,1,"APN",<APN>
AT+SAPBR=3,1,"USER",<USER>
AT+SAPBR=3,1,"PASS",<PASS>
AT+SAPBR=1,1
AT+SAPBR=2,1

CHECK ID CONNECTED TO INTERNET 
AT+CGATT?
check for =CGATT:1
anything else means not connected
# HTTP request

AT+HTTPINIT
AT+HTTPSSL=1
AT+HTTPPARA="CID",1
AT+HTTPPARA="URL",<FIREBASE_HOST>.json?auth=<FIREBASE_SECRET>
AT+HTTPPARA="REDIR",1
AT+HTTPPARA="CONTENT","application/json"
AT+HTTPDATA=<String(data.length())>,10000
wait for response "download"
send data
AT+HTTPACTION=1
check response

if(response.indexOf("+HTTPACTION:") > 0)
{
Serial.println(response);
break;
}

//+HTTPACTION: 1,603,0 (POST to Firebase failed)
//+HTTPACTION: 0,200,0 (POST to Firebase successfull)
//Read the response
AT+HTTPREAD

stop connection
AT+HTTPTERM

# disconnect gprs
AT+CGATT=0

# TO DO

-   [x] Shift to ESP32. As it may have more than 1 hardware serial ports
-   [x] Get GPS Co-Ordinates
-   [x] Send SMS on button press
-   [x] Send SMS to multiple devices [git](https://github.com/ahmadlogs/nodemcu/blob/main/sim800l-gps-reg-phone/sim800l-gps-reg-phone.ino)
-   [x] Receive call always
-   [x] Send GPS to Firebase
-   [ ] SetUp - customize emergency contacts.
-   [ ] LED Indicator
-   [x] Send SMS only if location changes
-   [ ] if location not fetched, keep fetching
-   [ ] Delete received SMS
-   [ ] Setup safe show removal
-   [ ] sms IS NOT BEING SENT AGAIN
PLEASE CHECK

