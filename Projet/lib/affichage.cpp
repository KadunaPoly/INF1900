#include <math.h>
#include "affichage.h"


Affichage::Affichage() {}
void Affichage::AfficherNomDuRobot()
{
  //PORTC car LCD brancher en C sur tous les pins
  LCM disp(&DDRC, &PORTC);
  disp.clear();
  disp = "Mr Propre";
}

void Affichage::AfficherDistance(double distanceAGauche, double distanceADroite)
{
  // Ajustement pour afficher distance mesuree a gauche
  //Troncature de la distance grace a Floor importé de math.h (ex: 158.6 -> 158)
  //conversion implicite de double en uint16_t
  uint16_t valEntiereGauche = floor(distanceAGauche);
  //Obtention de la valeur décimale (ex: 158.6 *10 - 158*10 -> 6)
  // uint16_t valeurDecimaleGauche = distanceAGauche*10 - valEntiereGauche *10;

  // Ajustement pour afficher distance mesuree a droite
  //Troncature de la distance grace a Floor importé de math.h (ex: 158.6 -> 158)
  //conversion implicite de double en uint16_t
  uint16_t valEntiereDroite = floor(distanceADroite);
  //Obtention de la valeur décimale (ex: 158.6 *10 - 158*10 -> 6)
  // uint16_t valeurDecimaleDroite = distanceADroite*10 - valEntiereDroite *10;

  //Affichage des valeur sur LCD
  LCM disp(&DDRC, &PORTC);
  disp.clear();
  disp << " Gauche : " << valEntiereGauche;
  //Disp[16] pour passer a la 2eme ligne
  disp[16] = " Droite : ";
  disp << valEntiereDroite;
}