#include <stdlib.h>
#include <stdio.h>


unsigned short *reverse_seq(unsigned short num)
{
  if (num == 0) return NULL;
  
  unsigned short *ret = (unsigned short *) malloc(num * sizeof(unsigned short));
  if (ret == NULL) return NULL;
  
  
  for (int i = num - 1; i >= 0; i--) {
    ret[i] = i + 1;  
  }
  
  return ret;
}

int main(void)
{
    unsigned short innum = 8;
    unsigned short *arr = reverse_seq(innum);
    for (int i = innum - 1; i >= 0; i--) {
        printf("%d, ", arr[i]);
    }

    


    return 0;
}