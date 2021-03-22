#include <customprocs.h>
#include <lcm_so1602dtr_m_fw.h>

#ifndef AFFICHAGE_H
#define AFFICHAGE_H

class Affichage
{
public:
    Affichage();
    void AfficherNomDuRobot();
    void AfficherDistance(double distanceAGauche, double distanceADroite);
};

#endif