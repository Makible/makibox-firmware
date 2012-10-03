

MCU=at90usb1286
F_CPU=16000000

BSP_OBJS       = pins_teensy.o usb.o \
                 main.o usb_api.o HardwareSerial.o Print.o \
                 Stream.o WString.o

MAKIBOX_OBJS   = arc_func.o heater.o makibox.o serial.o store_eeprom.o


CC=avr-gcc
CXX=avr-g++
OBJCOPY=avr-objcopy


CPPFLAGS       = -mmcu=$(MCU) -I. -DF_CPU=$(F_CPU)L -Os \
                 -ffunction-sections -fdata-sections -g \
                 -Wall -Wformat=2 -Werror
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
	rm -f makibox.elf
	rm -f makibox.hex

bsp: $(BSP_OBJS)
makibox: $(MAKIBOX_OBJS)

makibox.elf: $(BSP_OBJS) $(MAKIBOX_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ -lc -lm
	@(printf "Program size:\n")
	@(printf "  .text  %8d bytes\n" 0x$$(readelf -S $@ | grep '\.text' | cut -c 58-63))
	@(printf "  .data  %8d bytes\n" 0x$$(readelf -S $@ | grep '\.data' | cut -c 58-63))
	@(printf "  .bss   %8d bytes\n" 0x$$(readelf -S $@ | grep '\.bss' | cut -c 58-63))

makibox.hex: makibox.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
