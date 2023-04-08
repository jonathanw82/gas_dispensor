void debug() {
  static float error_val = target_gas - oxygen_value;


  Serial.print("oxygen concetnration is ");
  Serial.print(oxygen_value);
  Serial.println("% VOL");
  Serial.print("Gas output ");
  Serial.println(gas_output);
  Serial.print("Gas target ");
  Serial.println(target_gas);
  Serial.print("Gas Error ");
  Serial.println(error_val);
  Serial.print("Gas on time ");
  Serial.println(gas_on_time);
  Serial.print("Gas off time ");
  Serial.println(gas_off_time);
}
