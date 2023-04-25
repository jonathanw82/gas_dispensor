/*
  For more information on this code https://github.com/jonathanw82/gas_dispensor
*/

#include <DFRobot_MultiGasSensor.h>
#include <DFRobot_EOxygenSensor.h>
#include <Wire.h>     // I2c enable Lib
#include <avr/wdt.h>  // Watchdog Lib
#include <SPI.h>  // Allow access to serial
#include <PID.h>
#include <WiFiNINA.h>
#include <MQTT.h>
#include <EEPROM.h>

#define WIFI_NAME "TP-LINK_Extender_2.4GHz"
#define WIFI_PASSWORD "n6qYCUw46aUMv7"
#define MQTT_HOST "192.168.1.88"
#define PUBLISH_PATH "Gas_Dispenser/"
#define SUBSCRIBE_PATH "Gas_Dispenser/sub/"
#define DEVICE_NAME "Gas_Dispenser"
char LOCATION[5] = "R1";
char OWNER[10] = "owner=JON";
int status = WL_IDLE_STATUS;
MQTTClient mqtt_client;
WiFiClient www_client;
long last_connection_attempt = 0;
int isWifiConnectedCounter = 0;
int isMQTTConnectedCounter = 0;


const uint8_t on = HIGH;
const uint8_t off = LOW;
float safe_oxygen_level = 0;
float safe_oxygen_level_min = 19.5;
bool safety_shut_down = false;
float actual_oxygen_level = 0;
float oxygen_target_level = 10;  // target for oxygen is 10%
int time_period = 30;
float gas_on_time = 0;
float gas_off_time = 0;
float gas_output = 0;
unsigned long prev_Compute_time;
uint16_t pid_compute_interval = 5000;
uint16_t solenoid_pulse_interval = 1000;


//~~~~~~~~~~~~~~~~~~~~~~~~~~ Decalre Controllino pins with alias  ~~~~~~~~~~~~~~~~~~~~~~~~
const uint8_t gas_output_solenoid = LED_BUILTIN;
const uint8_t warning_beacon = 2;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Decalre Oxygen sensor  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DFRobot_EOxygenSensor_I2C oxygen(&Wire, 0x70);  // main sensor in the grow bed

#define I2C_COMMUNICATION
DFRobot_GAS_I2C safetyOxygenSensor(&Wire, 0x74);  // safety sensor to shut down output if the oxgen level in the room is unsafe

// input, output, target,P,I,D
PID gasPID(&actual_oxygen_level, &gas_output, &oxygen_target_level, 10, 0.2, 0);  // Decalre the PID classes

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Watchdog ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void watchdogSetup() {
#ifdef ARDUINO_ARCH_MEGAAVR
  if (RSTCTRL.RSTFR & RSTCTRL_WDRF_bm) {
    Serial.println(F("It was a watchdog reset."));
  }
  RSTCTRL.RSTFR |= RSTCTRL_WDRF_bm ;
  wdt_enable(WDT_PERIOD_2KCLK_gc);
#endif
}

void setup() {
  Serial.begin(9600);
  wifi();
  Wire.begin();
  gasPID.init();  // Initialise Pid for temp and hum
  gasPID.max_out = 100;
  gasPID.min_out = -100;
  get_EEprom();
  pinMode(gas_output_solenoid, OUTPUT);
  digitalWrite(gas_output_solenoid, off);
  pinMode(warning_beacon, OUTPUT);
  digitalWrite(warning_beacon, off);
  //
  //safetyOxygenSensor.begin();
  //  while (!oxygen.begin()) {
  //    Serial.println("NO Deivces !");
  //    delay(1000);
  //  } Serial.println("Device connected successfully !");

  safetyOxygenSensor.changeAcquireMode(safetyOxygenSensor.PASSIVITY);
  watchdogSetup();
}

void loop() {
  wdt_reset();
  maintain_mqtt_connection();
  safetyCheck();
  if (!safety_shut_down) {
    check_time_values();
    time_control_loop();
    if (millis() - prev_Compute_time > pid_compute_interval) {  // Compute every 5 seconds
      prev_Compute_time = millis();
      get_oxygen_reading();
      gasPID.Compute();
      //debug();
      publishMQTT();
    }
  }
  mqtt_client.loop();
}

void safetyCheck() {
  //safe_oxygen_level = safetyOxygenSensor.readGasConcentrationPPM());
  static unsigned long timer = 0;
  safe_oxygen_level = 19.60;
  if (safe_oxygen_level <= safe_oxygen_level_min) {
    safety_shut_down = true;  // the system will have to phsically reset to start up again
  }

  if (safety_shut_down) {
    digitalWrite(gas_output_solenoid, off);
    digitalWrite(warning_beacon, on);
    if (millis() - timer < 1000) {
      return;
    }
    timer = millis();
    Serial.print(F("Safe Oxygen level "));
    //Serial.print(safetyOxygenSensor.readGasConcentrationPPM());
    Serial.print(safe_oxygen_level);
    Serial.println(F(" %vol"));
    Serial.println(F("********* DANGER OXYGEN LEVEL LOW SAFETY SHUTDOWN *********"));
    
    char pubpath[200];
    strcpy(pubpath, SUBSCRIBE_PATH);
    strcat(pubpath, OWNER);
    strcat(pubpath, "/");
    strcat(pubpath, LOCATION);
    strcat(pubpath, "/");
    mqtt_client.publish(pubpath + String("********* DANGER ROOM OXYGEN LEVEL LOW SHUTDOWN *********"));
  }
}


void get_oxygen_reading() {
  //  actual_oxygen_level = oxygen.readOxygenConcentration();
  actual_oxygen_level = 20.22;
  //  Serial.print("oxygen concetnration is ");
  //  Serial.print(oxygen_value);
  //  Serial.println("% VOL");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Time values ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void check_time_values() {
  // Convert a negative number to a positive number

  if (gas_output < 0) gas_on_time = (gas_output * time_period) / 100 * -1;  // Temp
  else gas_on_time = (gas_output * time_period) / 100;
  gas_off_time = time_period - gas_on_time;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Time loop control ~~~~~~~~~~~~~~~~~~~~~~~~~~
void time_control_loop() {
  static unsigned long time_loop_start = 0;

  if (millis() - time_loop_start > (time_period * 1000)) {
    time_loop_start = millis();
  }

  if (millis() - time_loop_start < (gas_on_time * 1000)) {
    if (gas_output > 0) {
      digitalWrite(gas_output_solenoid, off);
    } else {
      activate_solenoid();  // increases nitrogen level
    }
  } else {
    digitalWrite(gas_output_solenoid, off);
  }
}

void activate_solenoid() {
  static unsigned long solenoid_wait_timer;
  if (millis() - solenoid_wait_timer < solenoid_pulse_interval) {
    return;
  }
  solenoid_wait_timer = millis();
  digitalWrite(gas_output_solenoid, !digitalRead(gas_output_solenoid));  // pulse the gas_output solenoid to avoid freezing
}