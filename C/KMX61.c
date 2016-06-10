// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// KMX61
// This code is designed to work with the KMX61_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, KMX61 I2C address is 0x0E(14)
	ioctl(file, I2C_SLAVE, 0x0E);

	// Select Stand-by register(0x29)
	// Accelerometer, Magnetometer operating mode(0x00)
	char config[2] = {0};
	config[0] = 0x29;
	config[1] = 0x00;
	write(file, config, 2);
	// Select control register1(0x2A)
	// Range = +/-2g, 12-bit resolution(0x00)
	config[0] = 0x29;
	config[1] = 0x10;
	write(file, config, 2);
	// Select Output data control register(0x2C)
	// ODR = 12.5 Hz(0x00)
	config[0] = 0x2C;
	config[1] = 0x00;
	write(file, config, 2);
	sleep(1);

	// Read 6 bytes of data
	// lsb first
	// Read xAccl lsb data from register(0x0A)
	char reg[1] = {0x0A};
	write(file, reg, 1);
	char data[1] = {0};
	if(read(file, data, 1) != 1)
	{
		printf("Erorr : Input/output Erorr \n");
		exit(1);
	}
	char data_0 = data[0];

	// Read xAccl msb data from register(0x0B)
	reg[0] = 0x0B;
	write(file, reg, 1);
	read(file, data, 1);
	char data_1 = data[0];

	// Read yAccl lsb data from register(0x0C)
	reg[0] = 0x0C;
	write(file, reg, 1);
	read(file, data, 1);
	char data_2 = data[0];

	// Read yAccl msb data from register(0x0D)
	reg[0] = 0x0D;
	write(file, reg, 1);
	read(file, data, 1);
	char data_3 = data[0];

	// Read zAccl lsb data from register(0x0E)
	reg[0] = 0x0E;
	write(file, reg, 1);
	read(file, data, 1);
	char data_4 = data[0];

	// Read zAccl msb data from register(0x0F)
	reg[0] = 0x0F;
	write(file, reg, 1);
	read(file, data, 1);
	char data_5 = data[0];

	// Convert the data
	int xAccl = (data_1 * 256 + (data_0 & 0xF0));
	if(xAccl > 2047)
	{
		xAccl -= 4096;
	}

	int yAccl = (data_3 * 256 + (data_2 & 0xF0));
	if(yAccl > 2047)
	{
		yAccl -= 4096;
	}

	int zAccl = (data_5 * 256 + (data_4 & 0xF0));
	if(zAccl > 2047)
	{
	zAccl -= 4096;
	}

	// Output data to screen
	printf("Rotation in X-Axis : %d \n", xAccl);
	printf("Rotation in Y-Axis : %d \n", yAccl);
	printf("Rotation in Z-Axis : %d \n", zAccl);

	// Read 6 bytes of data
	// msb first
	// Read xMag msb data from register(0x12)
	reg[0] = 0x12;
	write(file, reg, 1);
	read(file, data, 1);
	char data1_0 = data[0];

	// Read xMag lsb data from register(0x13)
	reg[0] = 0x13;
	write(file, reg, 1);
	read(file, data, 1);
	char data1_1 = data[0];

	// Read yMag msb data from register(0x14)
	reg[0] = 0x14;
	write(file, reg, 1);
	read(file, data, 1);
	char data1_2 = data[0];

	// Read yMag lsb data from register(0x15)
	reg[0] = 0x15;
	write(file, reg, 1);
	read(file, data, 1);
	char data1_3 = data[0];

	// Read zMag msb data from register(0x16)
	reg[0] = 0x16;
	write(file, reg, 1);
	read(file, data, 1);
	char data1_4 = data[0];

	// Read zMag lsb data from register(0x17)
	reg[0] = 0x17;
	write(file, reg, 1);
	read(file, data, 1);
	char data1_5 = data[0];

	// Convert the data
	int xMag = (data1_1 * 256 + (data1_0 & 0xFC));
	if(xMag > 8191)
	{
		xMag -= 16384;
	}	

	int yMag = (data1_3 * 256 + (data1_2 & 0xFC)) ;
	if(yMag > 8191)
	{
		yMag -= 16384;
	}

	int zMag = (data1_5 * 256 + (data1_4 & 0xFC)) ;
	if(zMag > 8191)
	{
		zMag -= 16384;
	}

	// Output data to screen
	printf("Magnetic field in X-Axis : %d \n", xMag);
	printf("Magnetic field in Y-Axis : %d \n", yMag);
	printf("Magnetic field in Z-Axis : %d \n", zMag);
}
