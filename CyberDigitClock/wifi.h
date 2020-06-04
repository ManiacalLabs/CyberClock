#include "globals.h"
#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>

#define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())

#define LED_ON      HIGH
#define LED_OFF     LOW

// SSID and PW for Config Portal
String ssid = "CyberClock";

// SSID and PW for your Router
String Router_SSID;
String Router_Pass;

// Use false if you don't like to display Available Pages in Information Page of Config Portal
// Comment out or use true to display Available Pages in Information Page of Config Portal
// Must be placed before #include <ESP_WiFiManager.h>
#define USE_AVAILABLE_PAGES     true

#include <ESP_WiFiManager.h>    //v1.0.7 - https://github.com/khoih-prog/ESP_WiFiManager

// Onboard LED I/O pin on NodeMCU board
const int PIN_LED = 2; // D4 on NodeMCU and WeMos. GPIO2/ADC12 of ESP32. Controls the onboard LED.

void heartBeatPrint(void)
{
  static int num = 1;

  if (WiFi.status() == WL_CONNECTED)
    Serial.print("C");        // C means connected to WiFi
  else
    Serial.print("D");        // D means not connected to WiFi

  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(" ");
  }
}

#define HEARTBEAT_INTERVAL    10000L
void check_wifi_status()
{
  static ulong checkstatus_timeout = 0;
  // Print hearbeat every HEARTBEAT_INTERVAL (10) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = millis() + HEARTBEAT_INTERVAL;
  }
}

// For some unknown reason webserver can only be started once per boot up
// so webserver can not be used again in the sketch.
#define WIFI_CONNECT_TIMEOUT        60000L
#define WHILE_LOOP_DELAY            200L
#define WHILE_LOOP_STEPS            (WIFI_CONNECT_TIMEOUT / ( 3 * WHILE_LOOP_DELAY ))

void wifi_setup()
{
  // btStop(); // turn of BT because it interfers with my logitech remote
  
  pinMode(PIN_LED, OUTPUT);
  unsigned long startedAt = millis();

  digitalWrite(PIN_LED, LED_ON); // turn the LED on to tell us we are in configuration mode.

  //Local intialization. Once its business is done, there is no need to keep it around
  // Use this to default DHCP hostname to ESP8266-XXXXXX or ESP32-XXXXXX
  ESP_WiFiManager ESP_wifiManager;

  ESP_wifiManager.setMinimumSignalQuality(-1);
  // Set static IP, Gateway, Subnetmask, DNS1 and DNS2. New in v1.0.5
  ESP_wifiManager.setSTAStaticIPConfig(IPAddress(192, 168, 2, 2), //IP
                                       IPAddress(192, 168, 2, 1),   //Gateway
                                       IPAddress(255, 255, 255, 0), //Subnet Mask
                                       IPAddress(192, 168, 2, 1),   //DNS1
                                       IPAddress(8, 8, 8, 8));      //DNS2

  // We can't use WiFi.SSID() in ESP32as it's only valid after connected.
  // SSID and Password stored in ESP32 wifi_ap_record_t and wifi_config_t are also cleared in reboot
  // Have to create a new function to store in EEPROM/SPIFFS for this purpose
  Router_SSID = ESP_wifiManager.WiFi_SSID();
  Router_Pass = ESP_wifiManager.WiFi_Pass();

  //Remove this line if you do not want to see WiFi password printed
  Serial.println("Stored: SSID = " + Router_SSID + ", Pass = " + Router_Pass);

  //Check if there is stored WiFi router/password credentials.
  //If not found, device will remain in configuration mode until switched off via webserver.
  Serial.println("Opening configuration portal.");
  Serial.println("Please connect to 192.168.2.1");

  if (Router_SSID != "")
  {
    ESP_wifiManager.setConfigPortalTimeout(60); //If no access point name has been previously entered disable timeout.
    Serial.println("Timeout 60s");
  }
  else
    Serial.println("No timeout");

  // SSID to uppercase
  ssid.toUpperCase();
  
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  FastLED.show();
  FastLED.delay(10);

  //it starts an access point
  //and goes into a blocking loop awaiting configuration
  if (!ESP_wifiManager.startConfigPortal((const char *) ssid.c_str(), NULL))
    Serial.println("Not connected to WiFi but continuing anyway.");
  else
    Serial.println("WiFi connected... \\o/ :)");

  digitalWrite(PIN_LED, LED_OFF); // Turn led off as we are not in configuration mode.
  
  fill_solid(leds, NUM_LEDS, CRGB::Purple);
  FastLED.show();
  FastLED.delay(10);

  startedAt = millis();

  while ( (WiFi.status() != WL_CONNECTED) && (millis() - startedAt < WIFI_CONNECT_TIMEOUT ) )
  {
    WiFi.mode(WIFI_STA);
    WiFi.persistent (true);
    // We start by connecting to a WiFi network

    Serial.print("Connecting to ");
    Serial.println(Router_SSID);

    WiFi.begin(Router_SSID.c_str(), Router_Pass.c_str());

    int i = 0;
    while ((!WiFi.status() || WiFi.status() >= WL_DISCONNECTED) && i++ < WHILE_LOOP_STEPS)
    {
      delay(WHILE_LOOP_DELAY);
    }
  }

  Serial.print("After waiting ");
  Serial.print((millis() - startedAt) / 1000);
  Serial.print(" secs more in setup(), connection result is ");

  CRGB status_color = CRGB::Green;
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("connected. Local IP: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.print("Failed to connect.");
    status_color = CRGB::Red;
  }
  
  
  fill_solid(leds, NUM_LEDS, status_color);
  FastLED.show();
  FastLED.delay(500);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  FastLED.delay(500);
  fill_solid(leds, NUM_LEDS, status_color);
  FastLED.show();
  FastLED.delay(500);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  FastLED.delay(500);
  fill_solid(leds, NUM_LEDS, status_color);
  FastLED.show();
  FastLED.delay(500);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  FastLED.delay(500);
}