#include <stdio.h>
#include <math.h>
#define ECHO(str) (fgets(str, 10, stdin), puts(str))
#define CUBE(x) ((x)*(x)*(x))
#define REM_MOD_4(x) ((x)%4)
#define LESS_THEN_100(x, y) (((x) * (y)) < 100 ? 1 : 0)
#define NELEMS(a) ((int) (sizeof(a) / sizeof(a[0])))
#define DISP(f, x) (printf(#f "(%g) = %g\n", (x), f(x)))

int main(void) {
    //char tchar[10];
    //ECHO(tchar); 

    printf("9 cubed: %d\n", CUBE(9));
    printf("Remainder 14 / 4: %d\n", REM_MOD_4(14));
    printf("Is x*y < 100?: %d\n", LESS_THEN_100(20, 5));
    char a[15];
    printf("Size of array: %d\n", NELEMS(a));
    DISP(cos, 3.0);


    return 0;
}