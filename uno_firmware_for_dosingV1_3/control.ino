
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
    Take the topic, find the last slash and return the word after it. strrchr locate last occurrence of a character in string
    "SS1/sub/Sensor_0" return Sensor_0;
  */
  char* find_the_slash = strrchr(topic, '/');
  char* output_str = find_the_slash ? find_the_slash + 1 : topic;
  return output_str;
}


void control_commands(char* topic, char* payload, int payload_length) {
  //Gas_Dispenser/sub/bed-environment/owner=JON/R1/solenoid_on_time_sec

  int array_index = 10;

  char* commands[7] = { "reset", "oxygen_target_level", "solenoid_on_time_sec", "solenoid_off_time_sec", "solenoid_cycles", "dispense_paused_period_sec" };

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
      delay(4500);
      break;
    case 1:
      oxygen_target_level = payloadCovertedToFloat(payload, payload_length);
      EEPROM.put(0, oxygen_target_level);
      break;
    case 2:
      solenoid_on_time_sec = payloadCovertedToInt(payload, payload_length);
      EEPROM.put(5, solenoid_on_time_sec);
      break;
    case 3:
      solenoid_off_time_sec = payloadCovertedToInt(payload, payload_length);
      EEPROM.put(10, solenoid_off_time_sec);
      break;
    case 4:
      solenoid_cycles = payloadCovertedToInt(payload, payload_length);
      EEPROM.put(15, solenoid_cycles);
      break;
    case 5:
      dispense_paused_period_sec = payloadCovertedToInt(payload, payload_length);
      if(dispense_paused_period_sec < solenoid_on_time_sec){
        dispense_paused_period_sec = solenoid_on_time_sec + 60;
      }
      EEPROM.put(20, dispense_paused_period_sec);
      break;
  }
}
