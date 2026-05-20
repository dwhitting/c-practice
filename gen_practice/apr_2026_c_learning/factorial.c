#include "factorial.h"

unsigned int factorial(unsigned int n) {
    unsigned int current = 1;

    if (n != 0) {
        current = n * factorial(n - 1);
    }
    return current;
}