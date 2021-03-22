#define F_CPU 8000000
#include "avr/io.h"
#include "constantes.h"
#include "memoire_24.h"
#include "piezo.h"
#include "pwm.h"
#include "util/delay.h"
#include <avr/interrupt.h>

#include <stdio.h>
#include <string.h>
#include "captureIR.h"
#include "can.h"
#include <customprocs.h>
#include <lcm_so1602dtr_m_fw.h>
#include <math.h>
#include "uart.h"
#include <affichage.h>


// Fonctions pour cartogrphie et transmission

// Distance en largeur
volatile const double DISTANCE = 60.96;
volatile const int8_t NBCARACTERES = 95;

// Note importante : la distance recue par le capteur est un double
void CartographierLigne(uint8_t distanceADroite, uint8_t distanceAGauche,
                        uint16_t adresse)
{

  Memoire24CXXX memoire = Memoire24CXXX();
  // On a une ligne de 193 caracteres car il y a 192 caracteres de long et un caractere de retour a la ligne
  uint8_t ligne[193];
  // Calcule du nomre de @ de chaque cote
  //utilisation de int16_t  car pas de comparaison possible avec les uint16_t
  int16_t nombreADroite = ((DISTANCE - distanceADroite) * NBCARACTERES) / DISTANCE;
  int16_t nombreAGauche = ((DISTANCE - distanceAGauche) * NBCARACTERES) / DISTANCE;
  int16_t position = 0;
  // Ecriture des caracteres sur la ligne
  //ecriture de la premiere moitié
  for (position = 0; position < nombreAGauche; position++)
  {
    ligne[position] = '@';
  }
  for (position = nombreAGauche; position < NBCARACTERES; position++)
  {
    ligne[position] = ' ';
  }
  ligne[NBCARACTERES] = '|';
  ligne[(NBCARACTERES + 1)] = '|';
  //ecriture de la deuxieme moitié
  for (position = (NBCARACTERES + 2); position < ((NBCARACTERES + 2) + (NBCARACTERES - nombreADroite)); position++)
  {
    ligne[position] = ' ';
  }
  for (position = ((NBCARACTERES + 2) + (NBCARACTERES - nombreADroite)); position < ((NBCARACTERES + 1) * 2); position++)
  {
    ligne[position] = '@';
  }
  //passage à ligne d'affichage suivante
  ligne[((NBCARACTERES + 1) * 2)] = '\n';
  //adresse++;

  memoire.ecriture(adresse, ligne, sizeof(ligne));
}

// Fonction pour suivre la ligne
void SuivreLigne(PWM &pwm)
{
  pwm.avancer(41);
  _delay_us(1);
  pwm.avancer(30);
  if (((PINA & 0b11111) == 0b00100) || ((PINA & 0b11111) == 0b00110) ||
      ((PINA & 0b11111) == 0b01100))
  {
    pwm.avancer(30);
  }

  if (((PINA & 0b11111) == 0b11000) || ((PINA & 0b11111) == 0b10000) ||
      ((PINA & 0b11111) == 0b01000))
  {
    pwm.ajustementADroite();
  }

  if (((PINA & 0b11111) == 0b00011) || ((PINA & 0b11111) == 0b00001) ||
      ((PINA & 0b11111) == 0b00010))
  {
    pwm.ajustementAGauche();
  }
}

