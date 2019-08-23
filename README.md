

# ArduinoProm

ArduinoProm. An Arduino based Original Xbox EEPROM reader and writer.
It can be used to recover from a corrupt BIOS or recover a HDD key etc. <br>
ArduinoProm is inspired and adapted upon the awesome work by Grimdoomer on PiPROM, now achievable on a <$3 Arduino board.
See https://github.com/grimdoomer/PiPROM for the Original Raspberry Pi version!

**Use at your own risk**

## Wiring
1. Connect `GND, SDA and SCL` on the Xbox LPC port to the corresponding pins on the Arduino module.
![enter image description here](https://i.imgur.com/No7bvLF.png)

![enter image description here](https://i.imgur.com/fokwQjY.jpg)
## Arduino Setup
The firmware has been written around the Arduino Pro Micro Leonardo (5V/16Mhz). However I would expect it to work on any Arduino with a built in USB bootloader/Virtual Comport support, and obviously I2C support.
1. Open `ArduinoProm.ino` in [Arduino IDE](https://www.arduino.cc/en/main/software)
2. Connect your Arduino to your PC and setup the IDE. An example below <br> ![enter image description here](https://i.imgur.com/V7CJpkd.png)
3. Hit the program button then confirm it compiles and programs successfully.


## Python Script
ArduinoProm accepts a few very basic commands over a virtual comport interface.
	`0x00 triggers an EEPROM read. ArduinoProm will send the contents of the EEPROM back (256 byte array)`
	`0x01 triggers an EEPROM write. This command should be followed with 256bytes of EEPROM data.`
	`0x02 will erase the Xbox eeprom (write 0x00 to all addresses)`
	`0x03 will return 0x00 if the eeprom is detected. (-1 otherwise)`

To facilitate these commands in a more user friendly way you can use a small python app `ArduinoProm.py`.

To run the Python program you need to install [Python 3.7](https://www.python.org). Once installed and setup do
`pip install pyserial`
`pip install wxPython`

`ArduinoProm.py` usage is as follows (Determine your comport number by plugging in the Arduino to your PC after programming it)

`python ArduinoProm.py COM1 READ eeprom.bin` Read the EEPROM on COM1 and save to file `eeprom.bin`
`python ArduinoProm.py COM1 WRITE eeprom.bin` Write `eeprom.bin` to the EEPROM on COM1
`python ArduinoProm.py COM1 ERASE` Write all 0's to the EEPROM effectively erasing it.
<br>
Example outputs:<br>
![enter image description here](https://i.imgur.com/mRRVvAb.png)

![enter image description here](https://i.imgur.com/p308Va2.png)

![enter image description here](https://i.imgur.com/r6GWkpm.png)

## Issues
Arduino I2C library can hang if the Xbox is powered off or the I2C connections are incorrectly connected resulting in the software freezing.
In this instance check your connections to the xbox, make sure the xbox is powered on then disconnect/reconnect the Arduino from your PCB to power cycle it.

By Ryzee119
