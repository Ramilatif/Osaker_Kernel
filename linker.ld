/* Load address on QEMU virt: see docs/adr/0001. */
KERNEL_BASE = 0x40200000;

/* Sets the ELF e_entry field: QEMU jumps here. */
ENTRY(_start)

SECTIONS
{
  . = KERNEL_BASE;

  /* _start first: a raw binary starts at the image's first byte. */
  .text : {
    KEEP(*(.text.boot))
    *(.text .text.*)
  }

  .data : {
    *(.data .data.*)
  }

  .bss : {
    *(.bss .bss.*)
  }
}
