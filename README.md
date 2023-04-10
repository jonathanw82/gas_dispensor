<div align="center">
   <img src="https://github.com/jonathanw82/gas_dispensor/blob/main/media/.jpg" alt="mist image"/>
 </div>

# Overview
This experiment proves that aeroponics could be less susceptible to [Pythium](https://en.wikipedia.org/wiki/Pythium), a significant issue for spinach in hydroponic greenhouse production. This is to confirm that it is possibly due to increased oxygenation with an aeroponic system.

The gas dispenser is to reduce the oxygen level in the root zone of an aeroponic grow bed by adding nitrogen to bring down the oxygen percentage.

As can be seen in Fig 1 on average breathable air is made up of oxygen 21%, nitrogen 78.1% and argon and other gasses make only 0.9% of that carbon dioxide makes up 0.04%

Therefore increasing the nitrogen level will decrease the oxygen level. Over a period of time, this can prove if aeroponics does indeed give the root zone more assess to oxygen and decrease the susceptibility to pythium.

### Fig 1
<div align="center">
   <img src="https://github.com/jonathanw82/gas_dispensor/blob/main/media/piechart.jpg" alt="piechart"/>
 </div>


## Hardware:

* 1 x Arduino Uno wifi rev2
* 1 x 5v relay
* 1 x 5v PSU
* 1 x Gravity: Electrochemical Oxygen / O2 Sensor (0-100%Vol, I2C) [Here](https://www.dfrobot.com/product-2569.html) (Main 02 sensor) 
* 1 x Gravity: O2 Sensor (Calibrated, I2C & UART) [Here](https://thepihut.com/products/gravity-o2-sensor-calibrated-i2c-uart?variant=41620114866371&currency=GBP&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&gclid=CjwKCAjw586hBhBrEiwAQYEnHaFBwm7ZAKjgB-vlygECEoYnv8AqbQjYx805CCJuayE0CSrMo6SIVhoCAVIQAvD_BwE) (Safety sensor)
* 1 x Project box
* 1 x Cyclinder of nitrogen
* 1 x Bottle regulator with solenoid [Here](https://www.onestopgrowshop.co.uk/pro-leaf-co2-regulator.html)

## Process:
The safety sensor will read the ambient oxygen level in the room if the level of oxygen is within safe levels. Under normal atmospheric pressure conditions, a human normally inhales air that contains 20.9% oxygen; if this falls even by 1 or 2% then it starts to become more laborious and the environment turns hypoxic meaning that oxygen levels are low and could be harmful. [More here](#safety)

If ambiant levels are safe a O2 sensor in the grow bed feed the data back to a PID algorithm, from here it will decide on the next course of action. For instance, if the oxygen level is higher than the set point, the controller will turn on the solenoid valve for time of a percentage of a time period, this allows the gases to pass up through the grow bed and reach the sensor. For now lets say a period of 30 seconds this could be 10 seconds or whatever length of time you want to tune the PID algorithm to, after the 30 seconds have elapsed it will turn on the solenoid again for a percentage of the time, depending on how far or close to the set point the oxygen level is the PID controller will adjust the percentage of time accordingly to attempt to reach the setpoint and maintain it without under or overshooting it.

## Safety:

Two 02 sensors are used, one in the root zone and another in the room. Unsafe oxygen levels are below 19.50% if this is detected the solonoid will be shut down to prevent anymore nitrogen being pumped into the envirment. at this point the only way for the system to start back up it to be phsically reset
oxygen alarms

If the oxygen in the room falls below 19.5% the alarm will sound alerty the staff of unsafe working conditions and to exit the room immediatly.
## Notes:

[Composition of air](https://en.wikipedia.org/wiki/Atmosphere_of_Earth)

<hr>
https://www.onestopgrowshop.co.uk/pro-leaf-co2-regulator.html





https://www.frontline-safety.co.uk/crowcon-clip-sgd-o2-19-5-23-5?gclid=Cj0KCQjwocShBhCOARIsAFVYq0gQbVtchKTyPnhoyY-E2DVQ6XILTxL77i3xV6OiKG-mPj-r-w2bqaUaArd8EALw_wcB

## refernce notes
https://peoplesafe.co.uk/blogs/safety-when-working-in-low-oxygen-environments/