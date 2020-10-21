#include <stdio.h>

int bitset(int a, int bit)
{
    asm (
            "btsl %2, %0"
            : "=r"(a)
            : "0"(a), "r"(bit)
            //:
        );

    return a;
}

int main(void)
{
    int a = 0;
    printf("%d\n", bitset(a, 3));

    return 0;
}
