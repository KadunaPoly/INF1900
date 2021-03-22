#ifndef PIEZO_H
#define PIEZO_H

#include <avr/io.h> 

class Piezo
{
public:
	
	//constructeur
	Piezo();
		
	//destructeur
	~Piezo();
	//activation du son
	void jouerNote(uint8_t note);
		
	//deactivation du son
	void desactiverNote();
	
	//void jouerChanson();

	
private:
};

#endif
