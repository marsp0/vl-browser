#include <stdio.h>
#include <stdint.h>

int main()
{
    unsigned char buf[] = { 0xd0, 0x96, 0x00 };
    printf("Browser compiled - %s - Ж\n", buf);
}