//! \file main.cpp
//! \brief Main file
//! \author Baptiste Hamard,Franck Mercier, Remy Guyonneau
//! \date 2017 05 24
//!
//! Main file for the Interface card. To change the card identifier, just change the IDMSCAN constant before generating the .hex file.
 
#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "led.h"
#include "pin.h"
#include "canISR.h"
#include <stdlib.h>

#define IDMSGCAN		0x050 //!< the identifier of the Interface card
#define IDBATLSTCAN		0x080 //!< IDentifier of BATterie LiSTening on CAN (IDBATLSTCAN), when battery is to low, the Interface module set alarm on
#define IDALIMLSTCAN	0x091 //!< IDentifier of ALIMentation LiSTening on CAN (IDALIMLSTCAN), listen the voltage and current monitoring
#define CANUSAREABGN	0x0C0 //!< CAN UltraSound AREA BeGiNning (CANUSAREABGN), beginning of us can id
#define CANUSAREASZ		0x00000004  //!< CAN UltraSound AREA SiZe (CANUSAREASZ), Size of us can id (number of us modules)

#define SCREEN_MAX	2



LED redLed(&LED_REDCARD_PORT, LED_REDCARD_PIN, LED_REDCARD_POL);
LED yellowLed(&LED_YELLOWCARD_PORT, LED_YELLOWCARD_PIN, LED_YELLOWCARD_POL);
LED GreenIndicLed(&LED_GREEN_PORT, LED_GREEN_PIN, LED_GREEN_POL);
LED YellowIndicLed(&LED_YELLOW_PORT, LED_YELLOW_PIN, LED_YELLOW_POL);
LED Red1IndicLed(&LED_RED1_PORT, LED_RED1_PIN, LED_RED1_POL);
LED Red2IndicLed(&LED_RED2_PORT, LED_RED2_PIN, LED_RED2_POL);

bool Flag_LowBattery=false;
bool Flag_Monitoring=false;
bool Flag_US=true;
uint16_t V_Bat, prev_V_Bat, V_12V, prev_V_12V, V_5V, prev_V_5V, I_Bat, prev_I_Bat, US_right, prev_US_right, US_front, prev_US_front, US_downfront, prev_US_downfront, US_left, prev_US_left;
uint8_t currentScreen=0, prev_currentSreen=0;
char acceuil[] = {'I','B','O','T',' ','-',' ','I','S','T','I','A','\0'};
char S1L1[17] = {'R',':',' ',' ',' ','m','m',' ',' ','F',':',' ',' ',' ','m','m','\0'};
char S1L2[17] = {'D',':',' ',' ',' ','m','m',' ',' ','L',':',' ',' ',' ','m','m','\0'};
char S2L1[17] = {'B',':',' ',' ',',',' ','V',' ',' ','M',':',' ',' ',',',' ','V','\0'};
char S2L2[17] = {'C',':',' ',',',' ',' ','V',' ',' ','I',':',' ',',',' ',' ','A','\0'};
char S3L1[17] = {'B','A','T','T','E','R','I','E',' ','F','A','I','B','L','E','!','\0'};
char S3L2[17] = {'V','b','a','t','t','e','r','i','e',':',' ',' ',' ',',',' ','V','\0'};

void buzzer(uint16_t tps_ms)
{
	uint32_t nb_boucle = tps_ms / 0.488;
	for (uint32_t i=0; i<nb_boucle; i++)
	{
		_delay_us(244);
		PORTC |= 0x40;
		_delay_us(244);
		PORTC &= 0xBF;
	}
}


