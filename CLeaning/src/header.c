//
// Created by hdc on 1/11/19.
//
#include <stdio.h>
#include "header.h"

int get_line(char s[], int lim){
    int c, i, l;

    for (i = 0, l = 0; (c = getchar())!=EOF && c!='\n' ; ++i) {
        if (i < lim -1)
        s[l++] = c;
    }
    if (c == '\n' && l < lim - 1)
        s[l++] = c;
    s[l] = '\0';

    return l;
}

void reverse(char s[])
{
    int j, i;

    int tmp;

    for (j = 0; s[j] != '\0' ; ++j) {
        ;
    }
    --j;
    if (s[j - 1] == '\n')
        --j;

    for (i = 0; i < j; ++i, --j) {
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }
}

