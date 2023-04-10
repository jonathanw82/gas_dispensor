void debug() {
  static float error_val = oxygen_target_level - actual_oxygen_level;

  Serial.println("");
  Serial.print("Safe Oxygen level ");
  Serial.print(safe_oxygen_level);
  Serial.println(" %vol");
  Serial.print("oxygen concetnration is ");
  Serial.print(actual_oxygen_level);
  Serial.println("% VOL");
  Serial.print("Gas output ");
  Serial.println(gas_output);
  Serial.print("gas_P_response ");
  Serial.println(gasPID.P_response);
  Serial.print("gas_I_response ");
  Serial.println(gasPID.I_response);
  Serial.print("gas_D_response ");
  Serial.println(gasPID.D_response);
  Serial.print("Gas target ");
  Serial.println(oxygen_target_level);
  Serial.print("Gas Error ");
  Serial.println(error_val);
  Serial.print("Gas on time ");
  Serial.println(gas_on_time);
  Serial.print("Gas off time ");
  Serial.println(gas_off_time);
  Serial.print("solenoid pulse interval ");
  Serial.println(solenoid_pulse_interval);
  Serial.print("PID compute interval ");
  Serial.println(pid_compute_interval);
  Serial.print("P ");
  Serial.println(P);
  Serial.print("I ");
  Serial.println(I);
  Serial.print("D ");
  Serial.println(D);
  
}
