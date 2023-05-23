bool wifi() {
  static int isWifiConnectedCounter = 0;
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println(F("Communication with WiFi module failed!"));
    // don't continue
    while (true);
  }

  while (status != WL_CONNECTED) {                                // attempt to connect to WiFi network:
    Serial.println(F("Attempting to connect to wifi network"));
    Serial.print(F("WiFi name: "));
    Serial.println(WIFI_NAME);
    Serial.print(F("WiFi password: "));
    Serial.println(WIFI_PASSWORD);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
    // wait 5 seconds for connection:
    delay(2000);
    // you're connected now, so print out the data:
    if (status != WL_CONNECTED) {
      isWifiConnectedCounter++;
      Serial.println(F("Wifi Not connected"));
      if (isWifiConnectedCounter >= 5) {
        Serial.println(F("Wifi Not connected check connection"));
        isWifiConnectedCounter = 0;
        return false;
      }
    } else {
      Serial.println(F("connection successfull!"));
      setUpMqtt();                              // Setup The MQTT protacol
      byte mac[6];
      WiFi.macAddress(mac);
      getMacAddress(mac);
      isWifiConnectedCounter = 0;
      return true;
    }
  }
}

bool maintain_wifi_connection() {
  static unsigned long last_wifi_check;
    // only attempt to connect once every 10 seconds
  if (millis() - last_wifi_check < 5000) {
    return true;
  }
  last_wifi_check = millis();
  if (WiFi.status() == WL_CONNECTED)
  {
    return true;
  }
  activate_solenoid(reset_activate_solenoid);
  Serial.println(WiFi.status());
  Serial.println("Wifi not connected");
  WiFi.end();
  WiFi.disconnect();
  Serial.println("Attempted wifi restart");
  Serial.print("WiFi name: ");
  Serial.println(WIFI_NAME);
  Serial.print("WiFi password: ");
  Serial.println(WIFI_PASSWORD);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  delay(1000);
  // check ip address
  IPAddress ip = WiFi.localIP();
  char ip_string[17];
  sprintf(ip_string, "%d.%d.%d.%d", ip[0],ip[1],ip[2],ip[3]);
  // ip_str(ip_string, ip);
  Serial.print("IP Address: ");
  Serial.println(ip_string);
  if (WiFi.status() != WL_CONNECTED || !strcmp(ip_string, "0.0.0.0"))
  {
    Serial.println("Connection failed.");
    return false;
  }
  Serial.println("Connection sucessful.");
  return true;
}

void getMacAddress(byte mac[]) {
    sprintf(MACADDRESS, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}
