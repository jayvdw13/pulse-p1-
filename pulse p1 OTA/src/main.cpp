#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.H>
#include <Arduino.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/pcnt.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "WiFiManager.H"
#include "ESPAsyncWebServer.h"
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <index.h>

float version = 1.8;

extern uint32_t chipId;
extern String chipId2;
extern int i;

extern String serverName;
extern String serverName2;

// WiFiManager
// Local intialization. Once its business is done, there is no need to keep it around
WiFiManager wifiManager;

// Create AsyncWebServer object on port 80
AsyncWebServer server2(80);
WiFiServer server(80);

// variables to count the pulses of the meters.
extern volatile unsigned long count1;
extern unsigned long prior_count1;
extern volatile unsigned long count2;
extern unsigned long prior_count2;
extern volatile unsigned long count3;
extern unsigned long prior_count3;
extern volatile unsigned long count4;
extern unsigned long prior_count4;
extern volatile unsigned long countgas;
extern unsigned long prior_countgas;
extern volatile unsigned long countwater;
extern unsigned long prior_countwater;

extern String payload;
extern String payload2;
extern String payload3;
extern String payload4;
extern String payloadgas;
extern String payloadwater;

extern unsigned long lastTime;
extern unsigned long timerDelay;

// function to increment the meters value by 1.
void pulse1(){
  count1 = count1 + 1;
}
void pulse2(){
  count2 = count2 + 1;
}
void pulse3(){
  count3 = count3 + 1;
}
void pulse4(){
  count4 = count4 + 1;
}
void pulsegas(){
  countgas = countgas + 1;
}
void pulsewater(){
  countwater = countwater + 1;
}

extern char input; // inkomende seriele data (byte).
extern bool readnextLine; // is used for the gas value of the p1 meter.
        #define BUFSIZE 1550 // the amount a line can be maximally from the p1 meter.
        char buffer[BUFSIZE]; //Buffer for the serial data to find /n
extern  int messageCount;
extern int bufpos; // the original value of the buffer position.
extern long mEVLT; //Meterstand Elektra - verbruik laag tarief
extern long mEVHT; //Meterstand Elektra - verbruik hoog tarief
extern long mETLT; //Meterstand Elektra - teruglevering laag tarief
extern long mETHT; //Meterstand Elektra - teruglevering hoog tarief
extern long mEAV;  //Meterstand Elektra - actueel verbruik
extern long mEAT;  //Meterstand Elektra - actueel teruglevering
extern float mG;   //Meterstand Gas

extern long mEVLTData; //Meterstand Elektra - verbruik laag tarief
extern long mEVHTData; //Meterstand Elektra - verbruik hoog tarief
extern long mETLTData; //Meterstand Elektra - teruglevering laag tarief
extern long mETHTData; //Meterstand Elektra - teruglevering hoog tarief
extern long mEAVData;  //Meterstand Elektra - actueel verbruik
extern long mEATData;  //Meterstand Elektra - actueel teruglevering
extern float mGData;   //Meterstand Gas

//--------------------------------------------------------OTA------------------------------------------------------------------------------
TaskHandle_t Task1;                                                                                                                     //!
void Task1code( void * pvParameters ){                                                                                                  //!
    for(;;){                                                                                                                            //!
  OTA();                                                                                                                                //!                                                                                                                      //!
  delay(500);                                                                                                                           //!
  }                                                                                                                                     //!
}                                                                                                                                       //!
void setup()                                                                                                                            //!
{                                                                                                                                       //!
  Serial.begin(115200);                                                                                                                 //!
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0                             //!
  xTaskCreatePinnedToCore(                                                                                                              //!
                    Task1code,   /* Task function. */                                                                                   //!
                    "OverTheAir",     /* name of task. */                                                                               //!
                    10000,       /* Stack size of task */                                                                               //!
                    NULL,        /* parameter of the task */                                                                            //!
                    1,           /* priority of the task */                                                                             //!
                    &Task1,      /* Task handle to keep track of created task */                                                        //!
                    0);          /* pin task to core 0 */                                                                               //!
  //---------------------------------------------------------------------------------------------------------------------------------------
  
  String ssid = "FlexGreen-Counter";
  // fetches ssid and pass from eeprom and tries to connect.
  // if it does not connect it starts an access point with the specified name,
  // here  "AutoConnectAP",
  // and goes into a blocking loop awaiting configuration.
  wifiManager.autoConnect("FlexGreen-Counter", "FlexGreen");

  // Send web page with input fields to client
  server2.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/html", index_html);
  });

  server2.on("/count1", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", payload.c_str());
  }); 

  server2.on("/count2", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", payload2.c_str());
  });

  server2.on("/count3", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", payload3.c_str());
  }); 

  server2.on("/count4", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", payload4.c_str());
  });

  server2.on("/countgas", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", payloadgas.c_str());
  }); 

  server2.on("/countwater", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", payloadwater.c_str());
  }); 

  // Start server
  server2.begin();

  pinMode(25, INPUT_PULLDOWN); // Set GPIO25 as digital output pin
  attachInterrupt(digitalPinToInterrupt(25), pulse1, RISING); 

  pinMode(19, INPUT_PULLDOWN); // Set GPIO19 as digital output pin
  attachInterrupt(digitalPinToInterrupt(19), pulse2, RISING); 

  pinMode(23, INPUT_PULLDOWN); // Set GPIO22 as digital output pin
  attachInterrupt(digitalPinToInterrupt(23), pulse3, RISING); 

  pinMode(18, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(18), pulse4, RISING); 

  pinMode(21, INPUT_PULLDOWN); // Set GPIO22 as digital output pin
  attachInterrupt(digitalPinToInterrupt(21), pulsegas, RISING); 

  pinMode(5, INPUT_PULLDOWN); // Set GPIO5 as digital output pin
  attachInterrupt(digitalPinToInterrupt(5), pulsewater, RISING); 
}



