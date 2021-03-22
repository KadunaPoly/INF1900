#include <stdint.h>
#include <avr/io.h>  
#include <avr/interrupt.h>


class Minuterie
{
public:
    Minuterie();
    void partirMinuterie (uint16_t duree);
};
