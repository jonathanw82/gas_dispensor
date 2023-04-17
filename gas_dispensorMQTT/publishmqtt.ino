void publishMQTT(){

  static float error_val = actual_oxygen_level - oxygen_target_level;

  char pubpath[200];
  strcpy(pubpath, PUBLISH_PATH);
  strcat(pubpath, OWNER);
  strcat(pubpath, "/");
  strcat(pubpath, LOCATION);
  strcat(pubpath, "/");

  // strcat(pubpath, endpoint);
  // strcat(dest, subpath);

  mqtt_client.publish(pubpath + String("Room_Oxygen_level"), String(safe_oxygen_level));
  mqtt_client.publish(pubpath + String("Bed_Oxygen_level"), String(actual_oxygen_level));
  mqtt_client.publish(pubpath + String("P"), String(gasPID.P));
  mqtt_client.publish(pubpath + String("I"), String(gasPID.I));
  mqtt_client.publish(pubpath + String("D"), String(gasPID.D));
  mqtt_client.publish(pubpath + String("oxygen_target_level"), String(oxygen_target_level));
  mqtt_client.publish(pubpath + String("PID_gas_error"), String(error_val));
  mqtt_client.publish(pubpath + String("PID_Gas_output"), String(gas_output));
  mqtt_client.publish(pubpath + String("Gas_P_response"), String(gasPID.P_response));
  mqtt_client.publish(pubpath + String("Gas_I_response"), String(gasPID.I_response));
  mqtt_client.publish(pubpath + String("Gas_D_response"), String(gasPID.D_response));
  mqtt_client.publish(pubpath + String("Gas_on_time"), String(gas_on_time));
  mqtt_client.publish(pubpath + String("Gas_off_time"), String(gas_off_time));
  mqtt_client.publish(pubpath + String("solenoid_pulse_interval"), String(solenoid_pulse_interval));
  mqtt_client.publish(pubpath + String("time_period"), String(time_period));
  mqtt_client.publish(pubpath + String("Solenoid_active"), String(check_pin_state(gas_output_solenoid)));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~ Output pin state ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool check_pin_state(int pin) {
  bool state;
  return state = digitalRead(pin);      // Checks the status of the output pins
}
