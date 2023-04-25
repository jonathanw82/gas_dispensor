
float payloadCovertedToFloat(char* payload, int payload_length) {
  payload[payload_length] = '\0';
  String s = String((char*)payload);  // Convert the incomming string to an float
  float stringtofloat = s.toFloat();
  return stringtofloat;
}

int payloadCovertedToInt(char* payload, int payload_length) {
  payload[payload_length] = '\0';
  String s = String((char*)payload);  // Convert the incomming string to an Int
  int stringtoint = s.toInt();
  return stringtoint;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ String comparason ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char* str_return_command(char* topic) {
  /* 
    Take the topic, find the last slash and return the word after it.
    strrchr locate last occurrence of a character in string
    "SS1/sub/Sensor_0" return Sensor_0;
  */
  char* find_the_slash = strrchr(topic, '/');
  char* output_str = find_the_slash ? find_the_slash + 1 : topic;
  return output_str;
}


void control_commands(char* topic, char* payload, int payload_length) {

  int array_index = 0;

  char* commands[7] = { "reset", "P", "I", "D", "oxygen_target_level", "time_period", "solenoid_pulse_interval" };

  for (int i = 0; i < 7; i++) {
    if (!strcmp(str_return_command(topic), commands[i])) {
      array_index = i;
      break;
    }
  }

  switch (array_index) {
    Serial.print(F("Array index = "));
    Serial.println(array_index);
    case 0:
      Serial.print(F("Reset here"));
      delay(2500);
      break;
    case 1:
      gasPID.P = payloadCovertedToFloat(payload, payload_length);
      EEPROM.put(0, gasPID.P);
      break;
    case 2:
      gasPID.I = payloadCovertedToFloat(payload, payload_length);
      EEPROM.put(5, gasPID.I);
      break;
    case 3:
      gasPID.D = payloadCovertedToFloat(payload, payload_length);
      EEPROM.put(10, gasPID.D);
      break;
    case 4:
      oxygen_target_level = payloadCovertedToFloat(payload, payload_length);
      EEPROM.put(15, oxygen_target_level);
      break;
    case 5:
      time_period = payloadCovertedToInt(payload, payload_length);
      EEPROM.put(20, time_period);
      break;
    case 6:
      solenoid_pulse_interval = payloadCovertedToInt(payload, payload_length);
      EEPROM.put(25, solenoid_pulse_interval);
      break;
  }
}
