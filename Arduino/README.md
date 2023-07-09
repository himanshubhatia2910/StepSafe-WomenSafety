# Hardware Documentation

1. [Documentation of A9](https://ai-thinker-open.github.io/GPRS_C_SDK_DOC/en/hardware/a9.html)
2. [Ai-Thinker Documentation](https://docs.ai-thinker.com/en/gprs)
3. [Development Tool List](https://docs.ai-thinker.com/en/%E5%BC%80%E5%8F%91%E5%B7%A5)
4. [USB Serial Debugger](https://docs.ai-thinker.com/en/%E5%BC%80%E5%8F%91%E5%B7%A5)
5. [AT commands Documentation](https://github.com/SayuriYuto/StepSafe-WomenSafety/Arduino/AI-ThinkerA9CommandsDocumentation.md)


# Components used
- [ESP32](https://www.amazon.in/dp/B075YSNVPJ?ref_=cm_sw_r_cp_ud_dp_QDJGJ61BGQEMWSGKBM2G)
- [AI Thinker A9](https://www.techiesms.com/product/ai-thinker-a9g-development-board/)
- [Battery](https://www.amazon.in/Floricx-Rechargeable-Battery-Capacity-Performance/dp/B0C27XZQBK/ref=sr_1_4?keywords=18650&sr=8-4)
- [TP4056](https://www.amazon.in/Electronic-Converter-Overcharge-Over-Current-Protection/dp/B082MPDWRD/ref=sr_1_2?keywords=tp+4056&sr=8-2)
- [Limit Switch](https://www.amazon.in/Electronicspices-10pcs-Momentary-Roller-Switch/dp/B08KFLLJ54/ref=sr_1_5?keywords=limit+switch&sr=8-5)
- [Push Button](https://www.amazon.in/Electronicspices-Momentary-Tactile-Button-Assortment/dp/B084KMFKPZ/ref=sr_1_44?keywords=push+button&sr=8-44)

# Steps to communicate to AI-Thinker A9

1. Make connections as per pic<br/>
   <img src="https://github.com/himanshubhatia2910/StepSafe-WomenSafety/blob/master/Arduino/images/connections/connections.jpg" alt="connections" width="200"/><br>
   <img src="https://github.com/himanshubhatia2910/StepSafe-WomenSafety/blob/master/Arduino/images/connections/ESP32-Pinout.png" alt="esp32pindiagram"/><br/>
   TX = 17 (TX2)
   RX = 16 (RX2)
2. Set PORT
3. Set communication as "Both NL & CR".

# System Block Diagram
<img src="https://github.com/SayuriYuto/StepSafe-WomenSafety/blob/master/Arduino/images/system/StepSafeBlockdiagram.drawio.png" alt="Block Diagram"><br/>
<e>System Block Diagram</e><br/>
<img src="https://github.com/SayuriYuto/StepSafe-WomenSafety/blob/master/Arduino/images/system/prototype cirtuirty.png" alt="prototypecircuitry"/><br/>
<e>Prototype circuitry</e><br/>
<img src="https://github.com/SayuriYuto/StepSafe-WomenSafety/blob/master/Arduino/images/system/internal%20circuitry%20of%20StepSafe.png" alt="internalcircuitry"/><br/>
<e>Internal circuitry of Shoe</e>

# Variable Reference for [file](https://github.com/SayuriYuto/StepSafe-WomenSafety/blob/master/Arduino/SMSControlFromNodeMCU/SMSControlFromNodeMCU.ino)
| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `WIFI_SSID` | `string` | **Required**. Enter WiFi SSID in <secrets.h> |
| `WIFI_PASSWORD` | `string` | **Required**. Enter WiFi password in <secrets.h> |
| `API_KEY` | `string` | **Required**. Enter Firebase project api key in <secrets.h> |
| `USER_EMAIL` | `string` | **Required**. Enter Firebase project auth email in <secrets.h> |
| `USER_PASSWORD` | `string` | **Required**. Enter Firebase project auth password in <secrets.h>|
| `senderNumber` | `string` | **Required**. Enter Default SOS phone-number |

## [YouTube Tutorial](https://www.youtube.com/watch?v=Yi5dgZBiq7g)
