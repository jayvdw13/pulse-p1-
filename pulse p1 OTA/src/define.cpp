#include <Arduino.h>

uint32_t chipId = 0;
String chipId2 = "";
int i;

String serverName = "http://flexgreen.eu/pulse/pulse_register";
String serverName2 = "http://flexgreen.eu/p1/p1_register";
String update = "http://157.245.68.207/esp/update";
String get_update_settings = "http://flexgreen.eu/esp/esp_settings";

volatile unsigned long count1=0;
unsigned long prior_count1=0;

volatile unsigned long count2=0;
unsigned long prior_count2=0;

volatile unsigned long count3=0;
unsigned long prior_count3=0;

volatile unsigned long count4=0;
unsigned long prior_count4=0;

volatile unsigned long countgas=0;
unsigned long prior_countgas=0;

volatile unsigned long countwater=0;
unsigned long prior_countwater=0;

unsigned long lastTimeOTA = 0;  
unsigned long timerDelayOTA = 100000;

String payload = "0";
String payload2 = "0";
String payload3 = "0";
String payload4 = "0";
String payloadgas = "0";
String payloadwater = "0";
String update_settings;
String payload6;

unsigned long lastTime = 0; 
unsigned long timerDelay = 9000;

char input; 
bool readnextLine = false; 
    int messageCount = 0;
int bufpos = 0; 
long mEVLT = 0; 
long mEVHT = 0; 
long mETLT = 0; 
long mETHT = 0; 
long mEAV = 0;  
long mEAT = 0;  
float mG = 0;   

long mEVLTData = 0; 
long mEVHTData = 0; 
long mETLTData = 0; 
long mETHTData = 0; 
long mEAVData = 0;  
long mEATData = 0;  
float mGData = 0;   