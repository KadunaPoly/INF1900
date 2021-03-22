#include "minuterie.h"


Minuterie::Minuterie()
{
	// cli est une routine qui bloque toutes les interruptions.

// Il serait bien mauvais d'être interrompu alors que
// le microcontroleur n'est pas prêt...

cli ();

// cette procédure ajuste le registre EIMSK
// de l’ATmega324PA pour permettre les interruptions externes

EIMSK |= (1 << INT0) ;


// il faut sensibiliser les interruptions externes aux
// changements de niveau du bouton-poussoir

// en ajustant le registre EICRA

EICRA |= (1 << ISC00 | 1 << ISC01) ;


// sei permet de recevoir à nouveau des interruptions.

sei ();

}


void Minuterie::partirMinuterie (uint16_t duree) {

	//minuterieExpiree = 0;

	TCNT1 = 0 ; // Démarrer le compteur à 0

	OCR1A = duree; // Permet de compter à la valeur de durée

	TCCR1A |= _BV(COM1A0); // Mode CTC du timer 1 avec horloge divisée par 1024

	TCCR1B |= _BV(CS12)| _BV(CS10) | _BV(WGM12); // Mode CTC du timer 1 avec horloge divisée par 1024

	TCCR1C = 0; // Mode CTC du timer 1 avec horloge divisée par 1024

	TIMSK1 |= _BV(OCIE1A); // Timer/Counter1: interruption après que la durée est atteinte par le compteur
}