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
 */

#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h> 
#include <avr/interrupt.h>

using namespace std;

 enum Etats {INIT, E1, E2, E3,  E4, E5};
 volatile Etats etat = INIT;


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

EICRA |= (1 << ISC00) ;


// sei permet de recevoir à nouveau des interruptions.

sei ();

}



	// placer le bon type de signal d'interruption

	// à prendre en charge en argument

	ISR (INT0_vect) {

	// laisser un delai avant de confirmer la réponse du

	// bouton-poussoir: environ 30 ms (anti-rebond)
	int etatPinD = (PIND &0b100);
	_delay_ms ( 30 );
	


	// se souvenir ici si le bouton est pressé ou relâché

	if ((PIND &0b100) == etatPinD){
		
		
		

		// changements d'états tels que ceux de la

		// semaine précédente

		if ( etat == INIT){
			etat = E1;
		} else {
			if (etat == E1){
				etat = E2;
			} else {
				if (etat == E2){
					etat = E3;
				} else {
					if (etat == E3){
						etat = E4;
					} else {
						if (etat == E4){
							etat = E5;
						} else {
							etat = INIT;
						}
					}
				}
			}
		}
	
	}
	

		// Voir la note plus bas pour comprendre cette instruction et son rôle

	EIFR |= (1 << INTF0) ;

	}
	
int main() 
{
  DDRA = 0xff; // PORT A est en mode sortie
  DDRB = 0xff; // PORT B est en mode sortie
  DDRC = 0xff; // PORT C est en mode sortie
  DDRD = 0; // PORT D est en mode entree
  initialisation();
 

  for(;;)  // boucle sans fin
    {

		
	switch (etat) {
		case INIT : 
		PORTA = 0b10;
			
			break;
		case E1 :
			PORTA = 0b10;
			_delay_ms(5);
			PORTA = 0b01;
			_delay_ms(15);
			
			break;
		case E2 :
			PORTA = 0b01;
			
			break;
		case E3 :
			PORTA = 0b10;
			
			break;
		case E4 :
			PORTA =0b00;
			
			break;
		case E5 :
			PORTA = 0b01;
			
			break;
		}
	}	
  return 0; 
  
}

