/* Generic kernel entry: first C code run after the boot assembly. */
#include "kernel.h"
#include "printk.h"

_Noreturn void kernel_main(phys_addr_t dtb_phys)
{
    printk("Osaker_Kernel booting\n");
    printk("d: %d %d %d %d\n", 0, 42, -42, -2147483647 - 1);
    printk("u/x: %u %x\n", 42u, 0xbeefu);
    printk("p: %p\n", (void *)0xffff000040200000);
    printk("s: %s %s, c: %c, pct: 100%%\n", "ok", (const char *)0, 'A');

    (void)dtb_phys; /* Device tree parsing comes in M1. */
    
    for (;;) {
    }
}
