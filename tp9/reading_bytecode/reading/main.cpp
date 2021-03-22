#include "avr/io.h"
#include "util/delay.h"
#include "piezo.h"
#include "constantes.h"
#include "memoire_24.h"
#include "pwm.h"
#include "debug.h"
#include "transmissionUART.h"
#include <string.h>
#include <stdio.h>


void lireDonnees(uint8_t & adresse, Memoire24CXXX  memoire, uint8_t* instruction, uint8_t* operande){
	memoire.lecture(adresse, instruction);
	adresse ++;
	memoire.lecture(adresse, operande);
	adresse ++;
}





int main() {

	//Configuration des ports
	DDRA = 0xff; // PORT A est en mode sortie
	DDRB = 0xff; // PORT B est en mode sortie
	DDRC = 0xff; // PORT C est en mode sortie
	DDRD = 0xff; // PORT D est en mode sortie


	for(;;){
		// Creation des variables utilisees

		Memoire24CXXX memoire = Memoire24CXXX(); // Creation d'un objet de type Memoire24CXXX
		PWM pwm = PWM(); // Creation d'un objet de type PWM
		Piezo piezo = Piezo(); // Creation d'un objet de type Piezo
		TransmissionUART transmissionUART = TransmissionUART(); // Creation d'un objet de type transmissionUART
		Debug debug = Debug(); // Creation d'un objet de type transmissionUART
		uint8_t  instruction = 0; // 0 est une valeur non utilisée
		uint8_t  operande = 0;
		uint8_t adresse = 0x00;
		// variables pour les boucles
		uint8_t adresse2 ;
		uint8_t nbIteration;
		uint8_t nbIterationComplete;
		// lecture du nombre d'instructions
		uint8_t  nbInstructions;

		// Determination du nombre d'instructions
		memoire.lecture(adresse, & nbInstructions, sizeof(uint8_t));
		nbInstructions = (nbInstructions - 0x02) / 0x02;
		adresse = 0x02; //Passer a la premiere instruction

		do {
		memoire.lecture(adresse, & instruction, sizeof(uint8_t));
		// Instruction est sur 8 bits et operande sur 8 bits aussi
		// Pour lire la prochaine instruction il faut sauter 16 bits
		adresse += 02;
		nbInstructions--;
		}
		while ( instruction != 0x01 && nbInstructions > 0x00 );
		
		// Boucle for pour ne pas lire de la memoire qui n'est pas prévue pour ca
		bool finTrouvee = false;
		for (uint16_t i = 0x00; i < nbInstructions && !finTrouvee; i++){
			lireDonnees(adresse, memoire, & instruction, & operande);
			debug.print_debug_entier8bits(instruction);
			switch (instruction){
				case 0x02 : // attendre att
					for (uint16_t i = 0; i < operande; i ++){
						_delay_ms(25);
					}
					break;

				case 0x44 : // allumer les DEL : dal
					PORTA = 0b10;
					break;

				case 0x45 : // eteindre les DEL : det

					PORTA = 0;
					break;
				
				case 0x48 : // jouer une sonorite : SGO
					//Jouer une note quelconque
					piezo.jouerNote(67);

					break;

				case 0x09 : // arreter de jouer la sonorite en cours : SAR
					//Jouer une note quelconque
					piezo.desactiverNote();
					break;

				case 0x60 : // arreter les moteurs : MAR
					pwm.arreter();
					break;

				case 0x61 : // arreter les moteurs : MAR
					pwm.arreter();
					break;

				case 0x62 : // avancer : MAV
					pwm.avancer((operande / 0xFF) * 100);
					break;
				
				case 0x63 : // reculer : MRE
					pwm.reculer((operande / 0xFF) * 100);
					break;

				case 0x64 : // Trouner a droite : TRD
					pwm.tournerADroite();
					break;

				case 0x65 : // tourner a gauhce : TRG
					pwm.tournerAGauche();
					break;
				
				case 0xC0 : // debut de boucle : DBC
					adresse2 = adresse; // enrengistrement de l'adresse debut de boucle
					nbIteration = operande; // enrengistrement du nombre d'iteration a faire
					nbIterationComplete = 0; // initialisation du nombre d'iteration completee
					break;

				case 0xC1 : // fin de boucle : FBC
					nbIterationComplete ++; // incrementation du nombre d'iteration completee
					if(nbIterationComplete < nbIteration){ // Si on a pas fait le nombre d'iteration necessaire
						adresse = adresse2; // revenir a l'adresse debut de boucle
					}
					break;

				case 0xFF : // fin : fin
					finTrouvee = true;
					break;

			}
		}
	}
	return 0;
}
