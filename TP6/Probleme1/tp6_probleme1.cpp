/**
 * Auteurs : Paul Corrand (1886657) & Simon Zhang (1839892)
 * Numéro d'équipe : #41
 * Nom : TP6 - Problème 1 - Utilisation du bouton-poussoir
 * 
 * Description: Quand le bouton est enfoncé, un compteur qui incrémente 10 fois par seconde est activé. 
	 * Quand le bouton est relâché ou lorsque le compteur atteint 120, la lumière clignote vert pendant 1/2 seconde. 
	 * Ensuite, la carte mère ne fait rien. Puis, deux secondes plus tard, la lumière rouge s'allume. 
	 * Elle devra clignoter (compteur / 2) fois au rythme de 2 fois par seconde. Ensuite, la lumière tourne au vert 
	 * pendant une seconde. Finalement, elle s'éteint et le robot revient à son état original.
 * 
 * Configuration : Nous avons branché PB0 à la borne positive de la DEL libre, et PB1 du PORT B à la borne négative.
 * Le nouveau bouton-poussoir est relié à PD2. Nous avons enlevé le cavalier IntEN pour s'assurer qu'il n'y ait pas de conflit.
 */
 
#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t boutonPoussoir;
volatile int compteur;

const uint8_t GREEN = 0x02;
const uint8_t RED = 0x01;
const uint8_t LIGHT_OFF = 0x00;

void initialisation (void) {

// Empêcher les interruptions
	cli();

	DDRB = 0xFF; // PORT B est en mode sortie. Cela correspond à la DEL.
	DDRD = 0x00; // PORT D est en mode entrée. Cela correspond au nouveau bouton-poussoir sur la breadboard.
  
// Cette procédure ajuste le registre EIMSK de l’ATmega324PA pour permettre les interruptions externes
	EIMSK |= (1 << INT0);

// Il faut sensibiliser les interruptions externes aux changements de niveau du bouton-poussoir en ajustant le registre EICRA
	EICRA |= _BV(ISC00);

// Accepter les interruptions
	sei();
}

ISR (TIMER1_COMPA_vect) {
	compteur++; // Le compteur incrémente lorsque le bouton poussoir est appuyé
}

ISR (INT0_vect) {
	
	_delay_ms(30); // anti-rebond lorsque le bouton est appuyé
	boutonPoussoir = 1;
		  
	if (PIND & 0x04) { // Si le bouton est relâché
		boutonPoussoir = 0;
	}
}

void partirMinuterie (uint16_t duree) {

	TCNT1 = 0 ; // Démarrer le compteur à 0

	OCR1A = duree; // Permet de compter à la valeur de durée

	TCCR1A |= _BV(COM1A0); // Mode CTC du timer 1 avec horloge divisée par 1024

	TCCR1B |= _BV(CS12)| _BV(CS10) | _BV(WGM12); // Mode CTC du timer 1 avec horloge divisée par 1024

	TCCR1C = 0; // Mode CTC du timer 1 avec horloge divisée par 1024

	TIMSK1 |= _BV(OCIE1A); // Timer/Counter1: interruption après que la durée est atteinte par le compteur
}


int main()
{
	initialisation();
	for (;;) {
		compteur = 0;
		if (boutonPoussoir == 1) {
			partirMinuterie(781);	// (8000000/1024/10) : compteur (prescaler de 1024) qui sera incrémenté 10 fois / sec
			
			do {}
			while (compteur < 120 && boutonPoussoir == 1); // Tant que le compteur n'a pas atteint 120 ou que le bouton est pesé, ne rien faire

			TIMSK1 = 0; // Le TIMSK1 arrête de compter
			
			PORTB = GREEN; // La DEL devient verte pour 1/2 seconde
			_delay_ms(500);
			
			PORTB = LIGHT_OFF;   // La DEL est éteinte pendant 2 secondes
			_delay_ms(2000);
			
			// La DEL clignote rouge au rythme de 2 fois par seconde pour (compteur/2) tant de fois.
			for (uint8_t i = 0; i <= compteur/2; i++) {
				PORTB = RED;
				_delay_ms(250);
				PORTB = LIGHT_OFF;
				_delay_ms(250);
			}
			
			PORTB = GREEN; // La DEL devient ainsi verte pendant 1 seconde
			_delay_ms(1000);
			
			PORTB = LIGHT_OFF; // La DEl est éteinte et on peut recommencer la procédure
		}
	}
}
