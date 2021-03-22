#include "minuterie.h"

void partirMinuterie (uint16_t duree) {

	TCNT1 = 0 ; // Démarrer le compteur à 0

	OCR1A = duree; // Permet de compter à la valeur de durée

	TCCR1A |= _BV(COM1A0); // Mode CTC du timer 1 avec horloge divisée par 1024

	TCCR1B |= _BV(CS12)| _BV(CS10) | _BV(WGM12); // Mode CTC du timer 1 avec horloge divisée par 1024

	TCCR1C = 0; // Mode CTC du timer 1 avec horloge divisée par 1024

	TIMSK1 |= _BV(OCIE1A); // Timer/Counter1: interruption après que la durée est atteinte par le compteur
}