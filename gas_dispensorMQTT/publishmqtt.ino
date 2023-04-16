void publishMQTT(){

  static float error_val = oxygen_target_level - actual_oxygen_level;

  char pubpath[200];
  strcpy(pubpath, SUBSCRIBE_PATH);
  strcat(pubpath, OWNER);
  strcat(pubpath, "/");
  strcat(pubpath, LOCATION);
  strcat(pubpath, "/");

  // strcat(pubpath, endpoint);
  // strcat(dest, subpath);

  mqtt_client.publish(pubpath + String("Room_Oxygen_level"), String(safe_oxygen_level));
  mqtt_client.publish(pubpath + String("Bed_Oxygen_level"), String(actual_oxygen_level));
  mqtt_client.publish(pubpath + String("P"), String(P));
  mqtt_client.publish(pubpath + String("I"), String(I));
  mqtt_client.publish(pubpath + String("D"), String(D));
  mqtt_client.publish(pubpath + String("Oxygen_Level_Target"), String(oxygen_target_level));
  mqtt_client.publish(pubpath + String("PID_gas_error"), String(error_val));
  mqtt_client.publish(pubpath + String("PID_Gas_output"), String(gas_output));
  mqtt_client.publish(pubpath + String("Gas_P_response"), String(gasPID.P_response));
  mqtt_client.publish(pubpath + String("Gas_I_response"), String(gasPID.I_response));
  mqtt_client.publish(pubpath + String("Gas_D_response"), String(gasPID.D_response));
  mqtt_client.publish(pubpath + String("Gas_on_time"), String(gas_on_time));
  mqtt_client.publish(pubpath + String("Gas_off_time"), String(gas_off_time));
  mqtt_client.publish(pubpath + String("solenoid_pulse_interval"), String(solenoid_pulse_interval));
  mqtt_client.publish(pubpath + String("Solenoid_active"), String(check_pin_state(gas_output_solenoid)));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~ Output pin state ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool check_pin_state(int pin) {
  bool state;
  return state = digitalRead(pin);      // Checks the status of the output pins
}
