
int second = 0;

void timeclock() {
  static unsigned long time = 0;
  if (millis() - time > 1000) {
    time = millis();
    second++;
    Serial.println(second);
  }
}


void gasSystemErrorCheck() {
  static float gassample = 0;
  static float gas_sample1 = 0;
  static float gas_sample2 = 0;
  static float gas_sample3 = 0;
  static float gasvalue = 15.00;
  static uint8_t checkGas = 0;
  static bool getsample1 = false;
  static bool getsample2 = false;
  static bool getsample3 = false;
  static unsigned long timer = 0;
  static unsigned long timer1 = 0;
  static uint8_t step = 0;


  switch (step) {
    case 0:
      if (!getsample1) {
        gas_sample1 = gasvalue;
        getsample1 = true;
        Serial.println(F("Got sample 1"));
      }
      if (millis() - timer < 4000) {
        return;
      }
      timer = millis();
      if (!getsample2) {
        gas_sample2 = gasvalue;
        getsample2 = true;
        Serial.println(F("Got sample 2"));
        timer1 = millis();
      }
      if (millis() - timer1 < 4000) {
        return;
      }
      timer1 = millis();
      if (!getsample3) {
        gas_sample3 = gasvalue;
        getsample3 = true;
        Serial.println(F("Got sample 3"));
        step = 1;
      }
      break;
    case 1:
      if (millis() - timer < 30000) {  // wait 30 seconds
        return;
      }
      timer = millis();
      Serial.println(F("Timer elapsed"));
      getsample1 = false;
      getsample2 = false;
      getsample3 = false;
      gassample = (gas_sample1 + gas_sample2 + gas_sample3) / 3;
      Serial.print(F("This is the average sample = "));
      Serial.println(gassample);
      step = 2;
      break;
    case 2:
      Serial.print("step is now =");
      Serial.println(step);
      if (gasvalue < gassample - .3) {
        Serial.print("Gas moving in the right direction");
        checkGas = 0;
        step = 0;
      } else {
        step = 3;
      }
      break;
    case 3:
      static float low = gassample - .1;
      static float high = gassample + .1;
      Serial.print(F("Low = "));
      Serial.println(low);
      Serial.print(F("gas value = "));
      Serial.println(gasvalue);
      Serial.print(F("High = "));
      Serial.println(high);
      if (low <= gasvalue && gasvalue <= high) {
        checkGas++;
        Serial.print(F("Nothing changed increse gas by 1 new value = "));
        Serial.println(checkGas);
      }
      if (checkGas == 4) {
        step = 4;
      } else {
        step = 0;
      }
      break;
    case 4:
      Serial.println(F("lock out active"));
      runaway_lockout = true;
      timer = millis();
      step = 5;
      break;
    case 5:
      timeclock();
      if (millis() - timer < 300000) {  // lockout for 5 mins
        return;
      }
      timer = millis();
      Serial.println(F("lock out deactiveate"));
      runaway_lockout = false;
      checkGas = 0;
      second = 0;
      step = 0;
      break;
  }
}
