//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   EEPROM  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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