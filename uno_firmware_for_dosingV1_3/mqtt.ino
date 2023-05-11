void setUpMqtt() {
  mqtt_client.begin(MQTT_HOST, 1883, www_client);
  mqtt_client.onMessageAdvanced(mqtt_message);
  mqtt_client.setTimeout(4000);
  mqtt_client.setKeepAlive(10);
}

// void mqtt_send_error()){

// }

//Gas_Dispenser/sub/owner=JON/R1/time_period
char path[200];
char* construct_path(char* endpoint){
  char subpath[200];
  strcpy(subpath, SUBSCRIBE_PATH);
  strcat(subpath, OWNER);
  strcat(subpath, "/");
  strcat(subpath, LOCATION);
  strcat(subpath, "/");
  strcat(subpath, endpoint);
  strcpy(path, subpath);
  return path;
}

void maintain_mqtt_connection() {
  static long last_connection_attempt = 0;
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
    Serial.println(WiFi.status());
    return;
  }
  Serial.println(F("success!"));
  wdt_reset();
  mqtt_client.subscribe(construct_path("reset"));
  mqtt_client.subscribe(construct_path("oxygen_target_level"));
  mqtt_client.subscribe(construct_path("solenoid_on_time_sec"));
  mqtt_client.subscribe(construct_path("solenoid_off_time_sec"));
  mqtt_client.subscribe(construct_path("solenoid_cycles"));
  mqtt_client.subscribe(construct_path("dispense_paused_period_sec"));
}

void mqtt_message(MQTTClient * client, char topic[], char payload[], int payload_length) {
  Serial.print("message recieved: ");
  Serial.print(topic);
  Serial.print(" = ");
  Serial.println(payload);
  control_commands(topic, payload, payload_length);
}
