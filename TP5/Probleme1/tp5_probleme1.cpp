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

int main() {
	DDRB = 0xFF; // PORT B est en mode sortie. Cela correspond à la DEL.
	  
	PORTB = LIGHT_OFF; 
	  
	Memoire24CXXX memoire;

	uint8_t tabEcriture[] = "*E*C*O*L*E* *P*O*L*Y*T*E*C*H*N*I*Q*U*E*"; // 40 caractères

	memoire.ecriture(0x0000, tabEcriture, 40*sizeof(uint8_t));
	_delay_ms(5);

	uint8_t tabLecture[] = {0};
	memoire.lecture(0x0000, tabLecture, 40*sizeof(uint8_t));

	bool estPareil = true;

	for (int i = 0; i < 40; i++) {
		if(tabEcriture[i] != tabLecture[i])
			estPareil = false;
	}
	
	if (estPareil) {
		for(;;)
			PORTB = GREEN;
	}
	else {
		for(;;)
			PORTB = RED;
	}
}
