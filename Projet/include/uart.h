#include "avr/io.h"
#ifndef UART_H
#define UART_H

class Uart
{
public:
    Uart();
    void transmissionUART(uint8_t donnee);
};

#endif