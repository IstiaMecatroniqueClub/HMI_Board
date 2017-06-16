#include "config.h"
#include "led.h"
#include <avr/io.h>
#include <util/delay.h>


LED::LED(volatile uint8_t * port, uint8_t pinmask, uint8_t polarity) :
    Output(port, pinmask),
    polarity(polarity)
{
    off();
}

void LED::on() {
    polarity ? setHigh() : setLow();
}

void LED::off() {
    polarity ? setLow() : setHigh();
}

void LED::blink(uint16_t delay) {
    off();
    for(uint16_t i=0; i<delay; i++)
        _delay_ms(1);
    on();
    for(uint16_t i=0; i<delay; i++)
        _delay_ms(1);
    off();
}

void LED::setState(uint8_t state) {
    state ? on() : off();
}
