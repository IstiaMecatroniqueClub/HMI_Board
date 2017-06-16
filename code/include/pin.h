#ifndef PIN_H
#define PIN_H

#include <stdint.h>


/**
 * @brief      Pin class. This class represent a Pin and shouldn't be
 *             instanciated, only inherited
 */
class Pin
{
public:
protected:
    /*!
     * @brief      Pin       Constructor to represent a Pin
     *
     * @param      port     Volatile pointer to the SFR memspace for this port
     * @param      pinmask  Id of the pin to use
     */
    Pin(volatile uint8_t * port, uint8_t pinmask);

    /*!
     * @brief      port     volatile pointer to the PORTx register for this pin
     */
    volatile uint8_t * port;

    /*!
     * @brief      ddr      volatile pointer to the DDRx register for this pin
     */
    volatile uint8_t * ddr;

    /*!
     * @brief      pin      volatile pointer to the PINx register for this pin
     */
    volatile uint8_t * pin;

    /*!
     * @brief      pinmask  Id of the pin to use on this port
     */
    uint8_t pinmask;
};

#endif // PIN_H
