// Auteur : Philippe Proulx (philippe.proulx@polymtl.ca)
// Test du framework C++ pour afficheur LCD LCM-SO1602DTR/M.
# define F_CPU 8000000UL
#include <stdlib.h>

#include <avr/io.h>
#include "util/delay.h"
#include <lcm_so1602dtr_m_fw.h>
#include <customprocs.h>
#include "affichage.h"
#include <string.h>
#include <stdio.h>

//#define DEMO_DDR	DDRC // `Data Direction Register' AVR occupé par l'aff.
//#define DEMO_PORT	PORTC // Port AVR occupé par l'afficheur
using namespace std;
int main() {
	//Configuration des ports
	DDRA = 0xff; // PORT A est en mode entree
	DDRB = 0xff; // PORT B est en mode sortie
	DDRC = 0xff; // PORT C est en mode sortie
	DDRD = 0xff; // PORT D est en mode sortie

	// Ajustement pour afficher distance mesuree a gauche
	double distanceAGauche = 32.5;
	uint16_t valEntiereGauche = floor(distanceAGauche);
	uint16_t valeurDecimaleGauche = distanceAGauche*10 - valEntiereGauche *10;

	// Ajustement pour afficher distance mesuree a droite
	double distanceADroite = 27.3;
	uint16_t valEntiereDroite = floor(distanceADroite);
	uint16_t valeurDecimaleDroite = distanceADroite*10 - valEntiereDroite *10;

	LCM disp(&DDRC, &PORTC);
	disp.clear();
	disp << " Gauche : " << valEntiereGauche << "." << valeurDecimaleGauche;
	disp[16] = " Droite : ";
	disp << valEntiereDroite << "." << valeurDecimaleDroite;
	return 0;
}
