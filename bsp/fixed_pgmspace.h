#ifndef FIXED_PGMSPACE
#define FIXED_PGMSPACE

#include <avr/pgmspace.h>

// Silence bogus warnings in avr-gcc < 4.6.3
#undef PROGMEM
#define PROGMEM __attribute__(( section(".progmem.data") ))
#undef PSTR
#define PSTR(s) (__extension__({static const char __c[] PROGMEM = (s); &__c[0];}))

#endif
