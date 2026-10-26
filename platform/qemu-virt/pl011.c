/* PL011 UART driver for QEMU virt: implements platform_putc(). */
#include "platform.h"
#include "types.h"

/* PL011 base on QEMU virt (DTB node pl011@9000000). Used as a pointer
 * directly: only valid while the MMU is off (identity mapping). */
static const phys_addr_t PL011_BASE = 0x09000000;

/* Register byte offsets, from the PL011 TRM (ARM DDI 0183). */
static const phys_addr_t PL011_UARTDR = 0x000; /* Data register */
static const phys_addr_t PL011_UARTFR = 0x018; /* Flag register */

/* UARTFR bits. */
static const uint32_t PL011_UARTFR_TXFF = 1u << 5; /* Transmit FIFO full */

/* Pointer to the PL011 register at byte offset off. */
static volatile uint32_t *pl011_reg(phys_addr_t off)
{
    return (volatile uint32_t *)(PL011_BASE + off);
}

void platform_putc(char c)
{
    /* Wait until the UART is ready to accept a character. */
    while (*pl011_reg(PL011_UARTFR) & PL011_UARTFR_TXFF) {
    }

    /* Only the low 8 bits of UARTDR are sent. */
    pl011_reg(PL011_UARTDR)[0] = (uint8_t)c;
}
