class GetSet {
public:
  GetSet(String topic) {
    internal_topic = topic;
  }
  void set_float(float x, bool j) {
    is_retain(j);
    if (x != float_val || initial_message_send) {
      float_val = x;
      mqtt_client.publish(internal_topic, String(float_val), is_retained, 1);
      initial_message_send = false;
      Serial.println(internal_topic + " " + float_val);
    }
  }
  void set_int(int x, bool j) {
    is_retain(j);
    if (x != int_val || initial_message_send) {
      int_val = x;
      mqtt_client.publish(internal_topic, String(int_val), is_retained, 1);
      initial_message_send = false;
      Serial.println(internal_topic + " " + int_val);
    }
  }
  void set_string(String x, bool j) {
    is_retain(j);
    if (x != string_val || initial_message_send) {
      string_val = x;
      mqtt_client.publish(internal_topic, String(string_val), is_retained, 1);
      initial_message_send = false;
      Serial.println(internal_topic + " " + string_val);
    }
  }
  void set_bool(bool x, bool j) {
    is_retain(j);
    if (x != bool_val || initial_message_send) {
      bool_val = x;
      mqtt_client.publish(internal_topic, String(bool_val), is_retained, 1);
      initial_message_send = false;
      Serial.println(internal_topic + " " + bool_val);
    }
  }
  void is_retain(bool j) {
    if (j) is_retained = true;
    else is_retained = false;
  }

private:
  int get_val;
  float float_val;
  int int_val;
  String internal_topic;
  String string_val;
  bool bool_val;
  bool is_retained;
  bool initial_message_send = true;

};

char pubpath[200];
char* construct_mqtt_path(char* endpoint){
  char subpath[200];
  strcpy(subpath, PUBLISH_PATH);
  strcat(subpath, OWNER);
  strcat(subpath, "/");
  strcat(subpath, LOCATION);
  strcat(subpath, "/");
  strcat(subpath, "bed_status");
  strcat(subpath, "/");
  strcat(subpath, endpoint);
  strcpy(pubpath, subpath);
  return pubpath;
}

GetSet send_bed_oxygen_level(construct_mqtt_path("bed_oxygen_level"));
GetSet send_bed_oxygen_level_error_val(construct_mqtt_path("bed_oxygen_level_error_val"));
GetSet send_oxygen_target_level(construct_mqtt_path("oxygen_target_level"));
GetSet send_solenoid_on_time_sec(construct_mqtt_path("solenoid_on_time_sec"));
GetSet send_solenoid_off_time_sec(construct_mqtt_path("solenoid_off_time_sec"));
GetSet send_dispense_paused_period_sec(construct_mqtt_path("dispense_paused_period_sec"));
GetSet send_solenoid_cycles(construct_mqtt_path("solenoid_cycles"));
GetSet send_current_solenoid_cycles(construct_mqtt_path("current_solenoid_cycles"));
GetSet send_solenoid_active(construct_mqtt_path("solenoid_active"));
GetSet send_solenoid_paused_timer_active(construct_mqtt_path("solenoid_paused_timer_active"));


void publishMQTT() {

  static float error_val = bed_oxygen_level - oxygen_target_level;

  send_bed_oxygen_level.set_float(bed_oxygen_level, false);
  send_bed_oxygen_level_error_val.set_float(error_val, false);
  send_oxygen_target_level.set_float(oxygen_target_level, true);
  send_solenoid_on_time_sec.set_int(solenoid_on_time_sec, true);
  send_solenoid_off_time_sec.set_int(solenoid_off_time_sec, true);
  send_dispense_paused_period_sec.set_int(dispense_paused_period_sec, true);
  send_solenoid_cycles.set_int(solenoid_cycles, true);
  send_current_solenoid_cycles.set_int(cycles, false);
  send_solenoid_active.set_bool(check_pin_state(gas_output_solenoid), false);
  send_solenoid_paused_timer_active.set_bool(solenoid_paused_timer, false);

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~ Output pin state ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool check_pin_state(int pin) {
  bool state;
  return state = digitalRead(pin);  // Checks the status of the output pins
}
