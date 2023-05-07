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
  strcat(subpath, "room_safety");
  strcat(subpath, "/");
  strcat(subpath, endpoint);
  strcpy(pubpath, subpath);
  return pubpath;
}

GetSet send_room_oxygen_level(construct_mqtt_path("room_oxygen_level"));
GetSet send_status(construct_mqtt_path("system_status"));
GetSet send_room_oxygen_safe_level_min(construct_mqtt_path("room_oxygen_safe_level_min"));
GetSet send_safety_lockout(construct_mqtt_path("safety_lockout"));

void publishMQTT() {

  if(sensor_fault){
    send_status.set_string("** Warning Room Oxygen Sensor Error **", false);
  }else if(safety_lockout){
    send_status.set_string("Lockout Active", false);
  }else{
    send_status.set_string("Active", false);
  }
  send_room_oxygen_level.set_float(room_oxygen_level, false);
  send_room_oxygen_safe_level_min.set_float(room_oxygen_safe_level_min, true);
  send_safety_lockout.set_bool(safety_lockout, false);
}
