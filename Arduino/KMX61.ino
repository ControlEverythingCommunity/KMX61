// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// KMX61
// This code is designed to work with the KMX61_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include<Wire.h>

// KMX61 I2C address is 0x0E(14)
#define Addr 0x0E

void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select Stand-by register
  Wire.write(0x29);
  // Accelerometer, Magnetometer operating mode
  Wire.write(0x00);
  // Stop I2C transmission
  Wire.endTransmission();

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select control register1
  Wire.write(0x2A);
  // Range = +/-2g, 12-bit resolution
  Wire.write(0x10);
  // Stop I2C transmission
  Wire.endTransmission();

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select Output data control register
  Wire.write(0x2C);
  // ODR = 12.5 Hz
  Wire.write(0x00);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  unsigned int data[6];

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x0A);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 6 bytes of data
  Wire.requestFrom(Addr, 6);

  // Read 6 bytes of data
  // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
  if (Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
  }

  // Convert the data to 12-bits
  int xAccl = ((data[1] & 0xFF) * 256 + (data[0] & 0xF0)) / 16;
  if (xAccl > 2047)
  {
    xAccl -= 4096;
  }
  int yAccl = ((data[3] & 0xFF) * 256 + (data[2] & 0xF0)) / 16;
  if (yAccl > 2047)
  {
    yAccl -= 4096;
  }
  int zAccl = ((data[5] & 0xFF) * 256 + (data[4] & 0xF0)) / 16;
  if (zAccl > 2047)
  {
    zAccl -= 4096;
  }

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x12);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 6 bytes of data
  Wire.requestFrom(Addr, 6);

  // Read 6 bytes of data
  // xMag lsb, xMag msb, yMag lsb, yMag msb, zMag lsb, zMag msb
  if (Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
  }

  // Convert the data to 14-bits
  int xMag = (data[1] * 256 + (data[0] & 0xFC)) / 4;
  if (xMag > 8191)
  {
    xMag -= 16384;
  }
  int yMag = (data[3] * 256 + (data[2] & 0xFC)) / 4;
  if (yMag > 8191)
  {
    yMag -= 16384;
  }
  int zMag = (data[5] * 256 + (data[4] & 0xFC)) / 4;
  if (zMag > 8191)
  {
    zMag -= 16384;
  }

  // Output data to serial monitor
  Serial.print("Acceleration in X-Axis : ");
  Serial.println(xAccl);
  Serial.print("Acceleration in Y-Axis : ");
  Serial.println(yAccl);
  Serial.print("Acceleration in Z-Axis : ");
  Serial.println(zAccl);
  Serial.print("Magnetic Field in X-Axis : ");
  Serial.println(xMag);
  Serial.print("Magnetic Field in Y-Axis : ");
  Serial.println(yMag);
  Serial.print("Magnetic Field in Z-Axis : ");
  Serial.println(zMag);
  delay(1000);
}
