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

  .rodata : {
    *(.rodata .rodata.*)
  }

  .data : {
    *(.data .data.*)
  }

  .bss : {
    . = ALIGN(16);
    __bss_start = .;
    *(.bss .bss.*)
    . = ALIGN(16);
    __bss_end = .;
  }
  
  .stack (NOLOAD): ALIGN(4096) {

    . += 16k;
    __stack_top = .;
  }
}