int main()
{
  /********************/ //TABLEAU D'INFORMATIONS///**************************/
  /*        Segment 1 = 25cm                                                */
  /*        Segment 2 = 55cm                                                */
  /*        Segment 3 = 40cm                                                */
  /*        Longueur totale = 120cm                                         */
  /*        Proportion segment 1 = 25/120 = 20.83%                          */
  /*        Proportion segment 2 = 55/120 = 45.83%                          */
  /*        Proportion segment 3 = 40/120 = 33.83%                          */
  /*                                                                        */
  /*        Duree segment 1 = 8s                                            */
  /*        Duree segment 2 = 20s                                           */
  /*        Duree segment 3 = 15s                                           */
  /*                                                                        */
  /*        taille tableau 1 = 8 000/40 = 200                               */
  /*        taille tableau 2 = 20 000/40 = 500                              */
  /*        taille tableau 3 = 15 000/40 = 375                              */
  /*                                                                        */
  /*        Nombres de mesures segment 1 = 96*20.83% = 20                   */
  /*        Nombres de mesures segment 2 = 96*45.83% = 44                   */
  /*        Nombres de mesures segment 3 = 96*33.83% = 32                   */
  /*                                                                        */
  /*        0b00001 correspond a DS1                                        */
  /*        0b00010 correspond a DS2                                        */
  /*        0b00100 correspond a DS3                                        */
  /*        0b01000 correspond a DS4                                        */
  /*        0b10000 correspond a DS5                                        */
  /**************************************************************************/

  // Configuration des ports
  DDRA = 0x00; // PORT A est en mode entree
  DDRB = 0xff; // PORT B est en mode sortie
  DDRC = 0xff; // PORT C est en mode sortie
  DDRD = 0xff; // PORT D est en mode sortie
  // Initialisation des variables et des tableaux de mesures

  uint8_t Droite[440] = {};
  uint8_t Gauche[440] = {};
  uint8_t tableauDistancesDroite[98] = {}; //Pose probleme
  uint8_t indexDroite = 0;
  uint8_t tableauDistancesGauche[98] = {};
  uint8_t indexGauche = 0;

  uint32_t compteurDroit = 0;
  uint32_t compteurGauche = 0;

  uint16_t moyenneMesures = 0;
  double distanceDroite = 0;
  double distanceGauche = 0;
  //double distanceTemp = 0.0;
  const uint8_t NBMESURESSEGMENT1 = 20;
  const uint8_t NBMESURESSEGMENT2 = 44;
  const uint8_t NBMESURESSEGMENT3 = 32;

  uint16_t adresse = 0x00;
  bool valeurEnrengistree;

  uint8_t distanceEntiere = 0;
  uint8_t distanceDecimaleDroite = 0;
  uint8_t distanceDecimaleGauche = 0;
  uint16_t lecture = 0;

  //Creation del'Objet PWM
  PWM pwm;

  Memoire24CXXX memoire = Memoire24CXXX();

  Affichage affichage;

  CaptureIR capture;

  Piezo piezo;
  
  Uart uart;
  // Il y a beaucoup d'etats, cela permet de tester directement une partie du
  // parcour La cartographie se fait lors des etats appeles segments L'etat INIT
  // robot affiche nom et attends 3s Etat0 c'est avant qu'il croise la bande
  // noire perpendiculaire Pour l'instant la DEL est en rouge lorsque l'on
  // utilise les capteurs IR
  enum Detection
  {
    etat0,
    INIT,     // Le robot ne fait rien pendant 3 secondes
    segment1, // Le robot parcour le premier segment et prend les distances
    virage1,  // Le robot fait ses deux premiers virage a 90 degres. Il ne prend aucune distance
    reculer1, // Le robot recule pour reprendre ses mesures au bon endroit
    segment2, // Le robot parcour le second segment et prend les distances
    virage2,  // Le robot fait ses deux derniers virage a 90 degres. Il ne prend aucune distance
    reculer2, // Le robot recule pour reprendre ses mesures au bon endroit
    segment3, // Le robot parcour le dernier segment et prend les distances
    fin,      // Le robot est dans son etat final, affiche la distance et est pres a cartographier
  };

  Detection etat = INIT;

  uint8_t nombreRotation = 0;
  uint8_t compteur = 0;

  LCM disp(&DDRC, &PORTC);
  disp.clear();
  valeurEnrengistree = true;
  uint16_t duree = 0;

  for (;;)
  {
    switch (etat)
    {
    case INIT:
      affichage.AfficherNomDuRobot();
      do
      {
        duree += 1;
        _delay_ms(1);
      } while (duree < 3000);

      etat = etat0;
      break;

    case etat0:
      SuivreLigne(pwm);
      if ((PINA & 0b11111) == 0b11111)
      {
        etat = segment1;
        duree = 0;
        compteurDroit = 0;
        compteurGauche = 0;
        compteur = 0;
      }
      break;

    case segment1:
      duree += 1;
      _delay_ms(1);
      PORTB = 0b01;
      pwm.avancer(35);
      // Pour ne pas avoir une duree trop grande, je la remet a 0 des qu'elle atteient 100
      // Toutes les 100 ms on prend une mesure de la distance
      if (duree == 100)
      {
        distanceDroite = (capture.r_distancePower(distanceEntiere, distanceDecimaleDroite, lecture)) - 5;
        distanceGauche = (capture.l_distancePower(distanceEntiere, distanceDecimaleGauche, lecture)) + 14;
        //distanceDroite = 19;
        //distanceDroite = 25;
        Droite[compteurDroit] = distanceDroite;
        Gauche[compteurGauche] = distanceGauche;
        compteurDroit++;
        compteurGauche++;
        compteur++;
        duree = 0;
      }

      SuivreLigne(pwm);

      if ((((PINA & 0b11111) == 0b11100) || ((PINA & 0b11111) == 0b11110)) &&
          compteur >= 25) // compteur >= 25 pour etre sur qu'on ne capte pas mal la
                          // premiere bande et qu'on n'arrete pas la cartographie
      {
        etat = virage1;
        valeurEnrengistree = false;
        nombreRotation = 0;
      }

      break;

    case virage1:
      if (!valeurEnrengistree)
      {
        pwm.arreter();
        // Decremente les compteurs car on les a incremente une fois de trop
        compteurDroit--;
        compteurGauche--;
        // moyenneMesures correspond au nombre de mesures qui va servir a un caractere pour la cartographie
        // Chaque caracteres pour la cartographie va etre emvoye dans le tableau tableauDistancesDroite
        moyenneMesures = floor((compteurDroit) / NBMESURESSEGMENT1);
        distanceDroite = 0;
        for (uint16_t i = 0; i < compteurDroit; i++)
        {
          distanceDroite += (Droite[i]);
          if (i % moyenneMesures == 0)
          {
            // On fait la moyenne des moyennesMesures prise pour obtenir une valeur pour tableauDistancesDroite
            tableauDistancesDroite[indexDroite] =
                (distanceDroite / moyenneMesures);
            indexDroite++;
            distanceDroite = 0;
          }
        }

        // Calculs des longueurs a gauche
        moyenneMesures = floor((compteurGauche) / NBMESURESSEGMENT1);
        distanceGauche = 0;
        for (uint16_t i = 0; i < compteurGauche; i++)
        {
          distanceGauche += (Gauche[i]);
          if (i % moyenneMesures == 0)
          {
            tableauDistancesGauche[indexGauche] =
                (distanceGauche / moyenneMesures);
            indexGauche++;
            distanceGauche = 0;
          }
        }
        valeurEnrengistree = true;
      }
      duree = 0;
      SuivreLigne(pwm);

       if ((PINA & 0b11111) == 0b00000) {
        pwm.avancer(30);
        _delay_ms(600);
        if ((PINA & 0b11111) == 0b00000) {
          pwm.reculer(45);
          _delay_ms(1);
          pwm.reculer(30);
          _delay_ms(700);
          if (nombreRotation == 0) {
            while ((PINA & 0b11111) != 0b00100) {
              pwm.roueDroite(false, 47);
              pwm.roueGauche(false, 0);
            }
          } else {
            // On avance pour faire un virage a 90 degres qui tombe sur la ligne noir
            pwm.avancer(45); //was 45
            _delay_ms(1850);
            while ((PINA & 0b11111) != 0b00100) {
              pwm.roueDroite(false, 0);
              pwm.roueGauche(false, 47);
            }
          }
          nombreRotation++;
        }
      }

      if (nombreRotation == 2) {
        pwm.arreter();
        _delay_ms(500);
        etat = reculer1;
        compteur = 0;
      }


      break;

    case reculer1:
      pwm.reculer(45);
      if (((PINA & 0b11111) == 0b00111) || ((PINA & 0b11111) == 0b01111) ||
          ((PINA & 0b11111) == 0b11111) || ((PINA & 0b11111) == 0b00000))
      {
        pwm.arreter();
        _delay_ms(500);
        etat = segment2;
        duree = 0;
        compteurDroit = 0;
        compteurGauche = 0;
        compteur = 0;
        duree = 0;
      }

      break;

    case segment2:
      duree += 1;
      _delay_ms(1);
      // pwm.avancer(35);
      if (duree == 100)
      {
        distanceDroite = capture.r_distancePower(distanceEntiere, distanceDecimaleDroite, lecture) + 14;
        distanceGauche = capture.l_distancePower(distanceEntiere, distanceDecimaleGauche, lecture) - 5;
        //distanceDroite = 19;
        //distanceGauche = 25;
        Droite[compteurDroit] = distanceDroite;
        Gauche[compteurGauche] = distanceGauche;
        compteurDroit++;
        compteurGauche++;
        duree = 0;
        compteur++;
      }
      if ((PINA & 0b11111) == 0)
      {
        pwm.roueGauche(false, 20);
      }
      SuivreLigne(pwm);
      if ((((PINA & 0b11111) == 0b00111) || ((PINA & 0b11111) == 0b01111)) &&
          compteur > 25) // DS3 DS4 DS5
      {
        // arreterCalculerLongueur()
        // arreterCartographie()
        etat = virage2;
        nombreRotation = 0;
        pwm.arreter();
        valeurEnrengistree = false;
      }

      break;

    case virage2:

      if (!valeurEnrengistree)
      {
        pwm.arreter();
        compteurDroit--;
        compteurGauche--;

        // Calculs des longueurs a droite
        moyenneMesures = (compteurDroit) / NBMESURESSEGMENT2;
        distanceDroite = 0;
        for (uint16_t i = 0; i < compteurDroit; i++)
        {
          distanceDroite += (Droite[i]);
          if (i % moyenneMesures == 0)
          {
            tableauDistancesDroite[indexDroite] =
                (distanceDroite / moyenneMesures);
            indexDroite++;
            distanceDroite = 0;
          }
        }

        // Calculs des longueurs a gauche
        moyenneMesures = (compteurGauche) / NBMESURESSEGMENT2;
        distanceGauche = 0;
        for (uint16_t i = 0; i < compteurGauche; i++)
        {
          distanceGauche += (Gauche[i]);
          if (i % moyenneMesures == 0)
          {
            tableauDistancesGauche[indexGauche] =
                (distanceGauche / moyenneMesures);
            indexGauche++;
            distanceGauche = 0;
          }
        }
        valeurEnrengistree = true;
      }
      SuivreLigne(pwm);

      if ((PINA & 0b11111) == 0b00000) {
        pwm.avancer(30);
        _delay_ms(600);
        if ((PINA & 0b11111) == 0b00000) {
          pwm.reculer(45);
          _delay_ms(1);
          pwm.reculer(30);
          _delay_ms(700);
          if (nombreRotation == 0) {
            while ((PINA & 0b11111) != 0b00100) {
              pwm.roueDroite(false, 0);
              pwm.roueGauche(false, 47);
            }
          } else {
            pwm.avancer(45);
            _delay_ms(2000);
            while ((PINA & 0b11111) != 0b00100) {
              pwm.roueDroite(false, 47);
              pwm.roueGauche(false, 0);
            }
          }
          nombreRotation++;
        }
      }

      if (nombreRotation == 2) {
        etat = reculer2;
      }
      break;

    case reculer2:

      pwm.reculer(45);

      if (((PINA & 0b11111) == 0b11100) || ((PINA & 0b11111) == 0b11110) ||
          ((PINA & 0b11111) == 0b11111) || ((PINA & 0b11111) == 0b00000))
      {
        etat = segment3;
        duree = 0;
        compteurDroit = 0;
        compteurGauche = 0;
        duree = 0;
      }

      break;

    case segment3:
      duree += 1;
      _delay_ms(1);
      // pwm.avancer(35);
      if (duree == 100)
      {
        distanceDroite = capture.r_distancePower(distanceEntiere, distanceDecimaleDroite, lecture) - 5;
        distanceGauche = capture.l_distancePower(distanceEntiere, distanceDecimaleGauche, lecture) + 14;
        //distanceDroite = 19;
        //distanceDroite = 25;
        Droite[compteurDroit] = distanceDroite;
        Gauche[compteurGauche] = distanceGauche;
        compteurDroit++;
        compteurGauche++;
        duree = 0;
      }

      SuivreLigne(pwm);
      duree += 1;
      _delay_ms(1);

      if ((((PINA & 0b11111) == 0b11111)) && (duree >= 1000))
      {
        etat = fin;
        pwm.arreter();
        adresse = 0;
        adresse++;
        valeurEnrengistree = false;
      }

      break;

    case fin:

       // Partie cartographie

      if (!valeurEnrengistree) 
      {
        pwm.arreter();
        compteurDroit--;
        compteurGauche--;
        moyenneMesures = (compteurDroit) / NBMESURESSEGMENT3;
        distanceDroite = 0;

        for (uint16_t i = 0; ((i < compteurDroit)&&(indexDroite<96)); i++) 
        {

          distanceDroite += Droite[i];
          if (indexDroite > 95)
          {
            break;
          }
          if (i % moyenneMesures == 0) {

            tableauDistancesDroite[indexDroite] =
                (distanceDroite / moyenneMesures);
            indexDroite++;
            distanceDroite = 0;
          }
        }
       

        // Calculs des longueurs a gauche
        moyenneMesures = (compteurGauche) / NBMESURESSEGMENT3;
        distanceGauche = 0;
        for (uint16_t i = 0; ((i < compteurGauche)&&(indexGauche<96)); i++) {

          distanceGauche += (Gauche[i]);
          if (indexGauche > 95)
          {
            break;
          }
          if (i % moyenneMesures == 0) {
            tableauDistancesGauche[indexGauche] =
                (distanceGauche / moyenneMesures);
            indexGauche++;
            distanceGauche = 0;
          }
        }
        valeurEnrengistree = true;
      }
      pwm.arreter();

      // Calculs des longueurs a gauche
      distanceGauche = 0;
      distanceDroite = 0;
      for (uint8_t i = 0; i < 95; i++) {
        //distanceTemp = 1.25 * 1.25;
        distanceGauche +=
            sqrt((tableauDistancesGauche[i] - tableauDistancesGauche[i + 1]) *
            (tableauDistancesGauche[i] - tableauDistancesGauche[i + 1]) + 1.5627);
        //distanceGauche += pow(distanceTemp,0.5);
    
       // distanceTemp = 1.25 * 1.25;
        distanceDroite +=
            sqrt((tableauDistancesDroite[i] - tableauDistancesDroite[i + 1]) *
            (tableauDistancesDroite[i] - tableauDistancesDroite[i + 1]) + 1.5627);
        //distanceDroite += pow(distanceTemp,0.5);
      }
      affichage.AfficherDistance(distanceGauche, distanceDroite);

      valeurEnrengistree = false;
      adresse = 0;
      uint8_t ligne[193];
      for (;;) {
		  
      DDRD = 0x03; 

        if (PIND & 0b100) {
          if(!valeurEnrengistree)
          {
            for (uint8_t i = 0; i < 96; i++) {
              adresse = 193 * i;
              CartographierLigne(tableauDistancesDroite[i], tableauDistancesGauche[i],
                                adresse);
              valeurEnrengistree = true;
            }
          }
          duree = 0;
          for (uint16_t i = 0; i < (96 * sizeof(ligne)); i++) {
            //AfficherDistance(distanceGauche, distanceDroite);
            if (duree<=25)
            {
              PORTB = 0b10;
            }
            else
            {
              PORTB=0;
            }
            if(duree >=50)
            {
              duree = 0;
            }
            memoire.lecture(i, ligne, sizeof(ligne));
            uart.transmissionUART (ligne[0] );
            duree +=1;
            _delay_ms(1);
          }
          piezo.setup();
          
        }
        if(valeurEnrengistree)
        {
         // piezo.jouerChanson();
          PORTB = 0b10;
          _delay_ms(10);
          PORTB = 0b01;
          _delay_ms(5);
        }
      }
      break;
    }
  }
  return 0;
}
