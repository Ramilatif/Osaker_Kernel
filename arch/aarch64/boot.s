/* Kernel entry point: QEMU jumps here (ELF e_entry). */
    .section .text.boot, "ax"
    .global _start
_start:
    /* Park the CPU: sleep until an event, then sleep again. */
1:  wfe
    b 1b
