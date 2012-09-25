

MCU=at90usb1286
F_CPU=16000000

CC=avr-gcc
CXX=avr-g++


BSP_SRC        = $(wildcard bsp/*.cpp bsp/*.c)
MAKIBOX_SRC    = $(wildcard src/*.cpp)

BSP_OBJS       = malloc.o pins_teensy.o usb.o WInterrupts.o wiring.o
MAKIBOX_OBJS   = arc_func.o heater.o makibox.o store_eeprom.o \
                 Sd2Card.o SdFile.o SdVolume.o


CFLAGS=-mmcu=$(MCU) -Ibsp/ -DF_CPU=$(F_CPU)L -Os -Wall \
       -ffunction-sections -fdata-sections
CXXFLAGS=$(CFLAGS)


%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


#build/bsp/%.o: bsp/%.c
#	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
#build/bsp/%.o: bsp/%.cpp
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
#build/%.o: src/%.cpp
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

all: bsp makibox

clean:
	rm $(addprefix bsp/, $(BSP_OBJS))
	rm $(addprefix src/, $(MAKIBOX_OBJS))

bsp: $(addprefix bsp/, $(BSP_OBJS))
makibox: $(addprefix src/, $(MAKIBOX_OBJS))

