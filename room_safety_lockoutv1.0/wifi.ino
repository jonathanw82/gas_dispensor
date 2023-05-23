void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  setUpMqtt();
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
  Serial.println(WiFi.status());
  Serial.println("Wifi not connected");
  // WiFi.end();
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