#include <string.h>
#include <stdio.h>

long long eliminate_unset_bits(const char* number){
    long long l_ret_val = 0;
    unsigned long len = strlen(number); // Cache length for efficiency
  
    for (unsigned long i = 0; i < len; i++) {
        if (number[i] == '1') {
            // Shift left by multiplying by 2, then add the new bit 1
            l_ret_val = (l_ret_val * 2) + 1;
        }
    }
  
    return l_ret_val;
}

int main(void)
{
    // This will now correctly output 7
    printf("%lld\n", eliminate_unset_bits("111")); 
    return 0;
}
