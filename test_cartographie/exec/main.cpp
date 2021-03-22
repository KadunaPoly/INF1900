#define F_CPU 8000000
#include "avr/io.h"
#include <avr/interrupt.h>
#include "util/delay.h"
#include "piezo.h"
#include "constantes.h"
#include "memoire_24.h"
#include "pwm.h"
#include "transmissionUART.h"
#include <string.h>
#include <stdio.h>
#include <minuterie.h>
#include <lcm_so1602dtr_m_fw.h>
#include <customprocs.h>
#include "can.h"



#include <math.h>

static const char term_fmt[] = "Voltage:%u\tInverse:%u\tDistance:%u\n";
static const char term_info[] = "out of range\t Voltage:%u\n";
/*
static const char long10[] = "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
static const char long15[] = "@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
static const char long20[] = "@@@@@@@@@@@@@@@@@@@@@@@@\n";
static const char long25[] = "@@@@@@@@@@@@@@@@@@@@@\n";
static const char long30[] = "@@@@@@@@@@@@@@@@@@\n";
static const char long35[] = "@@@@@@@@@@@@@@@\n";
static const char long40[] = "@@@@@@@@@@@@\n";
static const char long45[] = "@@@@@@@@@\n";
static const char long50[] = "@@@@@@\n";
static const char long55[] = "@@@\n";
static const char long60[] = "@\n";
static const char long65[] = "\t\n";*/

/*void
print_term(uint16_t voltage, uint16_t distance, uint16_t power, TransmissionUART &uart)
{
  char buff[64];

  size_t n =  sprintf(buff,
		      term_fmt,
		      voltage,
		      distance,power);

  if (n > 64)
    return;

  //uart.transmissionUART_string(buff);
}

void print_info(uint16_t voltage , TransmissionUART &uart)
{
	char buffer[32];

    size_t n =  sprintf(buffer, term_info, voltage);

    if (n > 32)
      return;
	//uart.transmissionUART_string(buffer);	
}
*/

volatile const double DISTANCE = 60.96;
volatile const int8_t NBCARACTERES = 95;
//Note importante : la distance recue par le capteur est un double 
void CartographierLigne(double distanceADroite, double distanceAGauche, uint16_t adresse)
{
	Memoire24CXXX memoire = Memoire24CXXX();
	//uint16_t ligne[];
	uint8_t ligne[193];
	double temp = ((DISTANCE - distanceADroite)*NBCARACTERES)/DISTANCE;
	int16_t nombreADroite = floor(temp);
	int16_t position = 0;
	if(((temp*10)-(nombreADroite*10))>= 5)
	{
		nombreADroite++;
	}
	temp = ((DISTANCE - distanceAGauche)*NBCARACTERES)/DISTANCE;
	int16_t nombreAGauche = floor(temp);
	if(((temp*10)-(nombreAGauche*10))>= 5)
	{
		nombreAGauche++;
	}
	for(position = 0; position < nombreAGauche; position++)
	{
		ligne[position] = '@';
	}
	for(position = nombreAGauche; position < NBCARACTERES; position++)
	{
		ligne[position] = ' ';
	}
	ligne[NBCARACTERES] = '|';
	ligne[(NBCARACTERES+1)] = '|';
	for(position = (NBCARACTERES+2); position < ((NBCARACTERES+2)+(NBCARACTERES - nombreADroite)); position++)
	{
		ligne[position] = ' ';
	}
	for(position = ((NBCARACTERES+2)+(NBCARACTERES - nombreADroite)); position < ((NBCARACTERES + 1)*2 ); position++)
	{
		ligne[position] = '@';
	}
	ligne[((NBCARACTERES + 1)*2 )] = '\n';
	//adresse++;
	
	memoire.ecriture(adresse, ligne, sizeof(ligne));

/*
	for (uint8_t i = 0; i < sizeof(ligne); i++) {
		PORTB = 0b10;
		uint8_t ligne[1];
		memoire.lecture(i, ligne);
		transmissionUART (ligne[0] );

	}
*/
}

