#include "debug.h"
#include "transmissionUART.h"
#include <string.h>
#include <stdio.h>



void print_debug(int x, char* file, int line) {

    char a[8];

    snprintf(a, sizeof(a), "%d\n", x);
	for (uint8_t i = 0; i < sizeof(a); i++){
	   	transmissionUART(a[i]);
        }


    char b[100] = "fichier : ";
    for (uint8_t i = 0; i < sizeof(b); i++){
	   	transmissionUART(b[i]);
        }
    snprintf(b, sizeof(b), "%s\n", file);
	for (uint8_t i = 0; i < sizeof(b); i++){
	   	transmissionUART(b[i]);
        }

    char c[10] = "ligne : ";
    for (uint8_t i = 0; i < sizeof(c); i++){
	   	transmissionUART(c[i]);
        }
    snprintf(c, sizeof(c), "%d\n", line);
	for (uint8_t i = 0; i < sizeof(c); i++){
	   	transmissionUART(c[i]);
        }




/*
    std::cout << "fichier : " << file << std::endl;
    std::cout << "ligne : " << line << std::endl;
   */ 
}



void print_UART(char* file, int line){

#ifdef DEBUG
    #define DEBUG_PRINT(x) transmissionUART(x) // ou par RS-232

    #define assert(x) if(!(x)) print_debug(__FILE__, __LINE__)
else
    #define DEBUG_PRINT(x) do {} while (0) // code mort
#endif

}


