/*
 * Nom: compteur 32 bits
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: Ceci est un exemple simple de programme 
 * Version: 1.1
 */

#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h> 
#include <avr/interrupt.h> 

void ajustementPWM ( uint8_t intensite ) {

// mise à un des sorties OC1A et OC1B sur comparaison

// réussie en mode PWM 8 bits, phase correcte

// et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6

// page 177 de la description technique du ATmega324PA)


OCR1A = intensite ;

OCR1B = intensite ;

// division d'horloge par 8 - implique une frequence de PWM fixe

TCCR1A = _BV(COM1A0) | _BV(COM1B0) | _BV(WGM10);

TCCR1B = _BV(CS11) ;

TCCR1C = 0;

}


