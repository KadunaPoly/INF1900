#include "debug.h"
#include "transmissionUART.h"
#include <string.h>
#include <stdio.h>



void Debug::print_debug_entier8bits(uint8_t x) {
    TransmissionUART t;
    t.transmissionUART_entier(x);
}

void Debug::print_debug_string(char* x) {
    TransmissionUART t;
    t.transmissionUART_string(x);
}

void Debug::print_info(char* file, int line){
    TransmissionUART t;
    t.transmissionUART_info(file, line);
}


