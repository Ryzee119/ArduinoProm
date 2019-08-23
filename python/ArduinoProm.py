
"""
https://github.com/Ryzee119/ArduinoProm

Small Python application to interface with ArduinoProm. An Arduino based Original Xbox EEPROM
reader and writer. Can be used to recover from a corrupt BIOS or recover HDD key etc.

ArduinoProm is inpired and based upon the awesome work by Grimdoomer on PiPROM.
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
	
	ArduinoProm accepts a few very basic commands over a virtual comport interface.
	0x00 triggers an eeprom read. ArduinoProm will send the contents of the EEPROM back (256 byte array)
	0x01 is an eeprom write. This command should be followed with 256bytes of eeprom data.
	0x02 will erase the Xbox eeprom (write 0x00 to all addresses)
	0x03 will return 0x00 if the eeprom is detected.
	
	ArduinoProm returns -1 if an error has occured with any of these commands. It returns 0 if successful.
"""


import sys
import serial
import time
import binascii

def eeprom_read():
    ser.flushInput()
    ser.flushOutput()
    rx_data=bytes()
    ser.write(bytearray(b'\x00'))
    rx_data = ser.read(256)
    return rx_data

def eeprom_write(data):
    ser.flushInput()
    ser.flushOutput()
    rx_data=bytes()
    ser.write(bytearray(b'\x01'))
    ser.write(data)
    ser.flushOutput()
    time.sleep(.1)

    if ser.read(1)==b'\x00':
        print("Write sucessful")
    return eeprom_read()


def region_detect(argument):
    if argument   == b'\x00\x00\x00\x00':
        return "INVALID"
    elif argument == b'\x00\x01\x40\x00':
        return "NTSC-M"
    elif argument == b'\x00\x02\x40\x00':
        return "NTSC-J"
    elif argument == b'\x00\x03\x80\x00':
        return "PAL-I"
    elif  argument == b'\x00\x04\x40\x00':
        return "PAL-M"
    else:
        return "INVALID"
    
   


if len(sys.argv) > 1:
    comport=sys.argv[1]
    action=sys.argv[2]
    try:
        file=sys.argv[3]
    except:
        if action !="ERASE":
            print("You need to specificy a filename")


try:
    ser = serial.Serial(port=comport,
                    baudrate=9600,
                    timeout=5,
                    rtscts=1)
except:
    print("Could not open",comport)
    exit()


if action=="READ":
    print("Reading EEPROM via",comport)
    eeprom_data=eeprom_read();
    if len(eeprom_data) == 256:
        print("Saving EEPROM file to", file);
        eeprom_file = open(file, 'wb')
        eeprom_file.write(eeprom_data)

        #Print some basic info from EEPROM. I don't bother with decrypting HDD Key etc.
        print("Xbox Serial Number:",eeprom_data[0x34:0x40])

        
    else:
        print("Error Reading EEPROM. Check connections");


if action=="WRITE":
    print("Opening EEPROM",file)
    eeprom_file=open(file, 'rb')
    eeprom_data=eeprom_file.read();
    if len(eeprom_data) == 256:
        print("WARNING: This will rewrite the EEPROM ")
        input("Press Enter to Write EEPROM...")
        print("Writing EEPROM file via", comport);
        result=eeprom_write(eeprom_data)
        print("Verifying...")
        if eeprom_data == result:
            print("Verification Successful")
        else:
            print("Error writing eeprom")
    else:
        print("Error Reading EEPROM file. Check it's not corrupt and the filename is ok");


if action=="ERASE":
    print("WARNING: This will erase the EEPROM ")
    input("Press Enter to Erase EEPROM...")
    ser.write(bytearray(b'\x02'))
    ser.flushOutput()
    time.sleep(.1)

    if ser.read(1)==b'\x00':
        print("Erase successful.")
    else:
        print("Error erasing device. Check connections")
