/* Kernel entry point: QEMU jumps here (ELF e_entry). */
    .section .text.boot, "ax"
    .global _start
_start:
    /* Keep the DTB address (x0) in a callee-saved register. */
    mov     x19, x0

    /* Only core 0 (MPIDR_EL1.Aff0 == 0) boots; the others are parked. */
    mrs     x1, mpidr_el1
    and     x1, x1, #0xff
    cbnz    x1, .Lpark

    /* Point sp at the top of the boot stack (it grows down). */
    adrp    x1, __stack_top
    add     x1, x1, :lo12:__stack_top
    mov     sp, x1

    /* TODO: zero .bss and call kernel_main. */

.Lpark:
    /* Sleep until an event, then sleep again. */
    wfe
    b       .Lpark