void CartographierLigne2(uint8_t distanceADroite, uint8_t distanceAGauche,
                        uint16_t adresse) {

  	Memoire24CXXX memoire = Memoire24CXXX();
	//uint16_t ligne[];
	uint8_t ligne[193];
	double temp = ((DISTANCE - distanceADroite)*NBCARACTERES)/DISTANCE;
	int16_t nombreADroite = floor(temp);
	int16_t position = 0;
	if(((temp*10)-(nombreADroite*10))>= 5)
	{
		nombreADroite++;
	}
	temp = ((DISTANCE - distanceAGauche)*NBCARACTERES)/DISTANCE;
	int16_t nombreAGauche = floor(temp);
	if(((temp*10)-(nombreAGauche*10))>= 5)
	{
		nombreAGauche++;
	}
	for(position = 0; position < nombreAGauche; position++)
	{
		ligne[position] = '@';
	}
	for(position = nombreAGauche; position < NBCARACTERES; position++)
	{
		ligne[position] = ' ';
	}
	ligne[NBCARACTERES] = '|';
	ligne[(NBCARACTERES+1)] = '|';
	for(position = (NBCARACTERES+2); position < ((NBCARACTERES+2)+(NBCARACTERES - nombreADroite)); position++)
	{
		ligne[position] = ' ';
	}
	for(position = ((NBCARACTERES+2)+(NBCARACTERES - nombreADroite)); position < ((NBCARACTERES + 1)*2 ); position++)
	{
		ligne[position] = '@';
	}
	ligne[((NBCARACTERES + 1)*2 )] = '\n';
	//adresse++;
	
	memoire.ecriture(adresse, ligne, sizeof(ligne));
}

/*volatile const double DISTANCE = 60.96;
//Note importante : la distance recue par le capteur est un uint16_t dont la valeur réeelle a été multiplée par 100
void CartographierLigne(double distanceADroite, double distanceAGauche)
{
	uint16_t nbAGauche = distanceAGauche;
	nbAGauche++;
	uint16_t nbADroite = distanceADroite;
	nbADroite++;
	//uint16_t ligne[];

	//for 
}
*/


 void initialisationUART ( void ) {

	// 2400 bauds. Nous vous donnons la valeur des deux

	// premier registres pour vous éviter des complications

	UBRR0H = 0;

	UBRR0L = 0xCF;

	// permettre la reception et la transmission par le UART0

	UCSR0A = _BV(RXC0) | _BV(TXC0);

	UCSR0B = _BV(RXEN0) | _BV(TXEN0) ;

	// Format des trames: 8 bits, 1 stop bits, none parity

	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00) ;

}

// procédure qui envoie un octet de la carte vers le PC

void transmissionUART ( uint8_t donnee ) {
	/* Wait for empty transmit buffer */
   while ( !( UCSR0A & (1<<UDRE1)) );
   /* Put data into buffer, sends the data */
   UDR0 = donnee;
}


#define K 16.924656
#define P -0.85223

#define A 0.0368674 //obtenue en faisant un regretion puis une courbe de tendance
#define B 1.211909

#define S 31467
#define E -1.055

#define D 0.0504
#define Z -0.0031

can convAN;
int main() {

	//Configuration des ports
	DDRA = 0x00; // PORT A est en mode entree
	DDRB = 0xff; // PORT B est en mode sortie
	DDRC = 0xff; // PORT C est en mode sortie
	DDRD = 0xff; // PORT D est en mode sortie



		Memoire24CXXX memoire = Memoire24CXXX();
		//distanceAdroite
		uint16_t adresse = 0x00;

		//CartographierLigne(20.3, 22.4, adresse);
		//adresse += 192;
		//CartographierLigne(40.3, 12.4, adresse);
		initialisationUART ();
		uint8_t ligne[193];
	for(uint8_t i = 0; i < 96; i++)
	{
		/*uint16_t lecture = convAN.lecture(0);
		double V = (3.0 * lecture) / 1024.0;
		double a_retourner = 0.0;
		a_retourner = (1/((D*V)-Z));
		*/
		double a_retourner = 0;
		if(i< 10) 
		{
			a_retourner = 8;
		}
		else {
			if (i<55)
			{
				a_retourner = i;
			}
			else {
				a_retourner = 60;
			}
		}
		adresse = 193*i;
		PORTB = 0b10;
		//CartographierLigne(a_retourner, a_retourner, adresse);
		CartographierLigne2(a_retourner, a_retourner, adresse);
		PORTB =0b01;
	}
	for (uint16_t i = 0; i < (96*sizeof(ligne)); i++) {
		PORTB = 0b10;
		_delay_ms(5);
		PORTB = 0b01;
		_delay_ms(10);
		uint8_t ligne[1];
		memoire.lecture(i, ligne);
		transmissionUART (ligne[0] );
	}
		PORTB = 0b01;

	return 0;
}
