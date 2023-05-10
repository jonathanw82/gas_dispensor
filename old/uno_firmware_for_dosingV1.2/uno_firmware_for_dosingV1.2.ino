#include <DFRobot_MultiGasSensor.h>
#include <DFRobot_EOxygenSensor.h>
#include <Wire.h>     // I2c enable Lib
#include <avr/wdt.h>  // Watchdog Lib
#include <SPI.h>      // Allow access to serial
#include <WiFiNINA.h>
#include <MQTT.h>
#include <EEPROM.h>
#include <LGcredentials.h>

#define WIFI_NAME ssid
#define WIFI_PASSWORD wifipassword
#define MQTT_HOST mqtt_host_name
#define SUBSCRIBE_PATH "Gas_Dispenser/sub/"
#define DEVICE_NAME "Gas_Dispenser"
char PUBLISH_PATH[40] = "sensor/Gas_Dispenser/";
char MACADDRESS[18];  // 00:00:00:00:00:00
char LOCATION[5] = "R1";
char OWNER[10] = "owner=JON";
int status = WL_IDLE_STATUS;
MQTTClient mqtt_client;
WiFiClient www_client;

const uint8_t on = HIGH;
const uint8_t off = LOW;
float bed_oxygen_level = 0;
float oxygen_target_level = 10;               // target for oxygen is 10%
unsigned long solenoid_on_time_sec = 1;
unsigned long solenoid_off_time_sec = 42;
uint16_t dispense_paused_period_sec = 10;  // time to allow the gas to settle bewtween cycles this is in seconds
uint16_t solenoid_cycles = 0;
uint16_t mqtt_publish_interval_sec = 2;  // publish mqtt in seconds
bool solenoid_paused_timer = false;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Decalre control pins  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const uint8_t gas_output_solenoid = 6;
const uint8_t abmer_solenoid_active_led = 5;

DFRobot_EOxygenSensor_I2C bed_oxygen_sensor(&Wire, 0x70);  // main sensor in the grow bed

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Watchdog ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void watchdogSetup() {
#ifdef ARDUINO_ARCH_MEGAAVR
  if (RSTCTRL.RSTFR & RSTCTRL_WDRF_bm) {
    Serial.println(F("It was a watchdog reset."));
  }
  RSTCTRL.RSTFR |= RSTCTRL_WDRF_bm;
  wdt_enable(WDT_PERIOD_4KCLK_gc);
#endif
}

void setup() {
  Serial.begin(9600);
  wifi();
  Wire.begin();
  get_EEprom();
  pinMode(gas_output_solenoid, OUTPUT);
  digitalWrite(gas_output_solenoid, off);
  pinMode(abmer_solenoid_active_led, OUTPUT);
  digitalWrite(abmer_solenoid_active_led, off);

  while (!bed_oxygen_sensor.begin()) {
    Serial.println("NO Deivces !");
    delay(1000);
  }
  Serial.println("Bed oxygen sensor connected successfully !");

  watchdogSetup();
}

void loop() {
  wdt_reset();
  maintain_mqtt_connection();
  mqtt_client.loop();
  manage_oxygen_level();

  static unsigned long publish_timer;
  if (millis() - publish_timer > mqtt_publish_interval_sec * 1000) {
    publish_timer = millis();
    publishMQTT();
  }
}

float get_bed_oxygen_reading() {
   return bed_oxygen_level = bed_oxygen_sensor.readOxygenConcentration();
  //return bed_oxygen_level = 20;
}

void manage_oxygen_level() {
  static const bool reset_activate_solenoid = true;
  static const bool allow_activate_solenoid = false;

  if (get_bed_oxygen_reading() > oxygen_target_level) {
    activate_solenoid(allow_activate_solenoid);  // actiavte solenoid
  } else if (get_bed_oxygen_reading() <= oxygen_target_level) {
    activate_solenoid(reset_activate_solenoid);  // sensd true to activate the reset loop and shut down the solenoid
  }
}

void activate_solenoid(bool reset) {
  static unsigned long timer = 0;
  static unsigned long solenoid_wait_timer = 0;
  static int cycles = 0;
  static bool solenoid_active = false;


  if (reset) {
    // if true set all things off
    digitalWrite(gas_output_solenoid, off);
    digitalWrite(abmer_solenoid_active_led, off);
    cycles = 0;
    solenoid_paused_timer = false;
    solenoid_active = false;
    return;
  }

  if (solenoid_paused_timer) {
    // if true start waiting timer to allow gas to stablise.
    if (millis() - timer < dispense_paused_period_sec * 1000) {
      return;
    }
    timer = millis();
    solenoid_paused_timer = false;
  }

   static unsigned long soltime = 0;
  if (millis() - soltime >= (solenoid_active ? solenoid_on_time_sec * 1000 : solenoid_off_time_sec * 1000)) {
    soltime = millis();
    solenoid_active = !solenoid_active;
    digitalWrite(gas_output_solenoid, !digitalRead(gas_output_solenoid));  // pulse the gas_output solenoid to avoid freezing
    digitalWrite(abmer_solenoid_active_led, !digitalRead(abmer_solenoid_active_led));
    if (digitalRead(gas_output_solenoid)) Serial.println("Solenoid on"), cycles++, Serial.print("cycle = "),Serial.println(cycles);;
    if (!digitalRead(gas_output_solenoid)) Serial.println("Solenoid off");
  }

  if (cycles >= solenoid_cycles + 1) {
    cycles = 0;
    solenoid_paused_timer = true;
    solenoid_active = false;
    digitalWrite(gas_output_solenoid, off);
    digitalWrite(abmer_solenoid_active_led, off);
    timer = millis();
    Serial.println("Hold off timer active");
  }
}
