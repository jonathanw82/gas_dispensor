#include <DFRobot_EOxygenSensor.h>
#include <Wire.h>                   // I2c enable Lib
#include <avr/wdt.h>                // Watchdog Lib
//#include <Controllino.h>            // Controllino Lib to allow aliases
#include <SPI.h>                    // Allow access to serial
#include <PID.h>

const uint8_t on = HIGH;
const uint8_t off = LOW;
float actual_oxygen_level = 0;
float oxygen_target_level = 10; // target for oxygen is 10%
float time_period = 30;
float gas_on_time = 0;
float gas_off_time = 0;
float gas_output = 0;
unsigned long solenoid_wait_timer;
unsigned long prev_Compute_time;
uint16_t pid_compute_interval = 5000;
uint16_t solenoid_pulse_interval = 1000;
float P = 10;
float I = 0.2;
float D = 0;

//~~~~~~~~~~~~~~~~~~~~~~~~~~ Decalre Controllino pins with alias  ~~~~~~~~~~~~~~~~~~~~~~~~
//int gas_output_solenoid = CONTROLLINO_D0;

const uint8_t gas_output_solenoid = LED_BUILTIN;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Decalre Oxygen sensor  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DFRobot_EOxygenSensor_I2C oxygen(&Wire, 0x70);

// input, output, target,P,I,D
PID gasPID(&actual_oxygen_level, &gas_output, &oxygen_target_level, P, I, D); // Decalre the PID classes

void setup() {
  Serial.begin(9600);
  Wire.begin();
  gasPID.init();                           // Initialise Pid for temp and hum
  gasPID.max_out = 100;
  gasPID.min_out = -100;
  pinMode(gas_output_solenoid, OUTPUT);
  //digitalWrite(gas_output_solenoid, off);

//  while (!oxygen.begin()) {
//    Serial.println("NO Deivces !");
//    delay(1000);
//  } Serial.println("Device connected successfully !");

  wdt_enable(WDTO_2S);
}

void loop() {
  wdt_reset();
  check_time_values();
  time_control_loop();
  if (millis() - prev_Compute_time > pid_compute_interval) { // Compute every 5 seconds
    prev_Compute_time = millis();
    get_oxygen_reading();
    gasPID.Compute();
    debug();
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

  if (gas_output < 0) gas_on_time = (gas_output * time_period) / 100 * -1;             // Temp
  else gas_on_time = (gas_output * time_period) / 100;
  gas_off_time = time_period - gas_on_time;

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Time loop control ~~~~~~~~~~~~~~~~~~~~~~~~~~
void time_control_loop() {
  static unsigned long time_loop_start = 0;

  if (millis() - time_loop_start  > (time_period * 1000)) {
    time_loop_start = millis();
  }

  if (millis() - time_loop_start < (gas_on_time * 1000)) {
    if (gas_output > 0) {
      digitalWrite(gas_output_solenoid, off);
    }
    else {
      activate_solenoid();  // increases nitrogen level 
    }
  }
  else {
    digitalWrite(gas_output_solenoid, off);
  }
}

void activate_solenoid() {
  if (millis() - solenoid_wait_timer < solenoid_pulse_interval) {
    return;
  }
  solenoid_wait_timer = millis();
  digitalWrite(gas_output_solenoid, !digitalRead(gas_output_solenoid));   // pulse the gas_output solenoid to avoid freezing
}