int main(void)
{
	// configuration des E/S du µC
	DDRB = 0xFF;				// Port b en sortie
	DDRC |= 0x40;				// Broche piezo en sortie
	DIDR0 = 0xFF;				// Config des entrées analogique en digital
	PORTD = 0x00;				// disable des résistances de pull-up

	for (int i=0; i<5; i++) {
		redLed.blink();
		_delay_ms(200);
		yellowLed.blink();
		_delay_ms(200);
	}

	Lcd4_Init();
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String(acceuil);
	_delay_ms(500);
	GreenIndicLed.blink();
	YellowIndicLed.blink();
	Red1IndicLed.blink();
	Red2IndicLed.blink();
	Red1IndicLed.blink();
	YellowIndicLed.blink();
	GreenIndicLed.blink();
	for (int i=0; i<4; i++)	{
		Lcd4_Shift_Right();
		_delay_ms(200);
	}
	for (int i=0; i<4; i++)	{
		Lcd4_Shift_Left();
		_delay_ms(200);
	}
	Lcd4_Clear();
	buzzer(300);

	// configuration des interruptions sur les entrées PD0 PD1 PD2 PD5 PD6 PD7
	EICRA = 0x30;
	PCICR = 0x04;
	PCMSK2 = 0xE3;
	// sei() is done in initCANMOB. functions !

	// configuration des mobs d'interfacage can
	//		Mob 0 reservé pour l'emission de trames
	//		Mob 1 écoute de l'alarme batterie faible
	//		Mob 2 écoute du monitoring alimentation (V_batterie, V_+5V, V_+12V, I_batterie)
	//		Mob 3 écoute de la zone US
	initCANBus();
	initCANMOBasReceiver(1, IDBATLSTCAN, 0);
	initCANMOBasReceiver(2, IDALIMLSTCAN, 0);
	initCANMOBasIDBandReceiver(3, CANUSAREABGN, CANUSAREASZ, 0);

	while(1)
	{
		if ((currentScreen == 0) & Flag_US)
		{
			char my_string[10];
			if (US_right != prev_US_right) {
				prev_US_right = US_right;
				itoa(US_right, my_string, 10);
				if ((my_string[2] & 0xF0) == 0x30)	{ 
					S1L1[2] = my_string[0]; 
					S1L1[3] = my_string[1]; 
					S1L1[4] = my_string[2];
				}
				else {
					S1L1[2] = ' ';
					S1L1[3] = my_string[0];
					S1L1[4] = my_string[1];	
				}
			}
			if (US_front != prev_US_front) {
				prev_US_front = US_front;
				itoa(US_front, my_string, 10);
				if ((my_string[2] & 0xF0) == 0x30)	{
					S1L1[11] = my_string[0];
					S1L1[12] = my_string[1];
					S1L1[13] = my_string[2];
				}
				else {
					S1L1[11] = ' ';
					S1L1[12] = my_string[0];
					S1L1[13] = my_string[1];
				}
			}
			if (US_downfront != prev_US_downfront) {
				prev_US_downfront = US_downfront;
				itoa(US_downfront, my_string, 10);
				if ((my_string[2] & 0xF0) == 0x30)	{
					S1L2[2] = my_string[0];
					S1L2[3] = my_string[1];
					S1L2[4] = my_string[2];
				}
				else {
					S1L2[2] = ' ';
					S1L2[3] = my_string[0];
					S1L2[4] = my_string[1];
				}
			}
			if (US_left != prev_US_left) {
				prev_US_left = US_left;
				itoa(US_left, my_string, 10);
				if ((my_string[2] & 0xF0) == 0x30)	{
					S1L2[11] = my_string[0];
					S1L2[12] = my_string[1];
					S1L2[13] = my_string[2];
				}
				else {
					S1L2[11] = ' ';
					S1L2[12] = my_string[0];
					S1L2[13] = my_string[1];
				}
			}
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String(S1L1);
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String(S1L2);
			Flag_US = false;
		}
		if ((currentScreen == 1) & Flag_Monitoring)
		{
			char my_string[10];
			if (V_Bat != prev_V_Bat) {
				prev_V_Bat = V_Bat;
				itoa(V_Bat, my_string, 10);
				if ((my_string[3] & 0xF0) == 0x30)	{
					S2L1[2] = my_string[0];
					S2L1[3] = my_string[1];
					S2L1[5] = my_string[2];
				}
				else {
					S2L1[2] = ' ';
					S2L1[3] = my_string[0];
					S2L1[5] = my_string[1];
				}
			}
			if (V_12V != prev_V_12V) {
				prev_V_12V = V_12V;
				itoa(V_12V, my_string, 10);
				if ((my_string[3] & 0xF0) == 0x30)	{
					S2L1[11] = my_string[0];
					S2L1[12] = my_string[1];
					S2L1[14] = my_string[2];
				}
				else {
					S2L1[11] = ' ';
					S2L1[12] = my_string[0];
					S2L1[14] = my_string[1];
				}
			}
			if (V_5V != prev_V_5V) {
				prev_V_5V = V_5V;
				itoa(V_5V, my_string, 10);
				if ((my_string[2] & 0xF0) == 0x30)	{
					S2L2[2] = my_string[0];
					S2L2[4] = my_string[1];
					S2L2[5] = my_string[2];
				}
				else {
					S2L2[2] = ' ';
					S2L2[4] = my_string[0];
					S2L2[5] = my_string[1];
				}
			}
			if (I_Bat != prev_I_Bat) {
				prev_I_Bat = I_Bat;
				itoa(I_Bat, my_string, 10);
				if ((my_string[2] & 0xF0) == 0x30)	{
					S2L2[11] = my_string[0];
					S2L2[13] = my_string[1];
					S2L2[14] = my_string[2];
				}
				else {
					S2L2[11] = ' ';
					S2L2[13] = my_string[0];
					S2L2[14] = my_string[1];
				}
			}
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String(S2L1);
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String(S2L2);
			Flag_Monitoring = false;
		}
		if (Flag_LowBattery)
		{
			if (V_Bat > 10000)
			{
				char my_string[10];
				Lcd4_Set_Cursor(1,0);
				Lcd4_Write_String(S3L1);
				Lcd4_Set_Cursor(2,0);
				Lcd4_Write_String(S3L2);
				while(1)
				{
					if (V_Bat != prev_V_Bat) {
						prev_V_Bat = V_Bat;
						itoa(V_Bat, my_string, 10);
						if ((my_string[3] & 0xF0) == 0x30)	{
							S3L2[11] = my_string[0];
							S3L2[12] = my_string[1];
							S3L2[14] = my_string[2];
						}
						else {
							S3L2[11] = ' ';
							S3L2[12] = my_string[0];
							S3L2[14] = my_string[1];
						}
						Lcd4_Set_Cursor(2,0);
						Lcd4_Write_String(S3L2);
					}
					buzzer(300);
					_delay_ms(300);
				}
			}
			Red2IndicLed.blink(100);
		}
	}
}

