
void publishMQTT() {

  char subpath[200];
  strcpy(subpath, PUBLISH_PATH);
  strcat(subpath, OWNER);
  strcat(subpath, "/");
  strcat(subpath, LOCATION);
  strcat(subpath, "/");


  if(sensor_fault){
     mqtt_client.publish(subpath + String("room_oxygen_level"), String("** Warning Sensor Error**"));
  }else{
     mqtt_client.publish(subpath + String("room_oxygen_level"), String(room_oxygen_level));
  }
  mqtt_client.publish(subpath + String("room_oxygen_level_minimum"), String(room_oxygen_safe_level_min));
  mqtt_client.publish(subpath + String("room_safety_lockout_active"), String(safety_lockout));
}
