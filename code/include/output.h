#ifndef OUTPUT_H
#define OUTPUT_H

#include <pin.h>


/**
 * @brief      Output class. This class described the behavior for an output. It
 *             inherit from Pin.
 */
class Output : public Pin
{
public:

    /**
     * @brief      Constructor for an output
     *
     * @param      port     The port of the pin
     * @param[in]  pinmask  The pin id
     */
    Output(volatile uint8_t * port, uint8_t pinmask);

    /*!
     * @brief      setHigh   Switch pin to high state
     */
    void setHigh();

    /*!
     * @brief      setLow    Switch pin to low state
     */
    void setLow();

    /*!
     * @brief      toggle    Toggle pin state
     */
    void toggle();
};

#endif // OUPUT_H
