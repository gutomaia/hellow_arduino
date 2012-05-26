ARDUINO_DIR = /Applications/Arduino.app/Contents/Resources/Java

BOARD_TAG    = uno
ARDUINO_PORT = /dev/cu.usb*

ARDUINO_LIBS = Ethernet Ethernet/utility SPI

PARSE_BOARD = TT
VARIANT = standard
MCU = atmega328p

F_CPU = 16000000L

AVRDUDE_ARD_PROGRAMMER = arduino
AVRDUDE_ARD_BAUDRATE = 115200

ISP_LOCK_FUSE_PRE  = 0x3F
ISP_LOCK_FUSE_POST = 0x0F
ISP_HIGH_FUSE      = 0xde
ISP_LOW_FUSE       = 0xff
ISP_EXT_FUSE       = 0x05


include Arduino.mk