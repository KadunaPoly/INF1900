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
//#define DEBUG

#include "debug.h" 
#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h> 
#include "transmissionUART.h"

using namespace std;


int main()
{
  DDRA = 0xff; // PORT A est en mode sortie
  DDRB = 0xff; // PORT B est en mode sortie
  DDRC = 0xff; // PORT C est en mode sortie
  DDRD = 0; // PORT D est en mode entree


  initialisationUART();
  enum Etats {INIT, E1, E2, E3,  E4, E5};
  Etats etat = INIT;
  for(;;)  // boucle sans fin
    {
	switch (etat) {
		case INIT : 
		PORTA = 0b10;
			if  (PIND & 0b100) {
				etat = E1;
				print_debug(etat, __FILE__, __LINE__);

				
			}
			else {
				etat = INIT;
				
			}
			
			break;
		case E1 :
			PORTA = 0b10;
			_delay_ms(5);
			PORTA = 0b01;
			_delay_ms(15);
			if  (PIND & 0b100) {
				etat = E1;
			}
			else {
				etat = E2;
				print_debug(etat, __FILE__, __LINE__);
			}
			
			break;
		case E2 :
			PORTA = 0b01;
			if  (PIND & 0b100) {
				etat = E3;
				print_debug(etat, __FILE__, __LINE__);
			}
			else {
				etat = E2;
			}
		
			break;
		case E3 :
			PORTA = 0b10;
			if  (PIND & 0b100) {
				etat = E3;
			}
			else {
				etat = E4;
				print_debug(etat, __FILE__, __LINE__);
			}
		
			break;
		case E4 :
			PORTA =0b00;
			if  (PIND & 0b100) {
				etat = E5;
				print_debug(etat, __FILE__, __LINE__);
			}
			else {
				etat = E4;
			}
		
			break;
		case E5 :
			PORTA = 0b01;
			if  (PIND & 0b100) {
				etat = E5;
			}
			else {
				etat = INIT;
				print_debug(etat, __FILE__, __LINE__);
			}
		
			break;
		}
	}	
  return 0; 
}

