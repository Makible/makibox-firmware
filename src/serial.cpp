

#include "fixed_pgmspace.h"
#include <stdarg.h>
#include <stdio.h>

#include "Configuration.h"
#include "usb_api.h"


void serial_init()
{
    Serial.begin(BAUDRATE);
}


void serial_printf(const char *fmt, ...)
{
    char buf[128];
    int size;
    va_list args;

    va_start(args, fmt);
    size = vsnprintf(buf, 128, fmt, args);
    va_end(args);
    if (size < 128)
    {
        // TODO:  error handling when exceeding the 128-byte buffer
        Serial.write((uint8_t *)buf, size);
    }
}


void __serial_printf_P(PGM_P fmt, ...)
{
    char buf[128];
    int size;
    va_list args;

    va_start(args, fmt);
    size = vsnprintf_P(buf, 128, fmt, args);
    va_end(args);
    if (size < 128)
    {
        // TODO:  error handling when exceeding the 128-byte buffer
        Serial.write((uint8_t *)buf, size);
    }
}