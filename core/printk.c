/* printk: formatted kernel output on top of platform_putc(). */
#include "printk.h"
#include "platform.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

/* Write one character, turning '\n' into "\r\n" for serial terminals. */
static void kputc(char c)
{
    if (c == '\n') {
        platform_putc('\r');
    }
    platform_putc(c);
}

/* Write a NUL-terminated string; a NULL pointer prints "(null)". */
static void kputs(const char *s)
{
    if (s == NULL) {
        kputs("(null)");
        return;
    }
    while (*s != '\0') {
        kputc(*s++);
    }
}

/* Write n in the given base (10 or 16), lowercase digits, no prefix. */
static void kput_uint(unsigned long n, unsigned int base)
{
    char buf[20]; /* 2^64 - 1 has 20 decimal digits. */
    int i = 0;

    /* Digits come out least significant first: store, then print reversed. */
    do {
        unsigned int digit = n % base;
        buf[i++] = (char)(digit < 10 ? '0' + digit : 'a' + digit - 10);
        n /= base;
    } while (n > 0);

    for (int j = i - 1; j >= 0; j--) {
        kputc(buf[j]);
    }
}

void printk(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    for (const char *p = fmt; *p != '\0'; p++) {
        if (*p != '%') {
            kputc(*p);
            continue;
        }
        /* A lone '%' at the end: print it, never step past the NUL. */
        if (p[1] == '\0') {
            kputc('%');
            break;
        }
        p++;

        switch (*p) {
        case 'c': {
            char c = (char)va_arg(args, int); /* char is promoted to int */
            kputc(c);
            break;
        }
        case 's': {
            const char *s = va_arg(args, const char *);
            kputs(s);
            break;
        }
        case 'd': {
            int n = va_arg(args, int);
            if (n < 0) {
                /* Negate in 64 bits: -INT_MIN does not fit in an int. */
                kputc('-');
                kput_uint(-(long)n, 10);
            } else {
                kput_uint(n, 10);
            }
            break;
        }
        case 'u': {
            unsigned int n = va_arg(args, unsigned int);
            kput_uint(n, 10);
            break;
        }
        case 'x': {
            unsigned int n = va_arg(args, unsigned int);
            kput_uint(n, 16);
            break;
        }
        case 'p': {
            void *ptr = va_arg(args, void *);
            kputs("0x");
            kput_uint((uintptr_t)ptr, 16);
            break;
        }
        case 'l': {
            /* Length modifier: %ld, %lu, %lx read a 64-bit long. */
            if (p[1] == 'd') {
                p++;
                long n = va_arg(args, long);
                if (n < 0) {
                    /* Unsigned negation is well defined, even for LONG_MIN. */
                    kputc('-');
                    kput_uint(-(unsigned long)n, 10);
                } else {
                    kput_uint(n, 10);
                }
                break;
            }
            if (p[1] == 'u') {
                p++;
                unsigned long n = va_arg(args, unsigned long);
                kput_uint(n, 10);
                break;
            }
            if (p[1] == 'x') {
                p++;
                unsigned long n = va_arg(args, unsigned long);
                kput_uint(n, 16);
                break;
            }
            /* Unknown conversion after 'l': print it as is. */
            kputc('%');
            kputc('l');
            break;
        }
        case '%':
            kputc('%');
            break;
        default:
            /* Unknown conversion: print it as is. */
            kputc('%');
            kputc(*p);
            break;
        }
    }

    va_end(args);
}
