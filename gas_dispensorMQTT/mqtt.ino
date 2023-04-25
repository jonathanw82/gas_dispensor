void setUpMqtt() {
  mqtt_client.begin(MQTT_HOST, 1883, www_client);
  mqtt_client.onMessageAdvanced(mqtt_message);
}

void construct_path(char* dest ,char* endpoint){
  char subpath[200];
  strcpy(subpath, SUBSCRIBE_PATH);
  strcat(subpath, OWNER);
  strcat(subpath, "/");
  strcat(subpath, LOCATION);
  strcat(subpath, "/");
  strcat(subpath, endpoint);
  strcpy(dest, subpath);
}

void maintain_mqtt_connection() {
  if (mqtt_client.connected()) {
    return;
  }
  // only attempt to connect once a second
  if (millis() - last_connection_attempt < 1000) {
    return;
  }
  last_connection_attempt = millis();
  Serial.print(F("Connecting to MQTT host \""));
  Serial.print(MQTT_HOST);
  Serial.print(F("\" ... "));
  if (!mqtt_client.connect(DEVICE_NAME)) {
    Serial.println(F(" connection failed."));
    return;
  }
  Serial.println(F("success!"));

  char path[200];

  construct_path(path, "reset");
  mqtt_client.subscribe(path);
  construct_path(path, "P");
  mqtt_client.subscribe(path);
  construct_path(path, "I");
  mqtt_client.subscribe(path); 
  construct_path(path, "D");
  mqtt_client.subscribe(path);
  construct_path(path, "oxygen_target_level");
  mqtt_client.subscribe(path);
  construct_path(path, "solenoid_pulse_interval");
  mqtt_client.subscribe(path);
  construct_path(path, "time_period");
  mqtt_client.subscribe(path);
}

void mqtt_message(MQTTClient * client, char topic[], char payload[], int payload_length) {
  Serial.print("message recieved: ");
  Serial.print(topic);
  Serial.print(" = ");
  Serial.println(payload);
  control_commands(topic, payload, payload_length);
}
