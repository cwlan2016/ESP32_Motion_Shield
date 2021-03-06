/******************************************************************************
BME280_I2C_SPI.ino
BME280 on the ESP32 Thing

Marshall Taylor @ SparkFun Electronics
Original creation date: May 20, 2015
Modified: Nov 6, 2017
https://github.com/sparkfun/ESP32_Motion_Shield

This sketch configures a BME280 to produce comma separated values for use
in generating spreadsheet graphs.

It has been modified from the original BME280 example to demonstrate I2C and
SPI operation on the ESP32 Motion board.

Original source:
https://github.com/sparkfun/SparkFun_BME280_Arduino_Library

Resources:
Uses Wire.h for I2C operation
Uses SPI.h for SPI operation

Development environment specifics:
Arduino IDE 1.8.2

This code is released under the [MIT License](http://opensource.org/licenses/MIT).
Please review the LICENSE.md file included with this example. If you have any questions 
or concerns with licensing, please contact techsupport@sparkfun.com.
Distributed as-is; no warranty is given.
******************************************************************************/
#include <stdint.h>
#include "SparkFunBME280.h"

#include "Wire.h"
#include "SPI.h"

#define BME280_CS_PIN 17

//Global sensor object
BME280 mySensor;

unsigned int sampleNumber = 0; //For counting number of CSV rows

void setup()
{
	//***Driver settings********************************//
	//  commInterface can be I2C_MODE or SPI_MODE.
		
	//For I2C, enable the following and disable the SPI section.
	//I2CAddress can be 0x77(default) or 0x76.
	//mySensor.settings.commInterface = I2C_MODE;
	//mySensor.settings.I2CAddress = 0x77;	
	
	//For SPI enable the following and dissable the I2C section.
	//set chipSelectPin using arduino pin names.
	mySensor.settings.commInterface = SPI_MODE;
	mySensor.settings.chipSelectPin = BME280_CS_PIN;


	//***Operation settings*****************************//
	mySensor.settings.runMode = 3; //  3, Normal mode
	mySensor.settings.tStandby = 0; //  0, 0.5ms
	mySensor.settings.filter = 0; //  0, filter off
	//tempOverSample can be:
	//  0, skipped
	//  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
	mySensor.settings.tempOverSample = 1;
	//pressOverSample can be:
	//  0, skipped
	//  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
    mySensor.settings.pressOverSample = 1;
	//humidOverSample can be:
	//  0, skipped
	//  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
	mySensor.settings.humidOverSample = 1;
	
	Serial.begin(115200);
	Serial.print("Program Started\n");
	Serial.print("Starting BME280... result of .begin(): 0x");
	delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
	//Calling .begin() causes the settings to be loaded
	Serial.println(mySensor.begin(), HEX);

	//Build a first-row of column headers
	Serial.print("\n\n");
	Serial.print("Sample,");
	Serial.print("T(deg C),");
	Serial.print("T(deg F),");
	Serial.print("P(Pa),");
	Serial.print("Alt(m),");
	Serial.print("Alt(ft),");
	Serial.print("%RH");
	Serial.println("");
	
}

void loop()
{

	//Print each row in the loop
	//Start with temperature, as that data is needed for accurate compensation.
	//Reading the temperature updates the compensators of the other functions
	//in the background.
	Serial.print(sampleNumber);
	Serial.print(",");
	Serial.print(mySensor.readTempC(), 2);
	Serial.print(",");
	Serial.print(mySensor.readTempF(), 3);
	Serial.print(",");
	Serial.print(mySensor.readFloatPressure(), 0);
	Serial.print(",");
	Serial.print(mySensor.readFloatAltitudeMeters(), 3);
	Serial.print(",");
	Serial.print(mySensor.readFloatAltitudeFeet(), 3);
	Serial.print(",");
	Serial.print(mySensor.readFloatHumidity(), 0);
	Serial.println();
	
	sampleNumber++;
	
	delay(50);

}