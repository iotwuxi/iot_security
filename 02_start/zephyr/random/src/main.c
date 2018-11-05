#include <zephyr.h>
#include <random/rand32.h>

#include <stdio.h>

void main(void)
{
    printf("\n  %s board random:\n", CONFIG_BOARD);
    while (1) {
        printf("  0x%08x\n", sys_rand32_get());
        k_sleep(1000);
    }
}
