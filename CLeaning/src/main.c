#include <stdio.h>
#include "a.h"

int func(void){
    static int count = 10;
    return count--;
}


int count = 1;
int aa;
int main(void){
    printf("global\t\tlocal static\n");
    for (; count < 10; ++count) {
        printf("%d\t\t\t\t%d\n", count, func());

    }
    int i;
    static char str[10];
    printf("integer: %d; string:(begin)%s(end)", aa, str);
    return 0;
}