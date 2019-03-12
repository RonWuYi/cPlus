//
// Created by hdc on 12/28/17.
//
#include "sales.h"

Sales::bad_index::bad_index(int ix, const char *s)
:std::logic_error(s), bi(ix)
{

}

Sales::Sales(int yy)
{
    year = yy;
    for (int i = 0; i < MONTHS; i++) {
        gross[i] = 0;
    }
}

Sales::Sales(int yy, const double *gr, int n)
{
    year = yy;
    int lim = (n < MONTHS)?n:MONTHS;
    int i;
    for (i = 0; i < lim; ++i)
    {
       gross[i] = gr[i];
    }

    for ( ; i < MONTHS; ++i) {
        gross[i] = 0;
    }
}
