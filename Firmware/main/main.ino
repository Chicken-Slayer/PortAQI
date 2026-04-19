/*
PortAQI - Tejas Joshi
Complete project details at: https://github.com/Chicken-Slayer/PortAQI
*/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SensirionI2cScd30.h>
#include <Wire.h>
#include <SdsDustSensor.h>

// Don't know what this really does
#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

// Pin definitions:
const int OLED_SCL = 0;
const int OLED_SDA = 1;

const int rxPin = 20;
const int txPin = 21;

// SCD30 Configuration:
SensirionI2cScd30 CO2sensor;
static char errorMessage[64];
static int16_t error;


// SDS011 Configuration:

SoftwareSerial softwareSerial(SDS_RX_PIN, SDS_TX_PIN);
SdsDustSensor sds(softwareSerial);

const int MINUTE = 60000;
const int WAKEUP_WORKING_TIME = 30000; // 30 seconds.
const int MEASUREMENT_INTERVAL = 5 * MINUTE;

// OLED Display (pixels)
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 48

// ---

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("SDS011 PM Sensor ")
  delay(500);

  sds.begin();
  Serial.print("SDS011 ");
  Serial.println(sds.queryFirmwareVersion().toString());
  
  // Ensures SDS011 is in 'query' reporting mode:
  Serial.println(sds.setQueryReportingMode().toString());

}

void loop() {
  // waking up SDS011
  sds.wakeup();
  delay(WAKEUP_WORKING_TIME);

  // Get data from SDS011
  PmResult pm = sds.queryPm();
  if (pm.isOk()) {
    Serial.print("PM2.5 = ");
    Serial.print(pm.pm25); // float, μg/m3
    Serial.print(", PM10 = ");
    Serial.println(pm.pm10);
  } else {
    Serial.print("Could not read values from sensor, reason: ");
    Serial.println(pm.statusToString());
}
}
