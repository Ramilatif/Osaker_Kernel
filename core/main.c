/* Generic kernel entry: first C code run after the boot assembly. */
#include "kernel.h"

/* Lives in .bss: lets GDB check that _start zeroed it. */
static volatile uint64_t test_var;

_Noreturn void kernel_main(phys_addr_t dtb_phys)
{
    (void)test_var;
    (void)dtb_phys; /* Device tree parsing comes in M1. */

    for (;;) {
    }
}
