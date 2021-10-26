/*
 * RP 26.10.2021
 * code working, tested with ESP32
 * https://werner.rothschopf.net/microcontroller/202103_arduino_esp32_ntp_en.htm
 */

#include <WiFi.h>
#include <time.h>

const char* ssid       = "xxx";
const char* password   = "xxx";

const char* MyTMzone = "CET-1CEST,M3.5.0/02,M10.5.0/03";        //https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

const char* ntpServer = "ntp1.t-online.de";

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Month: ");
  Serial.println((timeinfo.tm_mon) + 1);      //-> month starts at '0', October is value '9'
}

void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  
  // ESP32 seems to be a little more complex:
  //++++++++++++++++++++++++++++++++++++++++++++
  configTime(0, 0, ntpServer);
  setenv("TZ", MyTMzone, 1);
  tzset();
  //++++++++++++++++++++++++++++++++++++++++++++
  
  // ESP8266
  //++++++++++++++++++++++++++++++++++++++++++++
  //configTime(MyTMzone, ntpServer); // --> for the ESP8266 only
  //++++++++++++++++++++++++++++++++++++++++++++
  
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  delay(1000);
  printLocalTime();
}
