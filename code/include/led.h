#ifndef LED_H
#define LED_H
#include <stdint.h>
#include <output.h>


/**
 * @brief      LED class. This class inherit from Output and implements behavior
 *             of an LED.
 */
class LED : public Output
{
public:

    /**
     * @brief      LED constructor
     *
     * @param      port      The port of the led pin.
     * @param[in]  pinmask   The pinmask for the led pin.
     * @param[in]  polarity  The polarity of the led (1=on with high state)
     */
    LED(volatile uint8_t * port, uint8_t pinmask, uint8_t polarity);

    /*!
     * @brief      on   Switch on the LED
     */
    void on();

    /*!
     * @brief      off   Switch off the LED
     */
    void off();

    /*!
     * \brief blink Blink the LED
     * \param delay The on/off delay (ms) for blinking
     */
    void blink(uint16_t delay=50);

    /*!
     * \brief setState Set LED State (1: on, 0:off)
     * \param state State to set (1: on, 0:off)
     */
    void setState(uint8_t state);

private:
    /*!
     * @brief      polarity   Polarity of the LED
     * @note       If polarity=1, the LED is ON on a high state
     */
    uint8_t polarity;
};

#endif // LED_H
