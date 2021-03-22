
#define F_CPU 8000000
#include "memoire_24.h"
#include "pwm.h"

void lireDonnees(uint16_t& adresse,/*const*/ Memoire24CXXX & memoire, uint8_t & instruction, uint8_t & operande){
	memoire.lecture(adresse, instruction, sizeof(uint8_t));
	adresse += 0x08;
	memoire.lecture(adresse, operande, sizeof(uint8_t));
	adresse += 0x08;
}

int main() {

	//Configuration des ports
	DDRA = 0xff; // PORT A est en mode sortie
	DDRB = 0xff; // PORT B est en mode sortie
	DDRC = 0xff; // PORT C est en mode sortie
	DDRD = 0xff; // PORT D est en mode sortie



	for(;;){
		Memoire24CXXX memoire; // Creation d'un objet de type Memoire24CXXX
		PWM pwm; // Creation d'un objet de type PWM
		uint8_t instruction = 0; // 0 est une valeur non utilisée
		uint8_t operande = 0;
		uint16_t adresse = 0x00;
		// lecture du nombre d'instructions
		uint16_t nbInstructions;
		memoire.lecture(adresse, nbInstructions, sizeof(uint16_t));
		nbInstructions = (nbInstructions - 0x02) / 0x02;
		adresse += 0x10; //Passer a la premiere instruction

		do {
		memoire.lecture(adresse, instruction, sizeof(uint8_t));
		// Instruction est sur 8 bits et operande sur 8 bits aussi
		// Pour lire la prochaine instruction il faut sauter 16 bits
		adresse += 0x10;
		nbInstructions--;
		}
		while ( instruction != 0x01 && nbInstructions > 0x00 );

		// Boucle for pour ne pas lire de la memoire qui n'est pas prévue pour ca
		bool finTrouvee = false;
		for (uint16_t i = 0x00; i < nbInstructions && !finTrouvee; i++){
			lireDonnees(adresse, memoire, instruction, operande);
			switch (instruction){
				case 0x02 : // attendre att
					for (uint16_t i = 0; i < operande; i ++){
						_delay_ms(25)
					}
					break;

				case 0x44 : // allumer les DEL : dal
					// Possibilite d'allumer la DEL en rouge ou en vert en fonction de l'operande
					if (operande == 0x01){ // Allume en vert
						PORTA = 0b01;
					}
					else { // Allume en rouge
						PORTA = 0b10;
					}
					
					break;

				case 0x45 : // eteindre les DEL : det

					PORTA = 0;
					break;
				
				case 0x48 : // jouer une sonorite : SGO
					// En attente du code MIDI
					break;

				case 0x09 : // arreter de jouer la sonorite en cours : SAR
					// En attente du code MIDI
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
					break;

				case 0xC1 : // fin de boucle : FBC
					break;

				case 0xFF : // fin : fin
					finTrouvee = true;
					break;

			}
		}
	}
	return 0;
}
