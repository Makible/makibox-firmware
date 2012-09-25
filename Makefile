

MCU=at90usb1286
F_CPU=16000000

BSP_OBJS       = pins_teensy.o usb.o WInterrupts.o wiring.o \
                 main.o new.o usb_api.o HardwareSerial.o Print.o \
                 Stream.o Tone.o WMath.o WString.o

MAKIBOX_OBJS   = arc_func.o heater.o makibox.o store_eeprom.o \
                 Sd2Card.o SdFile.o SdVolume.o


CC=avr-gcc
CXX=avr-g++
OBJCOPY=avr-objcopy
CPPFLAGS       = -mmcu=$(MCU) -Ibsp/ -DF_CPU=$(F_CPU)L -Os -Wall \
                 -ffunction-sections -fdata-sections
CFLAGS         = -std=gnu99
CXXFLAGS       = -fno-exceptions
LDFLAGS        = -mmcu=$(MCU) -Wl,--gc-sections -Os


%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

BSP_OBJS:=$(addprefix bsp/, $(BSP_OBJS))
MAKIBOX_OBJS:=$(addprefix src/, $(MAKIBOX_OBJS))


all: makibox.hex

clean:
	rm -f $(BSP_OBJS)
	rm -f $(MAKIBOX_OBJS)

bsp: $(BSP_OBJS)
makibox: $(MAKIBOX_OBJS)

makibox.elf: $(BSP_OBJS) $(MAKIBOX_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ -lc -lm

makibox.hex: makibox.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
