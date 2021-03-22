#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include "memoire_24.h"

void initialisationUART ( void );

void transmissionUART ( uint8_t donnee );