//--------------------------------------------------------------------
void loop()
{
  unsigned long new_count1 = count1;
  if (new_count1 < 1200){
  // keeps track of the amount of pulses of pulse meter one (on pin 22)
  if (new_count1 != prior_count1) {
     Serial.println(new_count1);
     prior_count1 = new_count1;
  } 
  // sends an update to the web every 1000 pulses.
  if (new_count1 > 0 && ((millis() - lastTime) > timerDelay))  {
    Serial.println(count1);
    Serial.println("Counter 1");
    count1 = 0;
  {   
      // checks if wifi is connected.
      if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
        // preparation HTTP GET request.
         String serverPath = serverName + "?chip_id=" + chipId + "&id1_name=pulse1&count1=" + new_count1;
        // Your Domain name with URL path or IP address with path.
        http.begin(serverPath.c_str());
        // Send HTTP GET request.
        int httpResponseCode = http.GET();
        // confirms the arrival of a response from the server or sends an error.
        if (httpResponseCode>0) {
          payload = http.getString();
          Serial.println(payload2);
         }
         else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
         }
        // closes the HTTP connection.
        http.end();
      }
      else {
       Serial.println("WiFi Disconnected");
        }  
      }
    }
  }

  // keeps track of the amount of pulses of pulse meter two (on pin 19)
  unsigned long new_count2 = count2;
  if (new_count2 < 1200){
  if (new_count2 != prior_count2) {
     Serial.println(new_count2);
     prior_count2 = new_count2;
  }
  // sends an update to the web every 1000 pulses.
  if (new_count2 > 0 && ((millis() - lastTime) > timerDelay))  {
    Serial.println(count2);
    Serial.println("Counter 2");
    count2 = 0;
  {   
      // checks if wifi is connected.
      if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
        // preparation HTTP GET request.
         String serverPath = serverName + "?chip_id=" + chipId + "&id2_name=pulse2&count2=" + new_count2;
        // Your Domain name with URL path or IP address with path.
        http.begin(serverPath.c_str());
        // Send HTTP GET request.
        int httpResponseCode = http.GET();
        // confirms the arrival of a response from the server or sends an error.
        if (httpResponseCode>0) {
          payload2 = http.getString();
          Serial.println(payload2);
         }
         else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
         }
        // closes the HTTP connection.
        http.end();
      }
      else {
       Serial.println("WiFi Disconnected");
        }  
      }
    }
  }
  
  // keeps track of the amount of pulses of pulse meter one (on pin 22)
  unsigned long new_count3 = count3;
  if (new_count3 < 1200){
  if (new_count3 != prior_count3) {
     Serial.println(new_count3);
     prior_count3 = new_count3;
  }
      // sends an update to the web every 1000 pulses.
  if (new_count3 > 0 && ((millis() - lastTime) > timerDelay))  {
    Serial.println(count3);
    Serial.println("Counter 3");
    count3 = 0;  
  {  
      // checks if wifi is connected.
      if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
        // preparation HTTP GET request.
         String serverPath = serverName + "?chip_id=" + chipId + "&id3_name=pulse3&count3=" + new_count3;
        // Your Domain name with URL path or IP address with path.
        http.begin(serverPath.c_str());
        // Send HTTP GET request.
        int httpResponseCode = http.GET();
        // confirms the arrival of a response from the server or sends an error.
        if (httpResponseCode>0) {
          payload3 = http.getString();
          Serial.println(payload3);
        }
        else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
        }
        // closes the HTTP connection.
        http.end();
      }
      else {
       Serial.println("WiFi Disconnected");
        } 
      }
    }
  }

  // keeps track of the amount of pulses of pulse meter one (on pin 22)
  unsigned long new_count4 = count4;
  if (new_count4 < 1200){
  if (new_count4 != prior_count4) {
     Serial.println(new_count4);
     prior_count4 = new_count4;
  }
    // sends an update to the web every 1000 pulses.
  if (new_count4 > 0 && ((millis() - lastTime) > timerDelay))  {
    Serial.println(count4);
    Serial.println("Counter 4");
    count4 = 0;  
  {  
      // checks if wifi is connected.
      if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
        // preparation HTTP GET request.
         String serverPath = serverName + "?chip_id=" + chipId + "&id4_name=pulse4&count4=" + new_count4;
        // Your Domain name with URL path or IP address with path.
        http.begin(serverPath.c_str());
        // Send HTTP GET request.
        int httpResponseCode = http.GET();
        // confirms the arrival of a response from the server or sends an error.
        if (httpResponseCode>0) {
          payload4 = http.getString();
          Serial.println(payload4);
         }
         else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
         }
        // closes the HTTP connection.
        http.end();
      }
      else {
       Serial.println("WiFi Disconnected");
        }
      }
    }
  }

  unsigned long new_countgas = countgas;
  if (new_countgas < 1200){
  if (new_countgas != prior_countgas) {
     Serial.println(new_countgas);
     prior_countgas = new_countgas;
  }
    
    // sends an update to the web every 1000 pulses.
      // sends an update to the web every 1000 pulses.
  if (new_countgas > 0 && ((millis() - lastTime) > timerDelay))  {
    Serial.println(countgas);
    Serial.println("Counter gas");
    countgas = 0;  
  {  
      // checks if wifi is connected.
      if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
        // preparation HTTP GET request.
         String serverPath = serverName + "?chip_id=" + chipId + "&id1_name=pulsegas&countgas=" + new_countgas;
        // Your Domain name with URL path or IP address with path.
        http.begin(serverPath.c_str());
        // Send HTTP GET request.
        int httpResponseCode = http.GET();
        // confirms the arrival of a response from the server or sends an error.
        if (httpResponseCode>0) {
          payloadgas = http.getString();
          Serial.println(payloadgas);
         }
         else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
         }
        // closes the HTTP connection.
        http.end();
      }
      else {
       Serial.println("WiFi Disconnected");
        } 
      }
    }
  }

  unsigned long new_countwater = countwater;
  if (new_countwater < 1200){
  if (new_countwater != prior_countwater) {
     Serial.println(new_countwater);
     prior_countwater = new_countwater;
  }
                                                                                             
    // sends an update to the web every 1000 pulses.
  if (new_countwater > 0 && ((millis() - lastTime) > timerDelay))  {
    Serial.println(countwater);
    Serial.println("Counter water");
    countwater = 0;  
  {  
      // checks if wifi is connected.
      if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
        // preparation HTTP GET request.
         String serverPath = serverName + "?chip_id=" + chipId + "&id1_name=pulsewater&countwater=" + new_countwater;
        // Your Domain name with URL path or IP address with path.
        http.begin(serverPath.c_str());
        // Send HTTP GET request.
        int httpResponseCode = http.GET();
        // confirms the arrival of a response from the server or sends an error.
        if (httpResponseCode>0) {
          payloadwater = http.getString();
          Serial.println(payloadwater);
         }
         else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
         }
        // closes the HTTP connection.
        http.end();
      }
      else {
       Serial.println("WiFi Disconnected");
        }
      }
    }
  }

  //reset timer when done sending data
  if (((millis() - lastTime) > timerDelay))  {
    lastTime = millis();
  }

  for(i=0; i<17; i=i+8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  chipId2 = String(chipId);

  long tl = 0;
  long tld =0;

  if (Serial.available() > 0) 
  {
    input = Serial.read(); 

    // --- 7 bits instelling ---
    input &= ~(1 << 7);
    // --- 7 bits instelling ---
 
    Serial.print(input);
 
    // fill the buffer untill we find a new line.
   buffer[bufpos] = input&127;
   bufpos++;
 
    if (input == '\n') 
    {
      
      if (sscanf(buffer,"1-0:1.8.1(%ld.%ld%*s" , &tl, &tld)) 
      {
        mEVLT = tl * 1000 + tld;
        if (mEVLT > 0) 
        { 
          Serial.print("Elektra - meterstand verbruik LAAG tarief (Wh): ");
          Serial.println(mEVLT);
          mEVLTData = mEVLT;
          mEVLT = 0;
        }
      }
 
      if (sscanf(buffer,"1-0:1.8.2(%ld.%ld%*s" , &tl, &tld)) 
      {
        mEVHT = tl * 1000 + tld;
        if (mEVHT > 0) 
        {
          Serial.print("Elektra - meterstand verbruik HOOG tarief (Wh): ");
          Serial.println(mEVHT);
          mEVHTData = mEVHT;
          mEVHT = 0;
        }
      }
 
      if (sscanf(buffer,"1-0:1.7.0(%ld.%ld%*s" , &tl , &tld)) 
      {
        mEAV = tl * 1000 + tld * 10;
        if (mEAV > 0) 
        {
          Serial.print("Elektra - actueel verbruik (W): ");
          Serial.println(mEAV);
          mEAVData = mEAV;
          mEAV = 0;
        }
      }
 
      if (sscanf(buffer,"1-0:2.8.1(%ld.%ld%*s" , &tl, &tld)) 
      {
        mETLT = tl * 1000 + tld;
        if (mETLT > 0) 
        {
          Serial.print("Elektra - meterstand teruglevering LAAG tarief (Wh): ");
          Serial.println(mETLT);
          mETLTData = mETLT;
           mETLT = 0;
        }
      }
 
      if (sscanf(buffer,"1-0:2.8.2(%ld.%ld%*s" , &tl, &tld)) 
      {
        mETHT = tl * 1000 + tld;
        if (mETHT > 0) 
        { 
          Serial.print("Elektra - meterstand teruglevering HOOG tarief (Wh): ");
          Serial.println(mETHT);
          mETHTData = mETHT;
          mETHT = 0;
        }
      }

      if(sscanf(buffer,"1-0:2.7.0(%ld.%ld%*s" , &tl , &tld)) 
      {
        mEAT = tl * 1000 + tld * 10;
        if (mEAT > 0) 
        {
          Serial.print("Elektra - actueel teruglevering (W): ");
          Serial.println(mEAT);
          mEATData = mEAT;
          mEAT = 0;
        }
      }
 
      if(sscanf(buffer,"0-1:24.3.0(%6ld%4ld%*s" , &tl, &tld)) 
        readnextLine = true; // we moeten de volgende lijn hebben
      if (readnextLine)
      {
        if(sscanf(buffer,"(%ld.%ld%*s" , &tl, &tld)) 
        {
          mG = float ( tl * 1000 + tld ) / 1000;
          Serial.print("Gas - meterstand (m3): ");
          Serial.println(mG);
          Serial.println("");
          readnextLine = false;
          mGData = mG;
          mG = 0;
        }
      }

    // Maak de buffer weer leeg (hele array)
    for (int i=0; i<BUFSIZE; i++)
      buffer[i] = 0;
    bufpos = 0;
    }
      // look for an "!" which always indicates the end of a telegram from the p1 meter.
      if(input == '!')
    {
      messageCount++;
      if(messageCount == 6){
      // looks if the wifi is connected.
      if(WiFi.status()== WL_CONNECTED){
            HTTPClient http;
            // serial print to confirm the connection.
          Serial.println("P1 Poort");
          // preparation HTTP GET request.
        String serverPath = serverName2 + "?chip_id=" + chipId2 + " &mEVLT=" + mEVLTData + " &mEVHT=" + mEVHTData + "&mETLT=" + mETLTData + "&mETHT=" + mETHTData + "&mEAV=" + mEAVData + "&mEAT=" + mEATData + "&mG=" + mGData;
         // Your Domain name with URL path or IP address with path.
       http.begin(serverPath.c_str());

       // Send HTTP GET request.
       int httpResponseCode = http.GET();

       // confirms the arrival of a response from the server or sends an error.
       if (httpResponseCode>0) {
         Serial.print("HTTP Response code: ");
         Serial.println(httpResponseCode);
         String payloadP1 = http.getString();
         Serial.println(payloadP1);
       }
       else {
         Serial.print("Error code: ");
         Serial.println(httpResponseCode);
       }
       // close the http connection.
       http.end();
     }
     else {
       Serial.println("WiFi Disconnected");
     } 
     messageCount = 0;
      }
    }
  }
}