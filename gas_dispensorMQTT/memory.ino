//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Write to EEPROM  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void write_to_EEprom() {
  /*
    This function writes the variables to EEprom, using the (put) method only allows data to be written to memory
    if it has changed else it get ignored.
  */
  EEPROM.put(0, gasPID.P);
  EEPROM.put(5, gasPID.I);
  EEPROM.put(10, gasPID.D);
  EEPROM.put(15, oxygen_target_level);
  EEPROM.put(20, time_period);
  EEPROM.put(25, solenoid_pulse_interval);
}

void get_EEprom() {
  /*
    This function gets the variable values from EEprom, and asigned then to there set variable names.
  */
  EEPROM.get(0, gasPID.P);
  EEPROM.get(5, gasPID.I);
  EEPROM.get(10, gasPID.D);
  EEPROM.get(15, oxygen_target_level);
  EEPROM.get(20, time_period);
  EEPROM.get(25, solenoid_pulse_interval);
}