// Interrupt vectors
ISR(PCINT2_vect)
{
	cli();
	EIFR = 0;						// raz du flag d'interruption
	
	_delay_us(5);
	if (PIND & 0x01) 
	{
		// ... ToDo .... Right Code
		GreenIndicLed.blink();
		currentScreen++;
		if (currentScreen >= SCREEN_MAX) {	currentScreen = 0;}
	}
	if (PIND & 0x02)
	{
		// ... ToDo .... Up Code
		YellowIndicLed.blink();
		currentScreen++;
		if (currentScreen >= SCREEN_MAX) {	currentScreen = 0;}
	}	
	if (PIND & 0x20)
	{
		// ... ToDo .... Left Code
		Red1IndicLed.blink();
		currentScreen--;
		if (currentScreen >= SCREEN_MAX) {	currentScreen = (SCREEN_MAX-1);}
	}
	if (PIND & 0x40)
	{
		// ... ToDo .... OK Code
		//Red2IndicLed.blink();
		buzzer(1000);
	}
	if (PIND & 0x80)
	{
		// ... ToDo .... Down Code
		YellowIndicLed.blink();
		currentScreen--;
		if (currentScreen >= SCREEN_MAX) {	currentScreen = (SCREEN_MAX-1);}
	}		
	sei();
}

//! \fn ISR(CAN_INT_vect)
//! \brief CAN interruption.
//! This function is called when an CAN interruption is raised.
//! When it appends, the US sensor value is readed and sent using the MOB 0
ISR(CAN_INT_vect){
	cli(); // disable the interruption (no to be disturbed when dealing with one)
	_delay_ms(10);
	uint8_t mask=0x02, mob=0;
	uint8_t rcvData[8];
	bool out = false;
	uint32_t rcvID=0;

	do 
	{
		mob++;
		if (mask & CANSIT2)
		{
			CANPAGE = (mob << 4) & 0xF0;
			out = true;
		}
		mask = mask << 1;
	} while (out == false);
	
	uint8_t dlc = CANCDMOB & 0x0F;
	for (uint8_t i=0; i<dlc; i++)
	{
		CANPAGE &= 0xF0;
		CANPAGE |= i;
		rcvData[i] = CANMSG;
	}
	rcvID = CANIDT1;
	rcvID = (rcvID << 3) | (CANIDT2 >> 5);

	//----------------------------------------------------------------------------------------------

	switch (mob)
	{
		case 1 :	V_Bat = rcvData[0]; V_Bat = V_Bat << 8; V_Bat |= rcvData[1];
					//disableCANMOB(2);
					//disableCANMOB(3);
					Flag_LowBattery=true;
					break;
		case 2 :	V_12V = rcvData[0]; V_12V = V_12V << 8; V_12V |= rcvData[1];
					V_5V = rcvData[2]; V_5V = V_5V << 8; V_5V |= rcvData[3];
					V_Bat = rcvData[4]; V_Bat = V_Bat << 8; V_Bat |= rcvData[5];
					I_Bat = rcvData[6]; I_Bat = I_Bat << 8; I_Bat |= rcvData[7];
					Flag_Monitoring=true;
					break;
		case 3 :	if (rcvID == 0x0C0)	{ US_right = rcvData[0]; US_right = US_right << 8; US_right = rcvData[1];}
					if (rcvID == 0x0C1)	{ US_front = rcvData[0]; US_front = US_front << 8; US_front = rcvData[1];}
					if (rcvID == 0x0C2)	{ US_downfront = rcvData[0]; US_downfront = US_downfront << 8; US_downfront = rcvData[1];}
					if (rcvID == 0x0C3)	{ US_left = rcvData[0]; US_left = US_left << 8; US_left = rcvData[1]; }
					Flag_US=true;
					break;
		default :	break;
	}

	//----------------------------------------------------------------------------------------------
	// reconfiguration du mob en écoute
	CANSTMOB = 0x00; // Reset the status of the MOB
	CANCDMOB = 0x80; // Config as reception MOB
	switch (mob) {
		case 0 : CANIE2 |= 0x01; break;
		case 1 : CANIE2 |= 0x02; break;
		case 2 : CANIE2 |= 0x04; break;
		case 3 : CANIE2 |= 0x08; break;
		case 4 : CANIE2 |= 0x10; break;
		case 5 : CANIE2 |= 0x20; break;
		default : break;
	}
	sei(); // enable the interruptions
}