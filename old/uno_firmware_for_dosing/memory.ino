//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   EEPROM  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void get_EEprom() {
  /*
    This function gets the variable values from EEprom, and asigned then to there set variable names.
  */
  EEPROM.get(0, oxygen_target_level);
  EEPROM.get(5, solenoid_pulse_interval_sec);
  EEPROM.get(10, solenoid_cycles);
  EEPROM.get(15, solenoid_holdoff_interval_sec);
}