#include <stdint.h>

extern uint8_t TestFunc(uint16_t temp, uint8_t temp2);

int main (void)
{
    uint8_t temp = 0;
    while(1) {
        temp = TestFunc(0, temp);
    }
    return 0;
}