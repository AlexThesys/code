#include <iostream>
#include <cmath>

using namespace std;

template<typename Width>
Width findNextPow2(Width v)
{
    --v;
    for (uint32_t i = 1, maxShift = sizeof (Width) << 3; i <= maxShift; i = i << 1){
        v |= v >> i;
    }
    ++v;
    return v;
}

template<typename Width>
Width findNextPow2_v2(Width v)
{
    unsigned int i = sizeof (Width) << 3;
    unsigned int mask = 1 << (--i);
    while ((!(v & mask)) && i) {
        mask = mask >> 1;
        --i;
    }
    return ((v & mask) << 1);
}

template<typename Width>
Width findNextPow2_v3(Width v)
{
    switch (sizeof (Width)) {
        case 8 :
            asm (
            "xorq %%rcx, %%rcx \n\t"
            "bsrq %%rax, %%rcx \n\t"
            "movq $1, %%rdx \n\t"
            "shlq %%cl, %%rdx \n\t"
            "andq %%rdx, %%rax  \n\t"
            "shlq $1, %%rax  \n\t"
            : "=a"(v)
            : "0"(v)
            : "rcx", "rdx", "cc"
            );
            break;
        case 4 :
            asm (
            "xorl %%ecx, %%ecx \n\t"
            "bsrl %%eax, %%ecx \n\t"
            "movl $1, %%edx \n\t"
            "shll %%cl, %%edx \n\t"
            "andl %%edx, %%eax  \n\t"
            "shll $1, %%eax  \n\t"
            : "=a"(v)
            : "0"(v)
            : "ecx", "edx", "cc"
            );
            break;
        case 2 :
            asm (
            "xorw %%cx, %%cx \n\t"
            "bsrw %%ax, %%cx \n\t"
            "movw $1, %%dx \n\t"
            "shlw %%cl, %%dx \n\t"
            "andw %%dx, %%ax  \n\t"
            "shlw $1, %%ax  \n\t"
            : "=a"(v)
            : "0"(v)
            : "cx", "dx", "cc"
            );
    }
    return v;
}

int main()
{
    uint32_t y = 93;
    cout << findNextPow2(y) << endl;
    cout << findNextPow2_v2(y) << endl;
    cout << findNextPow2_v3(y) << endl;

    return 0;
}

