# ArduinoProm
![Build](https://github.com/Ryzee119/ArduinoProm/workflows/Build/badge.svg)  
ArduinoProm. An Arduino based Original Xbox EEPROM reader and writer.
It can be used to recover a HDD key etc. <br><br>
ArduinoProm is inspired and adapted upon the awesome work by Grimdoomer on PiPROM, now achievable on a <$3 Arduino board.
See https://github.com/grimdoomer/PiPROM for the Original Raspberry Pi version!
  
**Use at your own risk**

## Wiring
1. Connect `GND, SDA and SCL` on the Xbox LPC port to the corresponding pins on the Arduino module.
![wiring](https://i.imgur.com/No7bvLF.png)

![installed photo](https://i.imgur.com/fokwQjY.jpg)
## Arduino Setup
The firmware has been written around the Arduino Pro Micro Leonardo (5V/16Mhz). However I would expect it to work on any Arduino with a built in USB bootloader/Virtual Comport support, and obviously I2C support.
1. Open `ArduinoProm.ino` in [Arduino IDE](https://www.arduino.cc/en/main/software)
2. Connect your Arduino to your PC and setup the IDE. An example below <br> ![Arduino IDE preview](https://i.imgur.com/V7CJpkd.png)
3. Hit the program button then confirm it compiles and programs successfully.


## Python Script
ArduinoProm accepts a few very basic commands over a virtual comport interface.
```
0x00 triggers an EEPROM read.
0x01 triggers an EEPROM write.
0x02 will erase the Xbox eeprom.
0x03 will return 0x00 if the eeprom is detected. (-1 otherwise)
```
To facilitate these commands in a more user friendly way you can use a small python app `ArduinoProm.py`.

To run the Python program you need to install [Python 3.7](https://www.python.org). Once installed and setup do  
```
pip install pyserial
pip install wxPython
```

`ArduinoProm.py` usage is as follows (Determine your comport number by plugging in the Arduino to your PC after programming it)
```
//Read the EEPROM on COM1 and save to file to eeprom.bin
python ArduinoProm.py COM1 READ eeprom.bin

//Write eeprom.bin to the EEPROM on COM1.
python ArduinoProm.py COM1 WRITE eeprom.bin

//Write all 0's to the EEPROM effectively erasing it.
python ArduinoProm.py COM1 ERASE
```
Example outputs:  
![Write](https://i.imgur.com/mRRVvAb.png)

![Read](https://i.imgur.com/p308Va2.png)

![Erase](https://i.imgur.com/r6GWkpm.png)

By Ryzee119
