#define F_CPU 8000000
#include <util/delay.h> 
#include "pwm.h"

int main() {

	//Configuration des ports
	DDRA = 0xff; // PORT A est en mode sortie
	DDRB = 0xff; // PORT B est en mode sortie
	DDRC = 0xff; // PORT C est en mode sortie
	DDRD = 0xff; // PORT D est en mode sortie



	for(;;){
		PWM pwm;
		//pwm.avancer(50);
		//_delay_ms(1000);
		//pwm.arreter();
		//_delay_ms(2000);

//pwm.reculer(100);
//_delay_ms(1000);
//pwm.arreter();
//_delay_ms(2000);
pwm.tournerADroite();
_delay_ms(2000);

//_delay_ms(1000);
//pwm.tournerAGauche();
		//_delay_ms(1000);

	}
	return 0;
}
