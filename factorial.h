#ifndef FACTORIAL_H
#define FACTORIAL_H

double getFactorial(int x)
{
    if (x==0) return 1;
    else return x*getFactorial(x-1);
}

#endif // FACTORIAL_H
