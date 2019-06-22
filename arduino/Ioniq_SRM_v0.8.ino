/*
  Ioniq SRM (SOC Remote Monitor)
  Copyright (c) 2019 for WE Koyote Ioniq.wekoyote at gmail.com. All rights reserved.
*/

#include <time.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <EasyDDNS.h>         // https://github.com/ayushsharma82/EasyDDNS
#include <ESP8266WiFi.h>  
#include <PubSubClient.h>     // IMPORTANT: Modify #define MQTT_MAX_PACKET_SIZE 256 in PubSubClient.h file from library directory
#include <stdlib.h>
#include "Config.h"

WiFiClient client;
WiFiClient mqttConnect;
PubSubClient mqttclient(mqttConnect);
WiFiClientSecure secureClient; 
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", timeZone * 3600, 60000); //Configure yput timeZone in Config.h


// ***************************************************************************
// MAIN
// ***************************************************************************
//

void setup()  {
  
    // ***************************************************************************
    // Setup: date & Time
    // ***************************************************************************
  if (daylightSaving){
      timeClient.setTimeOffset(timeZone * 3600 + 3600);
      timeClient.begin();
  }
  

    // ***************************************************************************
    // Setup: OLED Screen Wifi Kit_8
    // ***************************************************************************
    #ifdef ENABLE_HELTEC_WIFI_Kit_8
        pinMode(RST_OLED, OUTPUT);
        digitalWrite(RST_OLED, LOW);   // turn D2 low to reset OLED
        delay(50);
        digitalWrite(RST_OLED, HIGH);   // while OLED is running, must set D2 in high
        display.begin();
     
        // ***************************************************************************
        // Intro screen
        // ***************************************************************************
        display.print("Ioniq SOC Remote",0);
        display.print("     v ",1);
        display.print(versionSoft,1,8);
        display.print("  by WE Koyote  ",3);
        delay(4000);
        //display.clear(); 
    #endif
    
    // ***************************************************************************
    // Start WiFi
    // ***************************************************************************
    Serial.begin(115200);
    WiFi.begin(ssid, password);  
    while (WiFi.status() != WL_CONNECTED)  {
      Serial.print(".");
      delay(500);
    }
    
    Serial.println("WiFi Connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    timeClient.begin(); 

    #ifdef ENABLE_HELTEC_WIFI_Kit_8
        display.print("Conecting....   ",3);                       
    #endif    

    delay(500);
    Serial.println("Conecting OBD...");
    client.connect(serverOBD, port);
    Serial.println("OBD Conectado");
    
    #ifdef ENABLE_HELTEC_WIFI_Kit_8
        display.print("  OBD Connected  ",3);
    #endif        
    
    delay(500);
    initOBD();
    
    #ifdef ENABLE_HELTEC_WIFI_Kit_8
        display.print("                ",1);
    #endif

  Serial.println("Synchronizing date and time..");                 
  timeClient.update();  
  Serial.print("Date & Time:");
  Serial.println(getTimeStampString());

  #ifdef ENABLE_DDNS
    EasyDDNS.service(ddnsService);    
    EasyDDNS.client(ddnsDomain,ddnsUsername,ddnsPassword);  
 #endif
}


void loop() { 
   #ifdef ENABLE_DDNS
    EasyDDNS.update(ddnsUpdate); 
   #endif
   Ok_2101=0;
  
  //Update date/time every 5 minutes
  timer2 = millis();
  if(timer2 > (timer1+300000)){  
    timer1 = millis(); 
    
    Serial.println("Synchronizing date and time..");
    timeClient.update();  
    Serial.print("Date & Time:");
    Serial.println(getTimeStampString());
  }
  
   
  // Connecting to OBD
  Serial.println("");
  Serial.println("-------------------------------------");
  Serial.println("");
  
  Serial.println("Conecting OBD...");
  client.connect(serverOBD, port);
  Serial.println("OBD Connected");
  #ifdef ENABLE_HELTEC_WIFI_Kit_8
      display.print("Request 2101    ",2);
  #endif
  Serial.println("======================");
  Serial.println("Sending 2101");
  client.print("2101\r\n");
  delay(2000);    
  recvEndMarker(2101);
  obdSerial="";
  
  
  #ifdef ENABLE_HELTEC_WIFI_Kit_8
      display.print("Request 2105    ",2);
  #endif
  Serial.println("======================");
  Serial.println("Request 2105");
  client.print("2105\r\n");
  delay(2000);
  recvEndMarker(2105);
  obdSerial="";

 
  // *******************************************************************************
  // The next 'if' I use to discard erroneous lines. If the content is useless 
  // I do not enter, but if it is correct or it is in test mode, I continue forward
  // ******************************************************************************* 
  if(bsoDecimal>10.00 && Ok_2101==1){
      resetCount=0; 

      // *******************************************************************************
      // Start the construction of the MQTT message
      // ******************************************************************************* 

      dtostrf(bsoDecimal, 2, 1, bsoMsg);
      dtostrf(cecDecimal, 2, 1, cecDecimalMsg);
      dtostrf(cecInitial, 2, 1, cecInitialMsg);
      if(fan == ""){fan="0";} // Avoid mqtt message malformation if the data fails
       
      String payload = "{";
              payload += "\"cec\":"; payload += (cecDecimal-cecInitial); payload += ",";
              payload += "\"bso\":"; payload += bsoDecimal; payload += ",";
              payload += "\"i\":"; payload += currentDecimal; payload += ","; 
              payload += "\"v\":"; payload += voltageDecimal; payload += ",";
              payload += "\"p\":"; payload += powerDecimal; payload += ",";
              payload += "\"t\":"; payload += tempDecimal; payload += ",";
              payload += "\"tu\":"; payload += tempMaxDecimal; payload += ",";
              payload += "\"tl\":"; payload += tempMinDecimal; payload += ",";
              payload += "\"ch\":"; payload += chargerDecimal; payload += ",";
              payload += "\"end\":"; payload += endCharge; payload += ",";
              payload += "\"f\":"; payload += fan; payload += ",";
              payload += "\"k\":"; payload += kms;
              payload += "}";
      
      payload.toCharArray( mqttPayload, 256 );
                
      Serial.print("MQTT Msg");
      Serial.println(payload);
      Serial.println("Conectando  MQTT...");
      mqttclient.setServer(mqtt_server, mqttPort);
                
      if (mqttclient.connect(clientId.c_str(),mqttUser, mqttPassword)) { // Attempt to connect to MQTT server
        #ifdef ENABLE_HELTEC_WIFI_Kit_8
            display.print(cecInitialMsg,1,0);
            display.print(cecDecimalMsg,1,8);
        #endif
        Serial.println("Conectado MQTT");
      } else {
        Serial.print("fallo, rc=");
        Serial.print(mqttclient.state());
        Serial.println(" reintentando en 5 segundos");
        delay(5000); // Wait 5 seconds before retrying
      }
        
      if (mqttclient.connect(clientId.c_str())) {
       #ifdef ENABLE_HELTEC_WIFI_Kit_8
           display.print("                ",3);
           display.print("  Bateria: ",3);
           display.print(bsoMsg,3,11);
       #endif

        if (Ok_2101==1){ // Only publish in mqtt server if data is true
          mqttclient.publish(nodemqtt,mqttPayload,true);
        }
      }      
      Serial.println("Desconectando MQTT");
      delay(1000);
      client.stop();            
    
    }else{        
      resetCount=resetCount+1; // if the serial data is wrong...
      Serial.print("Reset control=");
      Serial.println(resetCount);
    }
    
  if(resetCount>=10){ // If data is corrupted or incorrect, restart ESP. Pending assigning a PIN to reset.
    //ESP.reset();  // Not run in Wifi Kit 8 form Heltec
    //ESP.restart() // Not run in Wifi Kit 8 form Heltec
    resetCount=0;
    initOBD();
  } 
    
}


// ***************************************************************************
// Differents functions 
// *************************************************************************** 


// Parsing serial and parsing OBD responses
void recvEndMarker(int comandoOBD) {
  char endMarker = '>';
  char recvSerial;

  while (client.available() > 0 && newData == false) {
    recvSerial = client.read();    
      if (recvSerial != endMarker) {
        Serial.print(recvSerial);
        obdSerial=obdSerial+recvSerial;
     }
   }

   if(obdSerial.substring (0,9)=="CAN ERROR"){ //If ECU is OFF
      Serial.println ("CANERROR: UNABLE TO CONNECT TO ECU"); 
      obdSerial="";
     }

    if(obdSerial.substring (0,6)=="UNABLE"){ //If OBD device not respond
        obdSerial="";
        Serial.println ("UNABLE TO CONNECT TO ECU"); 
        Serial.println ("Sleep"); 
        //display.clear(); 
        delay(15000);
        Serial.println("I'm awake");
     }
      
     if(obdSerial.substring (0,9)=="SEARCHING"){  //if the serial does not respond as expected
         obdSerial="";
         Serial.println ("UNABLE TO CONNECT TO ECU: \'SEARCHING\'"); 
     }

      // ***************************************************************************
      // Parsing 2101 command OBD (Volts, intensity, temperatures, etc)
      // ***************************************************************************
      if((comandoOBD==2101 & obdSerial !="" )|| testLine==1){
      
        if (testLine==1){ //For testing without OBD connect       
          testString();                 
        }
         
         String currentAMP1 = (obdSerial.substring (obdSerial.indexOf("1:")+21,obdSerial.indexOf("1:")+23));
         String currentAMP2 = (obdSerial.substring (obdSerial.indexOf("2:")+3,obdSerial.indexOf("2:")+5)); 
         String volt1 = (obdSerial.substring (obdSerial.indexOf("2:")+6,obdSerial.indexOf("2:")+8));
         String volt2 = (obdSerial.substring (obdSerial.indexOf("2:")+9,obdSerial.indexOf("2:")+11));     
         String charger = (obdSerial.substring (obdSerial.indexOf("1:")+18,obdSerial.indexOf("1:")+19));
         String tempMax = (obdSerial.substring (obdSerial.indexOf("2:")+12,obdSerial.indexOf("2:")+14));
         String tempMin = (obdSerial.substring (obdSerial.indexOf("2:")+15,obdSerial.indexOf("2:")+17));
         String tempM1 = (obdSerial.substring (obdSerial.indexOf("2:")+18,obdSerial.indexOf("2:")+20));
         String tempM2 = (obdSerial.substring (obdSerial.indexOf("2:")+21,obdSerial.indexOf("2:")+23));
         fan = (obdSerial.substring (obdSerial.indexOf("4:")+10,obdSerial.indexOf("4:")+11));

         //CEC
         String cec = (obdSerial.substring (obdSerial.indexOf("5:")+21,obdSerial.indexOf("5:")+23));
                cec += (obdSerial.substring (obdSerial.indexOf("6:")+3,obdSerial.indexOf("6:")+5));
                cec += (obdSerial.substring (obdSerial.indexOf("6:")+6,obdSerial.indexOf("6:")+8));
                cec += (obdSerial.substring (obdSerial.indexOf("6:")+9,obdSerial.indexOf("6:")+11));                

         currentAMP1Decimal=float(hexToDec(currentAMP1));
         currentAMP2Decimal=float(hexToDec(currentAMP2));
         voltageDecimal= (float((hexToDec(volt1))*256)+(float(hexToDec(volt2))))/10;
         tempDecimal= (float(float(hexToDec(tempM1))+float(hexToDec(tempM2)))/2);
         tempMaxDecimal = float(hexToDec(tempMax));
         tempMinDecimal = float(hexToDec(tempMin));
         cecDecimal=float(hexToDec(cec))/10;

         if(volt1=="" || volt2=="" || currentAMP1=="" || currentAMP2=="" || charger=="" || tempM1=="" || tempM2=="" || tempMin=="" || tempMax=="" || fan==""){
            Ok_2101=0;
         }else{
           Ok_2101=1;
         }

                    
      if(Ok_2101==1){  // consume or charge battery? Surely there are better ways to do it. Sorry.
             if(currentAMP1Decimal>127){ 
                 currentDecimal=(((currentAMP1Decimal-256)*256)+currentAMP2Decimal)/10;
             }else{
                 currentDecimal = ((currentAMP1Decimal*256)+currentAMP2Decimal)/10;
             }
    
            chargerDecimal = float(hexToDec(charger));
            if(chargerDecimal>=7){
              chargerDecimal=1;
            }else{
              chargerDecimal=0;
            }        


            if(chargerDecimal==1 & powerDecimal<0){    //Estimation of charge time
                endCharge=((((100-bsoDecimal)*kWhBattery)/100)/((powerDecimal*-1)/60));                
                envioInforme=1;
                
                if(timeInitial==""){
                  timeClient.update();
                  //Serial.println(getTimeStampString());    
                  timeInitial=(getTimeStampString());  
                  timeInitialEpoch=(timeClient.getEpochTime());
                }   
 
               if(fan == "1" && telegramLang == "ESP"){
                    fanStart= "Arrancó";
                } 
               if(fan == "1" && telegramLang == "ENG"){
                    fanStart= "If it started";
                } 
                 
             }else{
              endCharge=0;
            
            #ifdef ENABLE_TELEGRAM
              if(envioInforme==1 && (timeClient.getEpochTime()-timeInitialEpoch)>30 && bsoInitial>10 && bsoInitial<bsoDecimal && (cecDecimal-cecInitial)>0){  //To avoid "bounces" is not sent if the load has been greater than 60 seconds           
                  timeClient.update();
                  Serial.println("Send Telegram");

                if(telegramLang == "ESP"){      
                          telegramMSG = " ";
                          telegramMSG +="<b>INFORME DE CARGA DE</b>%0A";
                          telegramMSG +="<b>" ; telegramMSG +=vehicleID; telegramMSG +=  "</b>%0A";
                          telegramMSG +="<b>=====================</b>%0A%0A";
                          telegramMSG +="<b>Hora inicio:</b> " ; telegramMSG +=timeInitial; telegramMSG +=  " %0A";
                          telegramMSG +="<b>Hora final:</b> " ; telegramMSG +=(getTimeStampString()); telegramMSG +=  " %0A";
                          telegramMSG +="<b>Tiempo:</b> " ; telegramMSG += (getTimeDifference(timeClient.getEpochTime()-timeInitialEpoch)); telegramMSG +=  " h %0A";
                          telegramMSG +="-------------------------------------------- %0A";
                          telegramMSG +="<b>CEC Inicio:</b> " ; telegramMSG += cecInitial; telegramMSG +=  " kWh %0A";
                          telegramMSG +="<b>CEC final:</b> " ; telegramMSG += cecDecimal; telegramMSG +=  " kWh %0A";
                          telegramMSG +="<b>Energía cargada:</b> " ; telegramMSG += (cecDecimal-cecInitial); telegramMSG +=  " kWh %0A";
                          telegramMSG +="-------------------------------------------- %0A";
                          telegramMSG +="<b>Carga inicial:</b> " ; telegramMSG += bsoInitial; telegramMSG +=  "  % %0A";
                          telegramMSG +="<b>Carga final:</b>  " ; telegramMSG += bsoDecimal; telegramMSG +=  "  % %0A";
                          telegramMSG +="<b>Carga realizada:</b> " ; telegramMSG += (bsoDecimal-bsoInitial); telegramMSG +=  " % %0A";
                          telegramMSG +="-------------------------------------------- %0A";
                          telegramMSG +="<b>Temperatura máxima:</b>  " ; telegramMSG += tempMaxDecimal; telegramMSG +=  "º %0A";
                          telegramMSG +="<b>Temperatura mínima:</b>  " ; telegramMSG += tempMinDecimal; telegramMSG +=  "º %0A";
                          telegramMSG +="<b>Temperatura media:</b>  " ; telegramMSG += ((tempMaxDecimal+tempMinDecimal)/2); telegramMSG +=  "º %0A";
                          telegramMSG +="<b>Ventilador:</b> " ; telegramMSG +=fanStart; telegramMSG +=  " %0A%0A";      
                          telegramMSG +="Ioniq SOC Remonte Monitor %0Aby <b>WE Koyote</b> %0A%0A";      
                }else{
                          telegramMSG = " ";
                          telegramMSG +="<b>CHARGE REPORT</b>%0A";
                          telegramMSG +="<b>" ; telegramMSG +=vehicleID; telegramMSG +=  "</b>%0A";
                          telegramMSG +="<b>=====================</b>%0A%0A";
                          telegramMSG +="<b>Start Time:</b> " ; telegramMSG +=timeInitial; telegramMSG +=  " %0A";
                          telegramMSG +="<b>Final time:</b> " ; telegramMSG +=(getTimeStampString()); telegramMSG +=  " %0A";
                          telegramMSG +="<b>Time:</b> " ; telegramMSG += (getTimeDifference(timeClient.getEpochTime()-timeInitialEpoch)); telegramMSG +=  " h %0A";
                          telegramMSG +="-------------------------------------------- %0A";
                          telegramMSG +="<b>CEC Start:</b> " ; telegramMSG += cecInitial; telegramMSG +=  " kWh %0A";
                          telegramMSG +="<b>CEC final:</b> " ; telegramMSG += cecDecimal; telegramMSG +=  " kWh %0A";
                          telegramMSG +="<b>Charged energy:</b> " ; telegramMSG += (cecDecimal-cecInitial); telegramMSG +=  " kWh %0A";
                          telegramMSG +="-------------------------------------------- %0A";
                          telegramMSG +="<b>BSO Start:</b> " ; telegramMSG += bsoInitial; telegramMSG +=  "  % %0A";
                          telegramMSG +="<b>BSO final:</b>  " ; telegramMSG += bsoDecimal; telegramMSG +=  "  % %0A";
                          telegramMSG +="<b>Charged:</b> " ; telegramMSG += (bsoDecimal-bsoInitial); telegramMSG +=  " % %0A";
                          telegramMSG +="-------------------------------------------- %0A";
                          telegramMSG +="<b>Maximum temperature:</b>  " ; telegramMSG += tempMaxDecimal; telegramMSG +=  "º %0A";
                          telegramMSG +="<b>Minimum temperature:</b>  " ; telegramMSG += tempMinDecimal; telegramMSG +=  "º %0A";
                          telegramMSG +="<b>Medium temperature:</b>  " ; telegramMSG += ((tempMaxDecimal+tempMinDecimal)/2); telegramMSG +=  "º %0A";
                          telegramMSG +="<b>Fan battery:</b> " ; telegramMSG +=fanStart; telegramMSG +=  " %0A%0A";      
                          telegramMSG +="Ioniq SOC Remonte Monitor %0Aby <b>WE Koyote</b> %0A%0A";      
                }
     
                   
                   telegramSend(telegramMSG);              
               }
              #endif
                
               cecInitial = cecDecimal; 
               envioInforme=0;
               bsoInitial =  bsoDecimal;  
               timeInitial="";
               timeInitialEpoch=0;
               
               if(telegramLang == "ESP"){
                  fanStart = "No arrancó";
                }else{
                  fanStart = "It did not start";
               }
            }
      }
                 
        // ***************************************************************************
        // For debug in console
        // ***************************************************************************     
        
         Serial.println();
         Serial.println ("2101:");
         Serial.print("Time charge: ");
         Serial.println(endCharge); 

         Serial.print ("CURRENT HEX: ");
         Serial.println (currentAMP1+"  "+currentAMP2);
         Serial.print ("CURRENT AMP: ");
         Serial.print (currentAMP1Decimal,2);
         Serial.print (" ");
         Serial.println (currentAMP2Decimal,2);
         Serial.print ("CURRENT DEC: ");
         Serial.println (currentDecimal,2);
         Serial.print ("VOLTAGE HEX: ");
         Serial.println (volt1+" "+volt2);
         Serial.print ("VOLTAGE DEC: ");
         Serial.println (voltageDecimal,2);  
         Serial.print ("CHARGER HEX: ");
         Serial.println (charger); 
         Serial.print ("CHARGER DEC: ");
         Serial.println (chargerDecimal);
         Serial.print ("CEC HEX: ");
         Serial.println (cec);
         Serial.print ("CEC DEC: ");
         Serial.println (cecDecimal);
         Serial.print ("CEC Initial: ");
         Serial.println (cecInitial);         

         Serial.print ("TEMP M1 HEX: ");
         Serial.println (tempM1); 
         Serial.print ("TEMP M2 HEX: ");
         Serial.println (tempM2); 
         Serial.print ("TEMP DEC: ");
         Serial.println (tempDecimal);         
         Serial.print ("TEMP Min HEX: ");
         Serial.println (tempMin); 
         Serial.print ("TEMP Min DEC: ");
         Serial.println (tempMinDecimal);
         Serial.print ("TEMP Max HEX: ");
         Serial.println (tempMax); 
         Serial.print ("TEMP Max DEC: ");
         Serial.println (tempMaxDecimal);
         Serial.print ("FAN: ");
         Serial.println (fan);
    } 

      // ***************************************************************************
      // Parsing 2105 command OBD (BSO Display)
      // ***************************************************************************   
       if(comandoOBD==2105 && obdSerial !="" && Ok_2101==1){
   
        if (testLine==1){ //For testing without OBD connect
          obdSerial="5: 00 06 02 03 E8 51 " ; obdSerial +=(incr+25);        
        }
        
        String bso= (obdSerial.substring (obdSerial.indexOf("5:")+21,obdSerial.indexOf("5:")+23));
        bsoDecimal = (float(hexToDec(bso))/float(2));

        
        if(bsoDecimal==99.50){ //Small trap due to the shutdown of the ECU. Upon arrival the charge is 100%, the ECU goes off. Sorry. Ideas are accepted ;-)
          bsoDecimal=100;
          currentDecimal=0;
          currentDecimal=0;
          voltageDecimal=397.80;
          chargerDecimal=0;
        }


        if(bsoDecimal>=20){         //Rule of three calculator for mileage estimate in electrical
          kms=(bsoDecimal-20)*0.7875;  
        }else{
          kms=0;
        }

        powerDecimal=(voltageDecimal*currentDecimal)/1000; //Calculate power in kW


        // ***************************************************************************
        // For debug in console
        // ***************************************************************************  
        Serial.println ();
        Serial.println ("2105:");
        Serial.print ("BSO HEX: ");
        Serial.println (bso);
        Serial.print ("BSO DEC: ");
        Serial.println (bsoDecimal);
        Serial.println ("");
      }    
}



// ***************************************************************************
//
//                    The different functions starts here
//
// ***************************************************************************


// ***************************************************************************
// Setup: Strings for initilize OBD
// ***************************************************************************
void initOBD(){
    
    #ifdef ENABLE_HELTEC_WIFI_Kit_8
      display.print(" Inicializa OBD ",2);
    #endif 
    
    Serial.println("INIT OBD"); 
    Serial.println("ATZ"); 
    client.print("ATZ\r\n"); // Reset OBD
    delay(500);
    serialPrint();   
       
    Serial.println("ATE0"); 
    client.print("ATE0\r\n"); //ECHO OFF
    delay(500);
    serialPrint();
  
    Serial.println("ATL1"); 
    client.print("ATL1\r\n"); // LF OFF
    delay(500);
    serialPrint();
    
    Serial.println("ATH0"); // Headers OFF
    client.print("ATH0\r\n"); 
    delay(500);
    serialPrint();

    Serial.println("ATST62"); 
    client.print("ATST62\r\n"); //Timeout to 62 x 4 msecs
    delay(500);
    serialPrint();

    Serial.println("ATSH 7E4"); 
    client.print("ATSH 7E4\r\n"); //Filter for 7E4
    delay(500);
    serialPrint();
    
    Serial.println(); 
}


// ***************************************************************************
// Setup: Strings for initilize OBD
// ***************************************************************************
void serialPrint(){
    //For clean serial buffer
  char endMarker = '>';
  char recvSerial;
  while (client.available() > 0 && newData == false) {
    recvSerial = client.read();    
      if (recvSerial != endMarker) {
        Serial.print(recvSerial);
     }
   }
   Serial.println();
}



// ***************************************************************************
// Function for convert string to long
// ***************************************************************************
//
long stringToLong(String s){
   char arr[12];
   s.toCharArray(arr, sizeof(arr));
   return atol(arr);
}



// ***************************************************************************
// Function for convert HEX to DEC
// ***************************************************************************
//
unsigned long hexToDec(String hexString) {
  unsigned long decValue = 0;
  char nextInt;
  for ( long i = 0; i < hexString.length(); i++ ) {
    nextInt = toupper(hexString[i]);
    if( isxdigit(nextInt) ) {
        if (nextInt >= '0' && nextInt <= '9') nextInt = nextInt - '0';
        if (nextInt >= 'A' && nextInt <= 'F') nextInt = nextInt - 'A' + 10;
        decValue = (decValue << 4) + nextInt;
    }
  }
  return decValue;
}



// ***************************************************************************
// Function for convert string to HEX
// ***************************************************************************
//
long StrToHex(char str[]) {
    return (long) strtol(str, 0, 16);
}


// ***************************************************************************
// Function for send report via telegram
// ***************************************************************************
//
void telegramSend( String telegramText ) {
  if (secureClient.connect(telegramServer, 443)) {
    secureClient.println("GET /" + BOTtoken + "/sendMessage?parse_mode=HTML&chat_id=" + Chat_id + "&text=" + telegramText + "\n\r");
    Serial.println("GET /" + BOTtoken + "/sendMessage?parse_mode=HTML&chat_id=" + Chat_id + "&text=" + telegramText + "\n\r");
  }else{
     Serial.println("Send Telegram FAILED!!");
  }
}


// ***************************************************************************
// Return formated date/time
// ***************************************************************************
// https://github.com/arduino-libraries/NTPClient/issues/36
//
String getTimeStampString() {
   //Serial.println(timeClient.getEpochTime());
   time_t rawtime = timeClient.getEpochTime();
   struct tm * ti;
   ti = localtime (&rawtime);

   uint16_t year = ti->tm_year + 1900;
   String yearStr = String(year);

   uint8_t month = ti->tm_mon + 1;
   String monthStr = month < 10 ? "0" + String(month) : String(month);

   uint8_t day = ti->tm_mday;
   String dayStr = day < 10 ? "0" + String(day) : String(day);

   uint8_t hours = ti->tm_hour;
   String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

   uint8_t minutes = ti->tm_min;
   String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

   uint8_t seconds = ti->tm_sec;
   String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

  return dayStr + "-" + monthStr + "-" + yearStr + " " +
          hoursStr + ":" + minuteStr;
          
}


// ***************************************************************************
// Return difference between two date/time
// ***************************************************************************
// https://github.com/arduino-libraries/NTPClient/issues/36
//
String getTimeDifference(int difference) {

   time_t rawtime = difference;
   struct tm * ti;
   ti = localtime (&rawtime);

   uint16_t year = ti->tm_year + 1900;
   String yearStr = String(year);

   uint8_t month = ti->tm_mon + 1;
   String monthStr = month < 10 ? "0" + String(month) : String(month);

   uint8_t day = ti->tm_mday;
   String dayStr = day < 10 ? "0" + String(day) : String(day);

   uint8_t hours = ti->tm_hour;
   String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

   uint8_t minutes = ti->tm_min;
   String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

   uint8_t seconds = ti->tm_sec;
   String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

  return hoursStr + ":" + minuteStr + ":" + secondStr;
          
}


// ***************************************************************************
// If testLine is enabled, send simulatd data for test.
// Go ahead and try other values XD
// ***************************************************************************
// 
void testString(){
  //obdSerial= "1: 19 12 5C 16 F8 70 01\r\n2: 3F 0F 39 15 16 17 18\r\n4: B7 29 01 FF 90 00 00\n\r5: 89 BE 00 00 89 3A 00\n\r6: 00 36 A3 00 00 2F E0 "; // General
  
  delay(2000);
  if(incr>=50){
    //incr=0;
  }
  
  incr=incr+1;
  Serial.print("INCREMENTAL: ");
  Serial.println(incr);
  
  if(incr>0 && incr<10){ 
  obdSerial= "1: 19 12 5C 16 F8 70 01\r\n2: 3F 0F 39 15 16 17 18\r\n4: B7 29 00 FF 90 00 00\n\r5: 89 BE 00 00 89 3A 00\n\r6: 00 36 A3 00 00 2F E0 "; // Con cargador sin cargar 1398,7
  }
  
  if(incr>=10 && incr<15){ 
  obdSerial= "1: 19 12 5C 16 F8 70 FE\r\n2: 3F 0F 39 15 16 17 18\r\n4: B7 29 00 FF 90 00 00\n\r5: 89 BE 00 00 89 3A 00\n\r6: 00 36 A3 00 00 2F E0 "; // Con cargador y cargando  Lectura: 13987.00
  }
  
  if(incr>=15 && incr<20){ 
  obdSerial= "1: 19 12 5C 16 F8 70 FE\r\n2: 3F 0F 39 15 16 17 18\r\n4: B7 29 01 FF 90 00 00\n\r5: 89 BE 00 00 89 3A 00\n\r6: 00 36 AF 00 00 2F E0 "; // Con cargador, ventilador y cargando 1399,9
  }
  
  if(incr>=20 && incr<25){ 
  obdSerial= "1: 19 12 5C 16 F8 70 00\r\n2: 3F 0F 39 15 16 17 18\r\n4: B7 29 00 FF 90 00 00\n\r5: 89 BE 00 00 89 3A 00\n\r6: 00 36 B2 00 00 2F E0 "; // Con cargador y SIN CARGAR 1400,2
  }           
  
  // Repeat other test
  
  if(incr>=25 && incr<30){ 
  obdSerial= "1: 19 12 5C 16 F8 00 01\r\n2: 3F 0F 39 15 16 17 18\r\n4: B7 29 00 FF 90 00 00\n\r5: 89 BE 00 00 89 3A 00\n\r6: 00 36 C5 00 00 2F E0 "; // SIN CARGADOR 1402,1
  }             
  
  if(incr>=30 & incr<35){ 
  obdSerial= "1: 19 12 5C 16 F8 70 01\r\n2: 3F 0F 39 15 16 17 18\r\n4: B7 29 00 FF 90 00 00\n\r5: 89 BE 00 00 89 3A 00\n\r6: 00 36 C5 00 00 2F E0 "; // Con cargador sin cargar 1402,1
  }
  
  if(incr>=35 && incr<40){ 
  obdSerial= "1: 19 12 5C 16 F8 70 FE\r\n2: 3F 0F 39 15 16 17 18\r\n4: B7 29 00 FF 90 00 00\n\r5: 89 BE 00 00 89 3A 00\n\r6: 00 36 C5 00 00 2F E0 "; // Con cargador y cargando  Lectura: 1402,1
  }
            
  if(incr>=40 && incr<45){ 
  obdSerial= "1: 19 12 5C 16 F8 70 FE\r\n2: 3F 0F 39 15 16 17 18\r\n4: B7 29 01 FF 90 00 00\n\r5: 89 BE 00 00 89 3A 00\n\r6: 00 36 CF 00 00 2F E0 "; // Con cargador, ventilador y cargando 1403,1
  }
  
  if(incr>=45 && incr<50){
  obdSerial= "1: 19 12 5C 16 F8 00 00\r\n2: 3F 0F 39 15 16 17 18\r\n4: B7 29 00 FF 90 00 00\n\r5: 89 BE 00 00 89 3A 00\n\r6: 00 36 CF 00 00 2F E0 "; // SIN cargador 1403,1
  }  
}
  
