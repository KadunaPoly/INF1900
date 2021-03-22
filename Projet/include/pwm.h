
#ifndef PWM_H
#define PWM_H

#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>


class PWM
{
public :
    // Constructeur
    PWM();

    // Destructeur
    ~PWM();

    // Deplacement en ligne droite
    void avancer ( uint8_t rapport );
    void reculer (uint8_t rapport);
    void arreter();

    // Rotation 
    void tournerADroite();
    void tournerAGauche();

    // Ajustement de la vitesse des roues
    void roueDroite (bool direction, uint8_t rapport);
    void roueGauche (bool direction, uint8_t rapport);

    // Ajustement de la direction
    void ajustementADroite ();
    void ajustementAGauche ();

    void ajustementReculerADroite();
    void ajustementReculerAGauche();
};

#endif

