void publishMQTT() {

  char pubpath[200];
  strcpy(pubpath, PUBLISH_PATH);
  strcat(pubpath, OWNER);
  strcat(pubpath, "/");
  strcat(pubpath, LOCATION);
  strcat(pubpath, "/");

  mqtt_client.publish(pubpath + String("room_oxygen_level"), String(room_oxygen_level));
  mqtt_client.publish(pubpath + String("room_safety_lockout"), String(safety_lockout));
}
