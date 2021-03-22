#define F_CPU 8000000
#include "avr/io.h"
#include <avr/interrupt.h>
#include "util/delay.h"
#include "piezo.h"
#include "constantes.h"
#include "memoire_24.h"
#include "pwm.h"
#include "transmissionUART.h"
#include <string.h>
#include <stdio.h>
#include <minuterie.h>
#include <lcm_so1602dtr_m_fw.h>
#include <customprocs.h>

int main() {

	//Configuration des ports
	DDRA = 0x00; // PORT A est en mode entree
	DDRB = 0xff; // PORT B est en mode sortie
	DDRC = 0xff; // PORT C est en mode sortie
	DDRD = 0xff; // PORT D est en mode sortie

	
	PWM pwm;
	uint16_t duree = 0;
	//pwm.reculer(100);
	while (duree < 2800)
	{
		pwm.roueDroite(false, 40);
		duree +=1;
		_delay_ms(1);
	}
	pwm.arreter();
	return 0;
}
