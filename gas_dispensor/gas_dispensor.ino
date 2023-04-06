#include <DFRobot_EOxygenSensor.h>
#include <Wire.h>                   // I2c enable Lib
#include <avr/wdt.h>                // Watchdog Lib
#include <Controllino.h>            // Controllino Lib to allow aliases
#include <SPI.h>                    // Allow access to serial
#include <PID.h>

int on = HIGH;
int off = LOW;
float oxygen_value = 0;
float target_gas = 10; // target for oxygen is 10%
float time_period = 30;
float gas_on_time = 0;
float gas_off_time = 0;
float gas_output = 0;
unsigned long wait_timer = 0;
unsigned long prev_Compute_time;
unsigned long compute_interval = 5000;

//~~~~~~~~~~~~~~~~~~~~~~~~~~ Decalre Controllino pins with alias  ~~~~~~~~~~~~~~~~~~~~~~~~
int gas_output_solonoid = CONTROLLINO_D0;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Decalre Oxygen sensor  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DFRobot_EOxygenSensor_I2C oxygen(&Wire, 0x70);


PID gasPID(&oxygen_value, &gas_output, &target_gas, 1, 1, 1); // Decalre the PID classes

void setup() {
  Serial.begin(9600);
  Wire.begin();
  gasPID.init();                           // Initialise Pid for temp and hum
  gasPID.max_out = 100;
  gasPID.min_out = -100;
  pinMode(gas_output_solonoid, OUTPUT);
  digitalWrite(gas_output_solonoid, off);

  while (!oxygen.begin()) {
    Serial.println("NO Deivces !");
    delay(1000);
  } Serial.println("Device connected successfully !");

  wdt_enable(WDTO_2S);
}

void loop() {
  wdt_reset();
  get_oxygen_reading();
  if (millis() - prev_Compute_time > compute_interval) { // Compute every 5 seconds
    prev_Compute_time = millis();
    gasPID.Compute();
  }
}

void get_oxygen_reading() {
  oxygen_value = oxygen.readOxygenConcentration();
  Serial.print("oxygen concetnration is ");
  Serial.print(oxygen_value);
  Serial.println("% VOL");
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
      activate_solonoid();
    }
    else {
      digitalWrite(gas_output_solonoid, off);
    }
  }
  else {
    digitalWrite(gas_output_solonoid, off);
  }
}

void activate_solonoid() {
  // pulse the gas_output solonoid
  static int interval_time = 1000;

  if (millis() - wait_timer < interval_time) {
    return;
  }
  wait_timer = millis();
  digitalWrite(gas_output_solonoid, !digitalRead(gas_output_solonoid));
}
