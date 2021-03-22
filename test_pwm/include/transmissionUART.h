//# define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include "memoire_24.h"
#include <string.h>
#include <stdio.h>
class TransmissionUART{
public:
	TransmissionUART(); // Constructeur par defaut, initialise les registres
    void reinit(); // Permet de reinitialiser les registres au cas ou ils sont changés
    void transmission(uint8_t donnee);
    void transmissionUART_entier ( uint8_t donnee ); // TransmissionUART d'un entier
    void transmissionUART_string(char* donnee); // TransmissionUART d'un string
    void transmissionUART_info (char* file, int line);
};




