/**
 * Auteurs : Paul Corrand (1886657) & Simon Zhang (1839892)
 * Numéro d'équipe = #41
 * Nom : TP5 - Problème 1 - Lecture et écriture en mémoire
 * 
 * Description : Vous devrez faire un programme qui écrit la chaîne de caractères « *P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*» 
	 * suivi d'un 0x00 en mémoire externe. La chaîne commencera à l'adresse 0x00. Faire en sorte que votre programme puisse aller 
	 * relire la chaîne en question. Comparez la chaîne de caractères envoyée à la mémoire et celle relue. Il suffira de faire 
	 * afficher la DEL en vert si elles sont identiques et en rouge dans le cas contraire. Cette démarche est un peu douteuse au départ, 
	 * mais elle se raffinera avec le problème 3. Il faut prévoir un délai de 5 ms après l'écriture d'un octet en mémoire et l'accès suivant 
	 * à la mémoire. De cette façon, la mémoire peut compléter son cycle d'écriture sans problème.
 */
 

#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include "memoire_24.h"


const uint8_t GREEN = 0x02;
const uint8_t RED = 0x01;
const uint8_t LIGHT_OFF = 0x00;

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

unsigned char USART_Receive( void )
{
/* Wait for data to be received */
while ( !(UCSR0A & (1<<RXC0)) )
;
/* Get and return received data from buffer */
return UDR0;

}

void transmissionUART ( uint8_t donnee ) {
	/* Wait for empty transmit buffer */
   while ( !( UCSR0A & (1<<UDRE1)) );
   /* Put data into buffer, sends the data */
   UDR0 = donnee;
}


int main() {

		
	uint8_t taille_octet_1 = USART_Receive();
	uint8_t taille_octet_2 = USART_Receive();

	Memoire24CXXX memoire;

	for (uint8_t i = 0; i < taille_octet_2;i++)
	{
		// comment etre sur qu'on a ecrit ???
		memoire.ecriture(i, USART_Receive());
	}
 	/*
	uint8_t donnee;

	for (uint8_t i = 0; i < taille_octet_2;i++)
	{
		PORTA=0X1;
		memoire.lecture(i,donnee);
		transmissionUART(donnee);

	}


	
	
	uint8_t tabEcriture[] = "*E*C*O*L*E* *P*O*L*Y*T*E*C*H*N*I*Q*U*E*"; // 40 caractères
	
	uint8_t tabLecture[] = {0};
	memoire.lecture(0x0000, tabLecture, 40*sizeof(uint8_t));
	
	
	*/	
	

 return 0;

	
}
