# define F_CPU 8000000UL
#include <avr/io.h>
#include "piezo.h"
#include "constantes.h"
#include <util/delay.h>


using namespace std;

Piezo::Piezo(){
	
}

Piezo::~Piezo(){

}

void Piezo::jouerNote(uint8_t note){	
	TCCR0A |= (1<<COM0A0)|(1<<WGM01);
	TCCR0A &= ~(1<<COM0A1);
	TCCR0B |= (1 << CS02); //prescaler
	
	if (note >= MIN_MIDI && note <= MAX_MIDI){
		OCR0A = NOTE[note - 45];
		
		
	}

}

//const uint8_t *chanson, uint8_t size
void Piezo::desactiverNote(){
	TCCR0A &= ~(1 << COM0A0);

}
