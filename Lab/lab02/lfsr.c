#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"

void lfsr_calculate(uint16_t *reg)
{
    /* YOUR CODE HERE */
    int front = 0;
    front = (*reg & 1) ^ ((*reg >> 2) & 1);
    front ^= ((*reg >> 3) & 1);
    front ^= ((*reg >> 5) & 1);

    *reg >>= 1;
    *reg &= ~(1 << 15);
    if (front)
        *reg ^= (1 << 15);
}
