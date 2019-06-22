#define ENABLE_TELEGRAM;          //Disconnect if you do not want notifications by Telegram bot
#define ENABLE_DDNS;              //Disconnect if not use dynamic DNS
//#define ENABLE_TEST_MODE        //If enabled, for send data tests
//#define ENABLE_HELTEC_WIFI_Kit_8  //If defined, enable for compile HELTEC_WIFI_Kit_8

#ifdef ENABLE_HELTEC_WIFI_Kit_8
    #include <Wire.h>
    #include "OLED.h"
    #define RST_OLED 16
    OLED display(4, 5);
    /*
    HELTEC WIFI_Kit_8's OLED connection:
    SDA -- GPIO4 -- D2
    SCL -- GPIO5 -- D1
    RST -- GPIO16 -- D0
    */
#endif


// ***************************************************************************
// IMPORTANT: Enable wifi and OBD fot test or work in Ioniq
// ***************************************************************************
#ifdef ENABLE_TEST_MODE  ///Enable wifi for test in your LAN
    int testLine = 1; 
    const char* ssid = "YourSSIDForTest";
    const char* password = "YourPasswordForTest";
    int port= 2323; // Port OBD device
    IPAddress serverOBD(192,168,1,200);  //IP address OBD device
    
#else  //Enable wifi and IP for OBD in your Ioniq
    int testLine = 0; 
    const char* ssid = "YourSSIDIoniq";
    const char* password = "PasswordWifiIoniq";
    int port= 23; // Port OBD device
    IPAddress serverOBD(192,168,0,2);  //IP address OBD device
#endif



/*
// ***************************************************************************
// Very interesting to be used with IFTTT, but it has the inconveneinte that
// does not support persistence in MQTT
// ***************************************************************************
const char* mqtt_server = "io.adafruit.com";
const int mqttPort = 1883;
const char* mqttUser = "YourUsername";                              // Your Username
const char* mqttPassword = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";  // Your Active Key Key
const char* nodemqtt= "Username/f/ioniq";  //                   // Your feed
*/


// ***************************************************************************
// Config server mqttt cloudmqtt.com. Support persistence data.
// ***************************************************************************
const char* mqtt_server = "m24.cloudmqtt.com";  // Your server in CloudMQTT
const int mqttPort = 14357;                     // Your port NOT SSL
const char* mqttUser = "UserCloudMQTT";                 // Your user in CloudMQTT
const char* mqttPassword = "PasswordCloudMQTT";           // Your password in CloudMQTT
const char* nodemqtt= "ioniq/bso";              // Your topiq in CloudMQTT


// ***************************************************************************
// Config Telegram - https://core.telegram.org/bots#3-how-do-i-create-a-bot
// ***************************************************************************
String telegramLang= "ESP";  // ESP= Spanish |  ENG = English
String BOTtoken = "botxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";  //token format is botXXXXXXXXXXXXXXXXXXXXXXXXXXX
String Chat_id = "123456789"; // Chat_id
IPAddress telegramServer(149, 154, 167, 200); // IP de api.telegram.org



// ***************************************************************************
// Config DDNS
// ***************************************************************************
String ddnsService = "noip"; // Enter your DDNS Service Name - "duckdns" / "noip" / "dyndns" / "dynu" / "enom".
String ddnsDomain ="YourName.ddns.net"; 
String ddnsUsername ="UserDDNS";
String ddnsPassword ="PasswordDDNS";
int ddnsUpdate = 10000; // Check for New Ip Every 10 Seconds.


// ***************************************************************************
// Config variables
// ***************************************************************************
String vehicleID = "My IONIQ";   // VehicleID is sended in telegram message
float kWhBattery = 8.9;          // For Ioniq PHEV
//float kWhBattery = 38,3;       // For Ioniq EV
int timeZone = 1;                // Your time zone
bool daylightSaving = true;      // False for winter time, true for summer time.

char versionSoft[50] = "0.8";
String clientId = "IoniqOBD-"+ String(random(0xffff), HEX); //Client ID for server mqtt identification
String obdSerial="";
String fan;
String fanStart = "";
String timeInitial="";
String telegramMSG = "";

float bsoDecimal;
float currentDecimal;
float currentAMP1Decimal;
float currentAMP2Decimal;
float voltageDecimal;
float powerDecimal;
float tempDecimal;
float tempMinDecimal;
float tempMaxDecimal;
float cecDecimal;
float cecInitial;
float bsoInitial=0;

int Ok_2101=0;
int incr=0;
int chargerDecimal;
int endCharge;
int kms;
int delayTime=1000; // It is not convenient to saturate the OBD. I have come down to 250ms. Do you need it so fast?
int comandoOBD;
int resetCount=0;
int envioInforme=0;
int timeInitialEpoch;

char bsoMsg[50];
char cecInitialMsg[50];
char cecDecimalMsg[50];
char currentMsg[50];
char voltageMsg[50];
char mqttPayload[256];

const byte numChars = 32;
char receivedChars[numChars];   
boolean newData = false;

unsigned long timer1 = 0;
unsigned long timer2 = 0;
