# define F_CPU 8000000UL
#include "transmissionUART.h"

// Routine d'initialisation


TransmissionUART::TransmissionUART() {

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

void TransmissionUART::reinit(){
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

void TransmissionUART::transmissionUART_entier(uint8_t donnee){
	char a[8];
	snprintf(a, sizeof(a), "%d\n", donnee);
	for (uint8_t i = 0; i < sizeof(a); i++){
	   	transmission(a[i]);
    }
}

void TransmissionUART::transmissionUART_string(char* donnee){
	char a[100];
	snprintf(a, sizeof(a), "%s\n", donnee);
	for (uint8_t i = 0; i < sizeof(a); i++){
	   	transmission(a[i]);
    }

}

void TransmissionUART::transmissionUART_info (char* file, int line){
	transmissionUART_string( "fichier : ");
	transmissionUART_string( file);
	transmissionUART_string( "ligne : ");
	transmissionUART_entier(line);
}

// procédure qui envoie un octet de la carte vers le PC
void TransmissionUART::transmission( uint8_t donnee ) {
	/* Wait for empty transmit buffer */
   while ( !( UCSR0A & (1<<UDRE1)) );
   /* Put data into buffer, sends the data */
   UDR0 = donnee;
}