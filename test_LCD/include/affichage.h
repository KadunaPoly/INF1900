#define F_CPU 8000000UL
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
#define DEMO_DDR	DDRC // `Data Direction Register' AVR occupé par l'aff.
#define DEMO_PORT	PORTC // Port AVR occupé par l'afficheur
class Affichage
{
public: 
    Affichage();
    void afficherNomDuRobot();
    void afficherDistance(double distanceADroite, double distanceAGauche);
private:
    LCM disp;
    
}; 
