// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// KMX61
// This code is designed to work with the KMX61_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;

public class KMX61
{
	public static void main(String args[]) throws Exception
	{
		// Create I2CBus
		I2CBus bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, KMX61 I2C address is 0x0E(14)
		I2CDevice device = bus.getDevice(0x0E);

		// Select Stand-by register, 0x29(41)
		// Accelerometer, Magnetometer operating mode
		device.write(0x29 ,(byte)0x00);
		// Select control register1, 0x2A(42)
		// Range = +/-2g, 12-bit resolution
		device.write(0x2A ,(byte)0x10);
		// Select Output data control register, 0x2C(44)
		// ODR = 12.5 Hz
		device.write(0x2C ,(byte)0x00);
		Thread.sleep(500);

		// Read 6 bytes of data
		// xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
		byte[] data = new byte[6];
		data[0] = (byte)device.read(0x0A);
		data[1] = (byte)device.read(0x0B);
		data[2] = (byte)device.read(0x0C);
		data[3] = (byte)device.read(0x0D);
		data[4] = (byte)device.read(0x0E);
		data[5] = (byte)device.read(0x0F);

		// Convert the data to 12-bits
		int xAccl = ((data[1] & 0xFF) * 256 + (data[0] & 0xF0)) / 16;
		if(xAccl > 2047)
		{
			xAccl -= 4096;
		}	
		int yAccl = ((data[3] & 0xFF) * 256 + (data[2] & 0xF0)) / 16;
		if(yAccl > 2047)
		{
			yAccl -= 4096;
		}
		int zAccl = ((data[5] & 0xFF) * 256 + (data[4] & 0xF0)) / 16;
		if(zAccl > 2047)
		{
			zAccl -= 4096;
		}

		// Read 6 bytes of data
		// xMag msb, xMag lsb, zMag msb, zMag lsb, yMag msb, yMag lsb
		data[0] = (byte)device.read(0x12);
		data[1] = (byte)device.read(0x13);
		data[2] = (byte)device.read(0x14);
		data[3] = (byte)device.read(0x15);
		data[4] = (byte)device.read(0x16);
		data[5] = (byte)device.read(0x17);

		// Convert the data
		int xMag = ((data[1] & 0xFF) * 256 + (data[0] & 0xFC)) / 4;
		if(xMag > 8191)
		{
			xMag -= 16384;
		}	
		int yMag = ((data[3] & 0xFF) * 256 + (data[2] & 0xFC)) / 4;
		if(yMag > 8191)
		{
			yMag -= 16384;
		}
		int zMag = ((data[5] & 0xFF) * 256 + (data[4] & 0xFC)) / 4;
		if(zMag > 8191)
		{
			zMag -= 16384;
		}

		// Output data to screen
		System.out.printf("Acceleration in X-Axis : %d %n", xAccl);
		System.out.printf("Acceleration in Y-Axis : %d %n", yAccl);
		System.out.printf("Acceleration in Z-Axis : %d %n", zAccl);
		System.out.printf("Magnetic field in X-Axis : %d %n", xMag);
		System.out.printf("Magnetic field in Y-Axis : %d %n", yMag);
		System.out.printf("Magnetic field in Z-Axis : %d %n", zMag);
	}
}