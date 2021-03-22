#include "avr/io.h"
#include "util/delay.h"
#include "piezo.h"
#include "constantes.h"

#define F_CPU 8000000UL

void setup() {
	DDRB = 0xff; // PORT C est en mode sortie
	DDRD = 0xfb; // PORT D est en mode sortie	
}

int main()
{
	setup();
	Piezo piezo = Piezo();
	//Jouer une note quelconque
	piezo.jouerNote(71);

	return 0;
}
