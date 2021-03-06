

#include <bsp/pgmspace.h>
#include <stdarg.h>
#include <stdio.h>

#include <bsp/usb_api.h>


/* This is a mostly-useless definition, since we're using USB serial... */
#define BAUDRATE 57600


extern "C" {


void serial_init()
{
    Serial.begin(BAUDRATE);
}


int serial_can_read()
{
    return Serial.available() > 0;
}


uint8_t serial_read()
{
    int ch = Serial.read();
    if (ch < 0 || ch > 255)
    {
        // TODO:  error handling, somehow?
        return 0;
    }
    return (uint8_t)ch;
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


}  // extern "C"
