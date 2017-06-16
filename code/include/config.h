/*
 * config.h
 
	Config MotorBoard
	Auteur: F.Mercier
	Date: 01/12/2016
 
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

//_____ I N C L U D E S ________________________________________________________
#include <avr/io.h>
#include <avr/interrupt.h>

//_____ D E F I N I T I O N S __________________________________________________

/*********************************/
/*    MCU LIB CONFIGURATION      */
/*********************************/

#define FOSC           16000
#define F_CPU          16000000UL

/*********************************/
/*    CAN LIB CONFIGURATION      */
/*********************************/

#define CAN_BAUDRATE   500        // in kBit
//#define CAN_BAUDRATE   CAN_AUTOBAUD

/************************/
/*    UART              */
/************************/

#define			UART_BITRATE            1
#define			UART_BAUDRATE           115200

/************************/
/*    LED               */
/************************/

#define LED_REDCARD_PORT    PORTB
#define LED_REDCARD_PIN     3
#define LED_REDCARD_POL     0

#define LED_YELLOWCARD_PORT    PORTB
#define LED_YELLOWCARD_PIN     2
#define LED_YELLOWCARD_POL     0

#define LED_GREEN_PORT    PORTC
#define LED_GREEN_PIN     0
#define LED_GREEN_POL     0

#define LED_YELLOW_PORT    PORTC
#define LED_YELLOW_PIN     1
#define LED_YELLOW_POL     0

#define LED_RED1_PORT    PORTC
#define LED_RED1_PIN     4
#define LED_RED1_POL     0

#define LED_RED2_PORT    PORTC
#define LED_RED2_PIN     5
#define LED_RED2_POL     0

/***************************/
/*    Connections LCD      */
/***************************/

#define D4 eS_PORTB4
#define D5 eS_PORTB5
#define D6 eS_PORTB6
#define D7 eS_PORTB7
#define RS eS_PORTB0
#define EN eS_PORTB1


//_____ D E C L A R A T I O N S ________________________________________________

#endif  // _CONFIG_H_
