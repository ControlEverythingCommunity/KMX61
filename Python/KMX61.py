# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# KMX61
# This code is designed to work with the KMX61_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/products

import smbus
import time

# Get I2C bus
bus = smbus.SMBus(1)

# KMX61 address, 0x0E(14)
# Select Stand-by register, 0x29(41)
#		0x00(00)	Accelerometer, Magnetometer operating mode
bus.write_byte_data(0x0E, 0x29, 0x00)
# KMX61 address, 0x0E(14)
# Select Control register1, 0x2A(42)
#		0x10(16)	Range = +/-2g, 12-bit resolution
bus.write_byte_data(0x0E, 0x2A, 0x10)
# KMX61 address, 0x0E(14)
# Select Output data control register, 0x2C(44)
#		0x00(00)	ODR = 12.5 Hz
bus.write_byte_data(0x0E, 0x2C, 0x00)

time.sleep(0.5)

# KMX61 address, 0x0E(14)
# Read data back from 0x0A(10), 6 bytes
# xAccl LSB, xAccl MSB, yAccl LSB, yAccl MSB, zAccl LSB, zAccl MSB
data = bus.read_i2c_block_data(0x0E, 0x0A, 6)

# Convert the data to 12-bits
xAccl = ((data[1] * 256) + (data[0] & 0xF0)) / 16
if xAccl > 2047 :
	xAccl -= 4096
yAccl = ((data[3] * 256) + (data[2] & 0xF0)) / 16
if yAccl > 2047 :
	yAccl -= 4096
zAccl = ((data[5] * 256) + (data[4] & 0xF0)) / 16
if zAccl > 2047 :
	zAccl -= 4096

# KMX61 address, 0x0E(14)
# Read data back from, 0x12(18), 6 bytes
# xMag LSB, xMag MSB, yMag LSB, yMag MSB, zMag LSB, zMag MSB
data = bus.read_i2c_block_data(0x0E, 0x12, 6)

# Convert the data to 14-bits
xMag = ((data[1] * 256) + (data[0] & 0xFC)) / 4
if xMag > 8191 :
	xMag -= 16384
yMag = ((data[3] * 256) + (data[2] & 0xFC)) / 4
if yMag > 8191 :
	yMag -= 16384
zMag = ((data[5] * 256) + (data[4] & 0xFC)) / 4
if zMag > 8191 :
	zMag -= 16384

# Output data to screen
print "Acceleration in X-Axis : %d" %xAccl
print "Acceleration in Y-Axis : %d" %yAccl
print "Acceleration in Z-Axis : %d" %zAccl
print "Magnetic field in X-Axis : %d" %xMag
print "Magnetic field in Y-Axis : %d" %yMag
print "Magnetic field in Z-Axis : %d" %zMag
