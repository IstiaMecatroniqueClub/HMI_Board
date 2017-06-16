#include <led.h>

Pin::Pin(volatile uint8_t * port, uint8_t pinmask) :
    port(port),
    ddr(port-1),
    pin(port-2),
    pinmask(pinmask)
{
}