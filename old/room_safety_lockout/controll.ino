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
  //Gas_Dispenser/sub/owner=JON/R1/reset
  int array_index = 1;

  char* commands[1] = { "reset"};

  for (int i = 0; i < 7; i++) {
    if (!strcmp(str_return_command(topic), commands[i])) {
      array_index = i;
      break;
    }
  }

  switch (array_index) {
    case 0:
      Serial.print(F("User called reset"));
      resetFunc();
      break;
  }
}
