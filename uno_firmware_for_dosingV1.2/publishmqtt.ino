void publishMQTT() {

  static float error_val = bed_oxygen_level - oxygen_target_level;

  char pubpath[200];
  strcpy(pubpath, PUBLISH_PATH);
  strcat(pubpath, OWNER);
  strcat(pubpath, "/");
  strcat(pubpath, LOCATION);
  strcat(pubpath, "/");

  mqtt_client.publish(pubpath + String("bed_oxygen_level"), String(bed_oxygen_level));
  mqtt_client.publish(pubpath + String("oxygen_target_level"), String(oxygen_target_level));
  mqtt_client.publish(pubpath + String("solenoid_on_time_sec"), String(solenoid_on_time_sec));
  mqtt_client.publish(pubpath + String("solenoid_off_time_sec"), String(solenoid_off_time_sec));
  mqtt_client.publish(pubpath + String("solenoid_holdoff_interval_sec"), String(solenoid_holdoff_interval_sec));
  mqtt_client.publish(pubpath + String("solenoid_cycles"), String(solenoid_cycles));
  mqtt_client.publish(pubpath + String("solenoid_active"), String(check_pin_state(gas_output_solenoid)));
  mqtt_client.publish(pubpath + String("solenoid_holdoff_time_active"), String(solenoid_holdoff_timer));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~ Output pin state ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool check_pin_state(int pin) {
  bool state;
  return state = digitalRead(pin);  // Checks the status of the output pins
}
