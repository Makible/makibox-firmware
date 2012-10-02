

void serial_init();


// Serial output with printf()-style string formatting.  The version that works
// with format strings in RAM is straightforward;  we add the GCC format(...)
// attribute to the declaration so that we get helpful compiler warnings when
// the format string doesn't match the parameters.
void serial_printf(const char *fmt, ...) 
    __attribute__(( format(gnu_printf, 1, 2) ));


// But the one that works with format strings in flash memory is...  tricky.  It
// takes some amount of coaxing to get GCC to type-check format string parameters
// without also placing the format string into RAM.  The __dummy_printf() inline
// function seems to do the trick.
void inline __dummy_printf(const char *fmt, ...)
    __attribute__(( format(gnu_printf, 1, 2) ));

void inline __dummy_printf(const char *fmt, ...)
{
    return;
}


// Note that this macro *EVALUATES ITS ARGUMENTS TWICE*.  This is (sadly) an
// unavoidable consequence of needing to call __dummy_printf().  However, if you
// are doing silly stuff such as:
//     serial_send("i=%d\n", i++);
// then you quite honestly deserve what you get.
void __serial_printf_P(PGM_P fmt, ...);
#define serial_send(fmt, args...)                \
    do {                                            \
        static const char __c[] PROGMEM = (fmt);    \
        __dummy_printf((fmt), ##args);                  \
        __serial_printf_P(__c, ##args);         \
    } while(0)
