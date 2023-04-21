#include "math.h"

int find_n(unsigned long long x)
{
    int i;
    for(i = 0; i < 63; i++){
        unsigned long long y = (0x1ULL << i);
        if (y >= x)
            return i;
    }
    return 64;
}

/* how many b in a */
unsigned long long div_64(unsigned long long a, unsigned long long b)
{
    int a_n, b_n;
    unsigned long long res;

    res = 0;
    /* do div */
    while(a >= b){
        /* a: [2^(a_n-1), 2^a_n] */
        a_n = find_n(a);
        /* b: [2^(b_n-1), 2^b_n] */
        b_n = find_n(b);
        
        int estimate = a_n - 1 - b_n;
        if(estimate <= 0){
            a = a - b;
            res += 1;
        }
        else{
            a = a - (b << estimate);
            res += (0x1ULL << estimate);
        }
    }

    return res;
}

unsigned long long mod_64(unsigned long long a, unsigned long long b)
{
    unsigned long long res = div_64(a, b);

    return a - (b * res);
}