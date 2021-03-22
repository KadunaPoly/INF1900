#include <affichage.h>
#include "util/delay.h"
#include <stdlib.h>
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
#include <string.h>
#include <stdio.h>

#define DEMO_DDR	DDRC // `Data Direction Register' AVR occupé par l'aff.
#define DEMO_PORT	PORTC // Port AVR occupé par l'afficheur

Affichage::Affichage() : disp(&DEMO_DDR, &DEMO_PORT)
{
    PORTA = 0b01;
    _delay_ms(500);
    
}

void Affichage::afficherNomDuRobot()
{ 
    // On ne rentre pas la dedans
    PORTA = 0b01;
    _delay_ms(1000);
    disp.clear();
    //disp = "Mr Propre";
    disp = "ami";
    _delay_ms(5000);
}
void Affichage::afficherDistance(double distanceADroite, double distanceAGauche){
    PORTD = 0b10;
    _delay_ms(1000);
    char* a = "Gauche :";
	//snprintf(a, sizeof(a), "%d\n", distanceAGauche);
    disp << a;
    //disp[11] = distanceAGauche
}


