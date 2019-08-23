/*
https://github.com/Ryzee119/ArduinoProm

ArduinoProm. An Arduino based Original Xbox EEPROM reader and writer.
It can be used to recover from a corrupt BIOS or recover HDD key etc.

ArduinoProm is inpired and adapted upon the awesome work by Grimdoomer on PiPROM.
https://github.com/grimdoomer/PiPROM


This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
	

*/

#define XBOX_EEPROM_ADDRESS  0x54
#define XBOX_EEPROM_SIZE    256

#define EEPROM_READ 0
#define EEPROM_WRITE 1
#define EEPROM_ERASE 2
#define EEPROM_DETECT 3

#include <Wire.h>

char pbEEPROM[XBOX_EEPROM_SIZE];
char incomingCommand = 0;
int returnStatus = -1;


void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  returnStatus = -1;
  
  if (Serial.available()) {
    incomingCommand = Serial.read();
    switch (incomingCommand) {
      case EEPROM_READ:
        returnStatus = XboxI2C_ReadEEPROM(XBOX_EEPROM_ADDRESS, pbEEPROM);
        if (returnStatus != -1) {
          returnStatus = Serial.write(pbEEPROM, XBOX_EEPROM_SIZE);
        }
        break;

      case EEPROM_WRITE:
        if (Serial.readBytes(pbEEPROM, XBOX_EEPROM_SIZE) == XBOX_EEPROM_SIZE) {
          returnStatus = XboxI2C_WriteEEPROM(XBOX_EEPROM_ADDRESS, pbEEPROM);
        }
        break;

      case EEPROM_ERASE:
        memset(pbEEPROM, 0, XBOX_EEPROM_SIZE);
        returnStatus = XboxI2C_WriteEEPROM(XBOX_EEPROM_ADDRESS, pbEEPROM);
        break;

      case EEPROM_DETECT:
        returnStatus = XboxI2C_DetectEEPROM(XBOX_EEPROM_ADDRESS);
        break;

      default:
        returnStatus = -1;
        break;

    }
    Serial.write(returnStatus);
    Serial.flush();
  }
}

//Read the EEPROM
//bAddress: I2C address of EEPROM
//pbBuffer: Pointer the receiver buffer (256 bytes minimum)
//Returns: -1 on error, 0 on success.
int XboxI2C_ReadEEPROM(char bAddress, char *pbBuffer)
{

  //Some input sanity
  if (bAddress < 0 || bAddress > 128)
    return -1;
  if (pbBuffer == 0)
    return -1;

  memset(pbBuffer, 0, XBOX_EEPROM_SIZE);

  // Read the EEPROM buffer from the chip
  int add = 0;
  Wire.beginTransmission(bAddress);
  Wire.write(add);
  if (Wire.endTransmission(false) == 0) {
    while (add < XBOX_EEPROM_SIZE) {
      Wire.requestFrom(bAddress, 1);
      if (Wire.available()) {
        pbBuffer[add] = Wire.read();
      } else {
        return -1;
      }
      add++;
    }
  } else {
    return -1;
  }

  // Successfully read the EEPROM chip.
  return 0;
}


//Write the EEPROM
//bAddress: I2C address of EEPROM
//pbBuffer: Pointer the transmit data buffer (256 bytes minimum)
//Returns: -1 on error, 0 on success.
int XboxI2C_WriteEEPROM(char bAddress, char *pbBuffer)
{
  char commandBuffer[2];
  int i;

  //Some input sanity
  if (bAddress < 0 || bAddress > 128)
    return -1;
  if (pbBuffer == 0)
    return -1;

  //Loop through the buffer to write.
  for (i = 0; i < XBOX_EEPROM_SIZE; i++)
  {
    Wire.beginTransmission(bAddress);
    // Set the target address and data for the current byte.
    commandBuffer[0] = (char)i;
    commandBuffer[1] = pbBuffer[i];

    // Write the data to the chip.
    Wire.write(commandBuffer, 2);

    if (Wire.endTransmission() != 0) {
      return -1;
    }

    //Wait before writing the next byte.
    delay(10);
  }

  //Successfully wrote the buffer to the EEPROM chip.
  return 0;
}

//Requests an ACK from the EEPROM to detect if it's present
//bAddress: I2C address of EEPROM
//Returns: -1 on error, 0 on success.
int XboxI2C_DetectEEPROM(char bAddress)
{
  Wire.beginTransmission(bAddress);
  if (Wire.endTransmission(true) != 0) {
    return -1;
  }

  //I2C device at specified address detected
  return 0;
}
