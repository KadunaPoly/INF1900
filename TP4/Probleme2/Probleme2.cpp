/*
 * Entree : bouton poussoir (BP), E=1 bouton presse
 * Sorties : PORTA (rouge = 0b10, vert = 0b01, ambre = 0b10 et 0b01)
 * Etats possibles :
 * - INIT
 * - E1
 * - E2 
 * - E3 
 * - E4 
 * - E5
 * Tableau des etats
 * etat present		E		etat suivant	sortie PORTA
 * INIT				0			INIT			0b10
 * INIT				1			E1				0b10 et 0b01										
 * E1				0			E2				0b01			
 * E1				1			E1				0b10 et 0b01						
 * E2				0			E2				0b01					
 * E2				1			E3				0b10									
 * E3				0			E4				0b00				
 * E3				1			E3				0b10								
 * E4				0			E4				0b00						
 * E4				1			E5				0b01									
 * E5				0			INIT			0b10								
 * E5				1			E5				0b01	
modif 
*/

#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h> 
#include <avr/interrupt.h>

using namespace std;

volatile uint8_t minuterieExpiree;

volatile uint8_t boutonPoussoir; 


ISR (TIMER1_COMPA_vect) {

minuterieExpiree = 1;

}


ISR ( INT0_vect ) {

// anti-rebond

int etatPinD = (PIND &0b100);
_delay_ms ( 30 );
	
// se souvenir ici si le bouton est pressé ou relâché

if ((PIND &0b100) == etatPinD){
	
	boutonPoussoir = 1;

}
}

void partirMinuterie ( uint16_t duree ) {

minuterieExpiree = 0;

// mode CTC du timer 1 avec horloge divisée par 1024

// interruption après la durée spécifiée

TCNT1 = 0 ; // compteur de depart

OCR1A = duree;

TCCR1A = (1 << COM1A0) ; // pas d'output quand il y a match 

TCCR1B = (1 << WGM12 | 1 << CS12 | 1 << CS00) ; // clk I/O /1024 
// (From prescaler) table 16-5 CTC avec OCRnA

TCCR1C = 0; 

TIMSK1 = (1 << OCIE1A) ; // 00000010, car variable d'interruption de la
// minuterie ISR(TIMER1_COMPA)

}




void initialisation () {

// cli est une routine qui bloque toutes les interruptions.

// Il serait bien mauvais d'être interrompu alors que

// le microcontroleur n'est pas prêt...

cli ();


// configurer et choisir les ports pour les entrées

// et les sorties. DDRx... Initialisez bien vos variables

DDRA = 0xff;
DDRD = 0x00;


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



	
int main() 
{
  DDRA = 0xff; // PORT A est en mode sortie
  DDRB = 0xff; // PORT B est en mode sortie
  DDRC = 0xff; // PORT C est en mode sortie
  DDRD = 0; // PORT D est en mode entree
  
  _delay_ms(4000);
  PORTA = 0b10;
  _delay_ms(100);
  PORTA = 0;
  uint16_t duree = 3000;
  initialisation();
	partirMinuterie(duree);
	
	do {
	
// attendre qu'une des deux variables soit modifiée

// par une ou l'autre des interruptions.
	
	} while ( minuterieExpiree == 0 && boutonPoussoir == 0 );


// Une interruption s'est produite. Arrêter toute

// forme d'interruption. Une seule réponse suffit.

cli ();

// Verifier la réponse
for (;;){
	if( minuterieExpiree == 1){
		PORTA = 0b10;
	}
	else {
		PORTA = 0b01;
	}
	
}
  return 0; 
}

