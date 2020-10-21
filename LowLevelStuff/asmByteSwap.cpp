#include <iostream>

using namespace std;

unsigned bswap_dword(unsigned a)
{
    asm (
    "bswap %0 \n\t"
    : "=r"(a)
    : "0"(a)
    //: clobbered regs
    );
    return a;
}
/*
unsigned short bswap_word(unsigned short a)
{
    asm (
    "mov %0, %%ax \n\t"
    "xchg %%al, %%ah \n\t"
    "mov %%ax, %0 \n\t"
    : "=r"(a)
    : "0"(a)
    : "ax"
    );
    return a;
}
*/
unsigned short bswap_word(unsigned short a)
{
    asm (
    "xchg %%al, %%ah \n\t"
    : "=a"(a)
    : "0"(a)
    : 
    );
    return a;
}

int main()
{
    unsigned a = 8;
    unsigned short b = 8;
    cout << bswap_dword(a) << endl;
    cout << bswap_word(b) << endl;

    return 0;
}

