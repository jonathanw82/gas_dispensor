#include <ESP8266WiFi.h>
#include <DFRobot_MultiGasSensor.h>
#include <MQTT.h>
#include <OneWire.h>
#include <credentials.h>

#define WIFI_NAME ssid
#define WIFI_PASSWORD wifipassword
#define MQTT_HOST mqtt_host_name
#define SUBSCRIBE_PATH "Gas_Dispenser/sub/"
#define DEVICE_NAME "Gas_Dispenser"
//#define PUBLISH_PATH "Gas_Dispenser/"
char PUBLISH_PATH[30] = "sensor/bed-environment/";
char MACADDRESS[18];  // 00:00:00:00:00:00
char LOCATION[5] = "R1";
char OWNER[10] = "owner=JON";
int status = WL_IDLE_STATUS;
MQTTClient mqtt_client;
WiFiClient espClient;

const uint8_t on = HIGH;
const uint8_t off = LOW;
float room_oxygen_level = 0;
float room_oxygen_safe_level_min = 19.50;
bool safety_lockout = false;

const uint8_t gas_safety_relay = D5;
const uint8_t warning_beacon = D6;
const uint8_t red_lockout_led = D7;


#define I2C_COMMUNICATION
DFRobot_GAS_I2C roomSafetyOxygenSensor(&Wire, 0x74);  // safety sensor to shut down output if the oxgen level in the room is unsafe

void (*resetFunc)(void) = 0;

void setup() {
  Serial.begin(9600);
  setup_wifi();
  pinMode(gas_safety_relay, OUTPUT);
  digitalWrite(gas_safety_relay, off);
  pinMode(warning_beacon, OUTPUT);
  digitalWrite(warning_beacon, off);
  pinMode(red_lockout_led, OUTPUT);
  digitalWrite(red_lockout_led, off);

  while (!roomSafetyOxygenSensor.begin()) {
    Serial.println("NO Deivces !");
    delay(1000);
  }
  Serial.println("Room safety oxygen sensor connected successfully !");
  roomSafetyOxygenSensor.changeAcquireMode(roomSafetyOxygenSensor.PASSIVITY);
}

void loop() {
  maintain_mqtt_connection();
  mqtt_client.loop();
  roomOxygenLevelSafetyCheck();

  static unsigned long publish_timer;
  if (millis() - publish_timer > 2000) {
    publish_timer = millis();
    publishMQTT();
  }

  if (safety_lockout) {
    getRoomOxygenSample();
    lockoutLed();
    digitalWrite(gas_safety_relay, off);
    digitalWrite(warning_beacon, on);
    return;
  }
}

void getRoomOxygenSample() {
  static unsigned long timer = 0;
  if (millis() - timer < 2000) {
    return;
  }
  timer = millis();
  room_oxygen_level = roomSafetyOxygenSensor.readGasConcentrationPPM();
}

void roomOxygenLevelSafetyCheck() {
  getRoomOxygenSample();
  if (room_oxygen_level <= room_oxygen_safe_level_min) {
    safety_lockout = true;  // the system will have to phsically reset to start up again
    return;
  }
  digitalWrite(gas_safety_relay, on);
  digitalWrite(red_lockout_led, off);
  digitalWrite(warning_beacon, off);
}


void lockoutLed() {
  static unsigned long timer;
  if (millis() - timer < 1000) {
    return;
  }
  timer = millis();
  digitalWrite(red_lockout_led, !digitalRead(red_lockout_led));
}