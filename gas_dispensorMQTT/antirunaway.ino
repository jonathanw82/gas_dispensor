
int second = 0;

void timeclock() {
  static unsigned long time = 0;
  if (millis() - time > 1000) {
    time = millis();
    second++;
    Serial.println(second);
  }
}

void set_runaway_lockout() {
  /*
   Check if the runaway lockout is true if not set it true and start the timer
  */
  if (runaway_lockout) {
    return;
  }
  Serial.println(F("Setting runaaway lockout"));
  runaway_lockout = true;
  runaway_lockout_timer = millis();
}

void manage_runaway_lockout() {
  /*
   If the runaway lockout is true check the timer if it elaspes set runaway lockout to false
  */
  if (!runaway_lockout) {
    return;
  }
  if (millis() - runaway_lockout_timer > runaway_lockout_duration_ms) {
    Serial.println(F("Unsetting runaaway lockout"));
    runaway_lockout = false;
  }
}

void gasSystemRunawayStateCheck(bool reset) {
  static float gassample_average = 0;
  static float gas_sample1 = 0;
  static float gas_sample2 = 0;
  static float gas_sample3 = 0;
  static uint8_t checkGas = 0;
  static bool getsample1 = true;
  static bool getsample2 = true;
  static bool getsample3 = true;
  static unsigned long timer = 0;
  static unsigned long timer1 = 0;
  static uint8_t step = 0;

  manage_runaway_lockout();

  if (reset) {
    //
    getsample1 = true;
    getsample2 = true;
    getsample3 = true;
    checkGas = 0;
    step = 0;
    return;
  }

  switch (step) {
    case 0:
      if (getsample1) {
        gas_sample1 = get_bed_oxygen_reading();
        getsample1 = false;
        Serial.println(F("Got sample 1"));
        timer = millis();
        wdt_reset();
      }
      if (millis() - timer < 5000) {
        return;
      }
      timer = millis();
      if (getsample2) {
        gas_sample2 = get_bed_oxygen_reading();
        getsample2 = false;
        Serial.println(F("Got sample 2"));
        timer1 = millis();
        wdt_reset();
      }
      if (millis() - timer1 < 5000) {
        return;
      }
      timer1 = millis();
      if (getsample3) {
        gas_sample3 = get_bed_oxygen_reading();
        getsample3 = false;
        Serial.println(F("Got sample 3"));
        step = 1;
        wdt_reset();
      }
      break;
    case 1:
      if (millis() - timer < 5000) {  // wait 30 seconds
        return;
      }
      timer = millis();
      Serial.println(F("Timer elapsed"));
      getsample1 = false;
      getsample2 = false;
      getsample3 = false;
      gassample_average = (gas_sample1 + gas_sample2 + gas_sample3) / 3;
      Serial.print(F("This is the average sample = "));
      Serial.println(gassample_average);
      step = 2;
      wdt_reset();
      break;
    case 2:
      Serial.print("step is now =");
      Serial.println(step);
      if (get_bed_oxygen_reading() < gassample_average - .3) {
        wdt_reset();
        Serial.print("Gas moving in the right direction");
        checkGas = 0;
        step = 0;
      } else {
        step = 3;
      }
      break;
    case 3:
      static float low = gassample_average - .1;
      static float high = gassample_average + .1;
      Serial.print(F("Low = "));
      Serial.println(low);
      Serial.print(F("gas value = "));
      Serial.println(get_bed_oxygen_reading());
      Serial.print(F("High = "));
      Serial.println(high);
      if (low <= get_bed_oxygen_reading() && get_bed_oxygen_reading() <= high) {
        checkGas++;
        Serial.print(F("Nothing changed increse gas by 1 new value = "));
        Serial.println(checkGas);
        wdt_reset();
      }
      if (checkGas == 4) {
        // here set runaway lockout
        set_runaway_lockout();
        checkGas = 0;
      }
      getsample1 = true;
      getsample2 = true;
      getsample3 = true;
      step = 0;
      break;
  }
}