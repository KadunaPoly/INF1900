#include "can.h"
#include <math.h>
#ifndef CAPTUREIR_H
#define CAPTUREIR_H

//DEFINITION de constantes pour les equations des capteurs
// LEFT SENSOR
// Power function
#define A 5669.8
#define B -1.019
// Inverse distance linear function
#define C 0.0002
#define D 0.0018

// RIGHT SENSOR
// Power function
#define E 5688.3
#define F -1.02
// Inverse distance linear function
#define G 0.0002
#define H 0.002

// Position des capteurs gauche et droite
const int pos_left = 6;
const int pos_right = 7;
// Convertisseur analogique global?? I hope it works

class CaptureIR
{
public:
    CaptureIR();

    uint8_t l_distancePower(uint8_t &distanceEntiere, uint8_t &distanceDecimaleGauche, uint16_t &lecture);

    uint8_t l_distanceInverse(uint8_t &distanceEntiere, uint8_t &distanceDecimaleGauche, uint16_t &lecture);

    uint8_t r_distancePower(uint8_t &distanceEntiere, uint8_t &distanceDecimaleDroite, uint16_t &lecture);

    uint8_t r_distanceInverse(uint8_t &distanceEntiere, uint8_t &distanceDecimaleDroite, uint16_t &lecture);


};
#endif