/* Basic kernel types, shared by C code and Rust bindings. */
#ifndef OSAKER_TYPES_H
#define OSAKER_TYPES_H

#include <stdint.h>

/* Physical address: an integer, not a pointer; it cannot be dereferenced
 * once the MMU is on. */
typedef uint64_t phys_addr_t;

#endif
