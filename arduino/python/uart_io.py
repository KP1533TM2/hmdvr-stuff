#!/bin/python
import serial
import struct
import time

class VideoDecoderIC:
    types = {
        0x5150: "Texas Instruments TVP5150AM1",
        0x7150: "Chengdu Zhenxin Technology Co., Ltd GM7150BN/BC"
    }
    port = None
    type = None
    patch_ver = None

    def __init__(self, portID):
        self.port = serial.Serial(portID)
        print("Opened serial port {0}".format(self.port.name))
        self.port.setDTR(True)
        time.sleep(1)
        self.port.setDTR(False)
        self.probe()
        
    def __del__(self):
        print("Closing serial port {0}".format(self.port.name))
        self.port.close();     

    def probe(self):
        # Find out what kind of chip we're using
        chipID  = struct.unpack('>h', self.i2c_read(0x80,2))
        patch_ver = self.i2c_read(0x82,2)+self.i2c_read(0x33,1)
        if chipID[0] in self.types:
            print("Chip found\t: {0}".format(self.types[chipID[0]]))
        else:
            print("Chip is either unknown, something's wrong or shit happened")
        self.chipID = chipID[0]
        print("Patch version\t: {0:02x}.{1:02x}.{2:02x}".format(patch_ver[0], patch_ver[1], patch_ver[2]))
        self.patch_ver = patch_ver

    def i2c_read(self, addr, len):
        self.port.write(bytes([0x72, addr, len, 0x0a]))
        return self.port.read(len)
        
    def i2c_write(self, addr, bytes, increment = True):
        msg = [0x77, addr, len(bytes), 0xff if increment else 0x00]+bytes+[0x0a]
        self.port.write(msg)
        return self.port.read(1) # U
        
        
              
x = VideoDecoderIC('/dev/ttyACM0')
#x.i2c_write(0x18, [0x02])
#x.i2c_write(0x19, [0x00])

while True:
    print("{0:08b}".format(x.i2c_read(0x8A, 1)[0]))


