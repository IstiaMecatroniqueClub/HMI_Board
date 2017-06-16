#include "output.h"

#include <avr/io.h>
#include <util/delay.h>


Output::Output(volatile uint8_t * port, uint8_t pinmask) :
    Pin(port, pinmask)
{
    // Set pin as output
    *(ddr) |= (1 << pinmask);
    setHigh();
}

void Output::setHigh() {
    *(port) |= (1 << pinmask);
}

void Output::setLow() {
    *(port) &= ~(1 << pinmask);
}

void Output::toggle() {
    *(pin) |= (1 << pinmask);
}
