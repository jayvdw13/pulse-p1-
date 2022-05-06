/*#include "SPIFFS.h"
#include "Update.h"  
void UpdateSpiffs() {
   Serial.begin(115200);
  
  if(WiFi.status()== WL_CONNECTED){
    



   if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
   }
   
    File file = SPIFFS.open("/firmware.bin");
  
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }
       
    Serial.println("Starting update..");
        
 
    size_t fileSize = file.size();
 
    if(!Update.begin(fileSize)){
       
       Serial.println("Cannot do the update");
       return;
    };
 
    Update.writeStream(file);
 
    if(Update.end()){
       
      Serial.println("Successful update");  
    }else {
       
      Serial.println("Error Occurred: " + String(Update.getError()));
      return;
    }
     
    file.close();
 
    Serial.println("Reset in 4 seconds...");
    delay(4000);
 
    ESP.restart();
}
*/