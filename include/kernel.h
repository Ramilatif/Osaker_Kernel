/* Kernel entry points shared between the boot code and the C core. */
#ifndef OSAKER_KERNEL_H
#define OSAKER_KERNEL_H

#include "types.h"

/* Called once by _start (arch/aarch64/boot.s) on the boot core, with a
 * stack set up and .bss zeroed. dtb_phys is the physical address of the
 * device tree, or 0 when the loader passes none (QEMU ELF, see ADR 0001). */
_Noreturn void kernel_main(phys_addr_t dtb_phys);

#endif
