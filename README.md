<div align="center">
   <img src="https://github.com/jonathanw82/gas_dispensor/blob/main/media/mist.jpg" alt="mist image"/>
 </div>

# Overview
This experiment is intended to prove that aeroponics could be less susceptible to [Pythium](https://en.wikipedia.org/wiki/Pythium) a type of root rot, a significant issue for spinach in hydroponic greenhouse production. This test is to confirm that it is possibly due to increased oxygenation with the root zone of the aeroponic system.

The gas dispenser is to reduce the oxygen level in the root zone of an aeroponic growbed by adding nitrogen to bring down the percentage of oxygen.

As can be seen in Fig 1 on average breathable air is made up of oxygen 21%, nitrogen 78.1% and argon and other gasses make only 0.9% of that carbon dioxide makes up 0.04%. 

More information on the composition of air can be found [here.](https://en.wikipedia.org/wiki/Atmosphere_of_Earth)

Therefore increasing the nitrogen level will decrease the oxygen level. Over the growth cycle of some crop this can prove if aeroponics does indeed give the root zone more assess to oxygen and decrease the susceptibility to pythium.

### Fig 1
<div align="center">
   <img src="https://github.com/jonathanw82/gas_dispensor/blob/main/media/piechart.jpg" alt="piechart"/>
 </div>


## Hardware:

* 1 x Arduino Uno wifi rev2
* 1 x Wemos D1 Mini
* 4 x 5v relays
* 1 x 5v PSU
* 1 x Gravity: Electrochemical Oxygen / O2 Sensor (0-100%Vol, I2C) [Here](https://www.dfrobot.com/product-2569.html) (Main 02 sensor) 
* 1 x Gravity: O2 Sensor (Calibrated, I2C & UART) [Here](https://thepihut.com/products/gravity-o2-sensor-calibrated-i2c-uart?variant=41620114866371&currency=GBP&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&gclid=CjwKCAjw586hBhBrEiwAQYEnHaFBwm7ZAKjgB-vlygECEoYnv8AqbQjYx805CCJuayE0CSrMo6SIVhoCAVIQAvD_BwE) (Safety sensor)
* 1 x Project box
* 5 x cable glands
* 1 x Cyclinder of nitrogen
* 1 x Bottle regulator with solenoid [Here](https://www.onestopgrowshop.co.uk/pro-leaf-co2-regulator.html)
* 1 x Nitrogen Regulator [Here](https://www.welduk.com/nitrogen-regulator-p64)
* 1 x 1/4 male to 3/8 female BSP adaptor [Here](https://www.air-compressorsdirect.co.uk/bsp-fittings/taper-1-4-male-to-3-8-female-bsp-adaptor)
* 1 x Warning beacon
* 3 x LED's, Red, Green and Amber

<br>

| Component               | Consumption        |
| :----------------       | :-----------       |
| Arduino Uno Wifi Rev2   | @ 5v 93 ma         |
| Weemos D1 mini          | @ 5v 70 ma         |
| Twin relay              | @ 5v 40 ma         |
| Single 30amp relay      | @ 5v 20 ma         |
| Oxygen sensor SEN0465   | @ 5v < 5 ma        |
| Oxygen sesnor AA428-210 | @ 5v < 6.5 ma      |
| Warning Beacon          | @ 12v 300 ma       |
| Gas Solenoid            | @ 240v 1 amp       |
| Green status LED        | @ 5v 60 ma         |
| RED status LED          | @ 5v 60 ma         |
| AMBER status LED        | @ 5v 60 ma         |
|                         |Total = 1,652.48    |

<br>

### Fig 2
<div align="center">
   <img src="https://github.com/jonathanw82/gas_dispensor/blob/main/media/newschymatic.jpg" alt="schymatic"/>
 </div>
<br>

## Process:
The safety sensor will read the ambient oxygen level in the room to ascertain if the level of oxygen is within a safe range. Under normal atmospheric pressure conditions, a human normally inhales air that contains 20.9% oxygen, if this falls even by 1 or 2% then it starts to become more laborious and the environment turns hypoxic meaning that oxygen levels are low and could be harmful. [More here](#safety)

If ambient levels are within a safe range the safety relay will engaiage allowing power throught the dosing relay to energise the solenoid. If 0.0 is reported by the room sensor this is classed as an error. A further 8 samples are taken if they all return 0.0 the error status is reported and the lockout is enguaged and the warning beacon will sound. If of course the during those 8 rwading a correct value is retured then the system will continue as normal.

If all is well and the saftey relay is enguaged the bed level oxygen sensor will provide the level of oxygen in the root zone and the solenoid activated to the specified on off period, lets say 2 seconds on 4 seconds off with a 1 minute pause period to allow the gas to settle and not over shoot the set point. this is also helps to mitigate against the chance of the solenoid from freeing open.This process is repeated  to allow more nitrogen into the root zone to bring down the level to the required target. Once the target is met, the system will stop doseing and remain domant until more nitrogen is required and the process starts again. All the while the safety sensor is continuouly checking the oxygen level in the room to keep a safe enviroment.
<br>

### Fig 3
<div align="center">
   <img src="https://github.com/jonathanw82/gas_dispensor/blob/main/media/flowchart2.jpg" alt="flowchart2"/>
 </div>
<br>

## Safety:

Two 02 sensors are used, one in the root zone and another in the room. Unsafe oxygen levels are below 19.50% if this is detected the solenoid will be shut down to prevent any more nitrogen from being pumped into the environment a sounder and strobe will also activate. At this point, the only way for the system to start back up is to be physically reset by means of a power cycle by the operator.

The operator will also ware a personal oxygen safety alarm Fig 5 so if the oxygen in the room does fall below 19.50% the alarm will sound alerting the operator of unsafe working conditions and to exit and vent the room immediately.

### Fig 5
<div align="center">
   <img src="https://github.com/jonathanw82/gas_dispensor/blob/main/media/crowcon-clip-sgd--o2jpg_1.jpg" alt="alarm"/>
 </div>

<br>

## Control commands

sensding this command resets both arduinos.
* gas_dispenser/sub/owner=JON/location=r1/reset

The oxygen target level can be adjusted by sending a number to this command.
* gas_dispenser/sub/owner=JON/location=r1/oxygen_target_level

The solenoid on time can be adjusted by sending a number in seconds.
* gas_dispenser/sub/owner=JON/location=r1/solenoid_on_time_sec

The solenoid off time can be adjusted by sending a number in seconds.
* gas_dispenser/sub/owner=JON/location=r1/solenoid_off_time_sec


* gas_dispenser/sub/owner=JON/location=r1/solenoid_cycles
* gas_dispenser/sub/owner=JON/location=r1/dispense_paused_period_sec

## Notes:

<hr>
https://www.onestopgrowshop.co.uk/pro-leaf-co2-regulator.html


https://www.frontline-safety.co.uk/crowcon-clip-sgd-o2-19-5-23-5?gclid=Cj0KCQjwocShBhCOARIsAFVYq0gQbVtchKTyPnhoyY-E2DVQ6XILTxL77i3xV6OiKG-mPj-r-w2bqaUaArd8EALw_wcB

## refernce notes
https://peoplesafe.co.uk/blogs/safety-when-working-in-low-oxygen-environments/

calibrating the red sensor
https://wiki.dfrobot.com/SKU_SEN0496_Gravity_Electrochemical_Oxygen_Sensor_0_100_I2C


## refernce notes for each oxygen sensor

Gravity: I2C Oxygen / O2 Sensor SEN0322 0~25%Vol [here](https://wiki.dfrobot.com/Gravity_I2C_Oxygen_Sensor_SKU_SEN0322)


This full-scale oxygen / O2 sensor employs the original AO2 CiTiceL® oxygen sensor 0~100%Vol [here](https://wiki.dfrobot.com/SKU_SEN0496_Gravity_Electrochemical_Oxygen_Sensor_0_100_I2C)



CGA320 bottle connection
