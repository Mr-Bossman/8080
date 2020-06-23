#include "delay.h"
extern void delay(double milli_seconds) 
{ 
    double start_time = (double)clock(); 
    while ((double)clock() < start_time + (milli_seconds*(CLOCKS_PER_SEC/1000))); 
} 
