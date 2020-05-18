#include "gassp72.h"

int m2k(int k, u16 * tabSig);

extern u16 TabSig;

int main(void)
{     
    m2k(24, &TabSig);
        
    while    (1)
        {
        }
}