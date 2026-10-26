/* Kernel formatted output on the boot console. */
#ifndef OSAKER_PRINTK_H
#define OSAKER_PRINTK_H

/* printf-like output through platform_putc(): blocking, no buffering.
 * Supported conversions: %c %s %d %u %x %p %%, and %ld %lu %lx for
 * 64-bit values. No width, precision or floating point.
 * The format attribute lets the compiler check arguments against fmt. */
void printk(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

#endif /* OSAKER_PRINTK_H */
