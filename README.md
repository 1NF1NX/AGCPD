# AGCPD
Arduino Gesture Controlled pointing Device <br>
This program uses the MPU 9250, 9 axis IMU and an Arduino UNO R3 to take movement data and use it as a mouse. <br>

# Dependencies
 + <MPU9250_asukiaaa> library for arduino

# Wiring

          +----------------------+
          |      Arduino Uno     |
          |                      |
          |  [3.3V] ----+--------+---- VCC (MPU9250)
          |             |
          |  [GND] -------------- GND
          |                      |
          |  [A4] -------------- SDA
          |  [A5] -------------- SCL
          |                      |
          +----------------------+

                |
                |  MPU-9250 Sensor
                v
          +----------------------+
          |      MPU-9250        |
          |                      |
          |  VCC  <--- 3.3V      |
          |  GND  <--- GND       |
          |  SDA  <--- A4        |
          |  SCL  <--- A5        |
          |  AD0  --- GND  (I2C Addr = 0x68)
          |  NCS  --- 3.3V (Disable SPI)
          |  INT  (optional, for interrupts)
          +----------------------+


# Setup
- clone repo <br>
- make the circuit by manupulating the code <br>
- upload setup.ino <br>
- run AGCPD.cpp <br>

# How to use?
Tilting the sensor in desired direction moves the cursor, Left clicks are triggered by a single quick downwands jerk, Right clicks are triggered by a single quick upwards jerk.<br>
