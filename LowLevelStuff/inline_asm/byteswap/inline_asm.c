#include <stdio.h>

int main(void)
{
    unsigned int a = 45;
    unsigned int b = 0;

    asm (
        "bswap %1 \n\t"
        "movl %1, %0 \n\t"
        : "=r"(b)
        : "r"(a)
      //: clobbered regs
        );

    printf("%d\n", b);

    return 0;
}
