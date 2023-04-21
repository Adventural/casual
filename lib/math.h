#ifndef _CASUAL_LIBMATH_H
#define _CASUAL_LIBMATH_H

/* find the min n that 2^n >= x */
int find_n(unsigned long long x);

/* div with unsigned long long */
unsigned long long div_64(unsigned long long a, unsigned long long b);

/* mod with unsigned long long */
unsigned long long mod_64(unsigned long long a, unsigned long long b);

#endif