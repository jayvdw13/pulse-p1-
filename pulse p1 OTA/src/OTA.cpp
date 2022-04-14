#include <Arduino.h>
#include <Update.h>
#include <HTTPClient.H>
#include <SPIFFS.h>

extern float version;

extern uint32_t chipId;
extern String chipId2;
extern int i;

//Your Domain name with URL path or IP address with path
extern String serverName;
extern String serverName2;
extern String update;
extern String get_update_settings;

extern unsigned long lastTimeOTA;  
extern unsigned long timerDelayOTA;

extern volatile unsigned long count1;
extern volatile unsigned long count2;
extern volatile unsigned long count3;
extern volatile unsigned long count4;
extern volatile unsigned long countgas;
extern volatile unsigned long countwater;

extern String payload;
extern String payload2;
extern String payload3;
extern String payload4;
extern String payloadgas;
extern String payloadwater;
extern String update_settings;
extern String payload6;

extern unsigned long lastTime1;  
extern unsigned long lastTime2; 
extern unsigned long lastTime3; 
extern unsigned long lastTime4; 
extern unsigned long lastTime5; 
extern unsigned long lastTime6; 
extern unsigned long timerDelay;

String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  file.close();
  Serial.println(fileContent);
  return fileContent;
}

// ----------------------------------------------------------------
// Function to write files
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}


void get_auto_update(){
  HTTPClient http;

  String serverPath6 = get_update_settings + "?chip_id=" + chipId;

  // ----------------------------------------------------------------
  // Start generated link above
  http.begin(serverPath6.c_str());
      
  // ----------------------------------------------------------------
  // Send HTTP GET request
  int httpResponseCode4 = http.GET();
  if (httpResponseCode4>0) {
    payload6 = http.getString();
    Serial.println(payload6);
  }
  http.end();

  if (payload6 == "True" || payload6 == "False"){
    writeFile(SPIFFS, "/auto_update_settings.txt", payload6.c_str());
    update_settings = payload6;
  }
  else {
    String update_settings_temp = readFile(SPIFFS, "/auto_update_settings.txt");
    if (update_settings_temp == "True" || update_settings_temp == "False"){
      update_settings = update_settings_temp;
    }
    else{
      update_settings = "False";
    }
  }
}

int currentLength;
int totalLength;
void updateFirmware(uint8_t *data, size_t len){
  Update.write(data, len);
  currentLength += len;

  // ----------------------------------------------------------------
  // Print dots while waiting for update to finish
  Serial.print('.');
  
  // ----------------------------------------------------------------
  // if current length of written firmware is not equal to total firmware size, repeat

  if(currentLength != totalLength) return;
  Update.end(true);
  Serial.printf("\nUpdate Success, Total Size: %u\nRebooting...\n", currentLength);
  
  // ----------------------------------------------------------------
  // Restart ESP32 to see changes 
  ESP.restart();
}


void OTA(){
  // ----------------------------------------------------------------
  // If statement for second timer
  if ((millis() - lastTimeOTA) > timerDelayOTA) {

     // ----------------------------------------------------------------
     // Check WiFi connection status
      if(WiFi.status()== WL_CONNECTED){

      get_auto_update();
      
      HTTPClient http;
      
      // ----------------------------------------------------------------
      // server path for checking file version
      String serverPath2 = update + "?chip_id=" + chipId + "&type=pulse_p1-poort" + "&version=" + version + "&auto_update=" + update_settings;
      
      // ----------------------------------------------------------------
      // Start generated link above
      http.begin(serverPath2.c_str());
      
      // ----------------------------------------------------------------
      // Send HTTP GET request
      int httpResponseCode2 = http.GET();
      
      if (httpResponseCode2>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode2);
        String payload2 = http.getString();
        String response2 = payload2;
        Serial.println("update=" + payload2);
        if(payload2 != "False" && payload2 != "Error with given credentials."){
          
          totalLength = http.getSize();

          // ----------------------------------------------------------------
          // transfer to local variable
          int len = totalLength;

            //turning off the pins so the update can go through
            pinMode(25, INPUT_PULLDOWN); // Set GPIO25 as digital output pin
            attachInterrupt(digitalPinToInterrupt(25), pulse1, DISABLED); 

            pinMode(19, INPUT_PULLDOWN); // Set GPIO19 as digital output pin
            attachInterrupt(digitalPinToInterrupt(19), pulse2, DISABLED); 

            pinMode(23, INPUT_PULLDOWN); // Set GPIO22 as digital output pin
            attachInterrupt(digitalPinToInterrupt(23), pulse3, DISABLED); 

            pinMode(18, INPUT_PULLDOWN); // Set GPIO18 as digital output pin
            attachInterrupt(digitalPinToInterrupt(18), pulse4, DISABLED); 

            pinMode(21, INPUT_PULLDOWN); // Set GPIO22 as digital output pin
            attachInterrupt(digitalPinToInterrupt(21), pulsegas, DISABLED); 

            pinMode(5, INPUT_PULLDOWN); // Set GPIO5 as digital output pin
            attachInterrupt(digitalPinToInterrupt(5), pulsewater, DISABLED); 
          
          // ----------------------------------------------------------------
          // this is required to start firmware update process
          Update.begin(UPDATE_SIZE_UNKNOWN);
          Serial.printf("FW Size: %u\n",totalLength);
          // ----------------------------------------------------------------
          // create buffer for read
          uint8_t buff[128] = { 0 };

          // ----------------------------------------------------------------
          // get tcp stream
          WiFiClient * stream = http.getStreamPtr();

          // ----------------------------------------------------------------
          // read all data from server
          Serial.println("Updating firmware...");
          while(http.connected() && (len > 0 || len == -1)) {

            // ----------------------------------------------------------------
            // get available data size
            size_t size = stream->available();
            if(size) {

              int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

              // ----------------------------------------------------------------
              // pass to function
              updateFirmware(buff, c);
                
              if(len > 0) {
                len -= c;
              }
            }
            delay(1);
          }

        }
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode2);
      }
      http.end();
      
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTimeOTA = millis();
  } 
}