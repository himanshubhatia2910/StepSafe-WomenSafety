# Things learned
## Making secret.h
Use #define to state keys
```
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASS"
#define API_KEY "<API_KEY>"
#define FIREBASE_PROJECT_ID "<Document_Id>"
#define USER_EMAIL "<EMAIL>"
#define USER_PASSWORD "<PASS>"
```

## FIREBASE QUERY using FirebaseLibrary
Get project documentPath<br>
<img src="https://github.com/himanshubhatia2910/StepSafe-WomenSafety/blob/master/Arduino/images/firebase/documentpath.jpg" alt="documentpath" width="200"/>

`content.set("fields/<Key>/<valueType>,<value>)`<br>
`<valueType>`can be as follows:<br>
State value as string
* `doubleValue`
* `booleanValue`
* `integerValue`
* `nullValue`
* `referenceValue`
* `timestampValue`
* `arrayValue/values/[n]/<valueType>`
* `mapValue`
* `stringValue`
* `geoPointValue/latitude` : double
* `geoPointValue/longitude` : double

