#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h> 
#include <avr/interrupt.h>
#include <can.h>
#include <string.h>
#include <stdio.h>

using namespace std;

// Routine d'initialisation
 void initialisationUART ( void ) {

	// 2400 bauds. Nous vous donnons la valeur des deux

	// premier registres pour vous éviter des complications

	UBRR0H = 0;

	UBRR0L = 0xCF;

	// permettre la reception et la transmission par le UART0

	UCSR0A = _BV(RXC0) | _BV(TXC0);

	UCSR0B = _BV(RXEN0) | _BV(TXEN0) ;

	// Format des trames: 8 bits, 1 stop bits, none parity

	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00) ;

}

// procédure qui envoie un octet de la carte vers le PC

void transmissionUART ( uint8_t donnee ) {
	/* Wait for empty transmit buffer */
   while ( !( UCSR0A & (1<<UDRE1)) );
   /* Put data into buffer, sends the data */
   UDR0 = donnee;
}
	
int main() 
{
	initialisationUART ();
	
	// configuration et choisir les ports pour les entrées et sortie
	DDRA = 0x00; // PORT A est en mode entree
	DDRB = 0xff; // PORT B est en mode sortie
	DDRC = 0xff; // PORT C est en mode sortie
	DDRD = 0; // PORT D est en mode entree
http://www.groupes.polymtl.ca/inf1900/tp/tp6/
		can convAN;
  for(;;)  // boucle sans fin
    {
		
		// Permet de passer d'un entier de 10 bits a un entier de 8 bits
		// en decalant de deux bits vers la droite
		// max ambiant 200
		uint8_t lecture = convAN.lecture(1) >> 2;
		
		if(lecture < 150){
			PORTB = 0b01; // DEL allume en vert
		}
		else {
			if (lecture > 200){
				PORTB = 0b10; // DEL allume en rouge
			}
			else {
					PORTB = 0b01;
					_delay_ms(15);
					PORTB = 0b10;
					_delay_ms(5);
		
					}
			}
		char a[5];
		
		snprintf(a, sizeof(a), "%d\n", lecture);
		for (uint8_t i = 0; i < sizeof(a); i++){
			transmissionUART(a[i]);
		}
	}
	return 0;
}
