
#include <MPU9250_asukiaaa.h>
#include <Wire.h>

  MPU9250_asukiaaa mySensor;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();

  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();

  Serial.println("MPU-9250 initialized.");
}

void loop() {
  // put your main code here, to run repeatedly:
  mySensor.accelUpdate();
  mySensor.gyroUpdate();
  mySensor.magUpdate();

  //Serial.print("Accel (g): ");
  Serial.print(mySensor.accelX());
  Serial.print(",");
  Serial.print(mySensor.accelY());
  Serial.print(",");
  Serial.print(mySensor.accelZ());
  Serial.print(",");
  //Serial.print("Gyro (deg/s): ");
  Serial.print(mySensor.gyroX());
  Serial.print(",");
  Serial.print(mySensor.gyroY());
  Serial.print(",");
  Serial.println(mySensor.gyroZ());
  /*Serial.print(", ");
  //Serial.print("Mag (uT): ");
  Serial.print(mySensor.magX());
  Serial.print(", ");
  Serial.print(mySensor.magY());
  Serial.print(", ");
  Serial.println(mySensor.magZ());
  */
}
