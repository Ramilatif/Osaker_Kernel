/* Platform contract: every platform/<board>/ directory implements these
 * functions, so that core/ never touches board-specific hardware. */
#ifndef OSAKER_PLATFORM_H
#define OSAKER_PLATFORM_H

/* Write one character to the boot console. Busy-waits until the device can
 * accept it: do not call where blocking is not allowed. */
void platform_putc(char c);

#endif
