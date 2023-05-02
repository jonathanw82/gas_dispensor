void publishMQTT() {

  static float error_val = bed_oxygen_level - oxygen_target_level;

  char pubpath[200];
  strcpy(pubpath, PUBLISH_PATH);
  strcat(pubpath, OWNER);
  strcat(pubpath, "/");
  strcat(pubpath, LOCATION);
  strcat(pubpath, "/");

  mqtt_client.publish(pubpath + String("room_oxygen_level"), String(room_oxygen_level));
  mqtt_client.publish(pubpath + String("bed_oxygen_level"), String(bed_oxygen_level));
  wdt_reset();
  mqtt_client.publish(pubpath + String("P"), String(gasPID.P));
  mqtt_client.publish(pubpath + String("I"), String(gasPID.I));
  mqtt_client.publish(pubpath + String("D"), String(gasPID.D));
  wdt_reset();
  mqtt_client.publish(pubpath + String("oxygen_target_level"), String(oxygen_target_level));
  mqtt_client.publish(pubpath + String("PID_gas_error"), String(error_val));
  mqtt_client.publish(pubpath + String("PID_gas_output"), String(gas_output));
  wdt_reset();
  mqtt_client.publish(pubpath + String("gas_P_response"), String(gasPID.P_response));
  mqtt_client.publish(pubpath + String("gas_I_response"), String(gasPID.I_response));
  mqtt_client.publish(pubpath + String("gas_D_response"), String(gasPID.D_response));
  wdt_reset();
  mqtt_client.publish(pubpath + String("gas_on_time"), String(gas_on_time));
  mqtt_client.publish(pubpath + String("gas_off_time"), String(gas_off_time));
  mqtt_client.publish(pubpath + String("solenoid_pulse_interval"), String(solenoid_pulse_interval));
  mqtt_client.publish(pubpath + String("time_period"), String(time_period));
  wdt_reset();
  mqtt_client.publish(pubpath + String("solenoid_active"), String(check_pin_state(gas_output_solenoid)));
  mqtt_client.publish(pubpath + String("Room_safety_lockout"), String(safety_lockout));
  mqtt_client.publish(pubpath + String("runaway_lockout"), String(runaway_lockout));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~ Output pin state ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool check_pin_state(int pin) {
  bool state;
  return state = digitalRead(pin);  // Checks the status of the output pins
}
