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

    /* Zero .bss, 16 bytes at a time. */
    adrp    x1, __bss_start
    add     x1, x1, :lo12:__bss_start
    adrp    x2, __bss_end
    add     x2, x2, :lo12:__bss_end
.Lbss_loop:
    cmp     x1, x2
    b.hs    .Lbss_done
    stp     xzr, xzr, [x1], #16
    b       .Lbss_loop
.Lbss_done:

    /* Enter C with the DTB address as first argument; never returns. */
    mov     x0, x19
    bl      kernel_main

.Lpark:
    /* Sleep until an event, then sleep again. */
    wfe
    b       .Lpark
