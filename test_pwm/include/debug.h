#include <stdint.h>
#include <string.h>


class Debug{
public:
    Debug(){};
    void print_debug_entier8bits(uint8_t x);
    void print_debug_string(char* x);
    void print_info(char* file, int line);
private:
};




