/* Generic kernel entry: first C code run after the boot assembly. */
#include "kernel.h"
#include "platform.h"

/* Lives in .bss: lets GDB check that _start zeroed it. */
static volatile uint64_t test_var;

_Noreturn void kernel_main(phys_addr_t dtb_phys)
{
    platform_putc('S');
    platform_putc('A');
    platform_putc('L');
    platform_putc('A');
    platform_putc('R');

    (void)test_var;
    (void)dtb_phys; /* Device tree parsing comes in M1. */

    for (;;) {
    }
}
