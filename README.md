# Osaker_Kernel

An operating system kernel for IoT, written from scratch in C and Rust,
targeting 64-bit ARM processors (AArch64).

## Goals

- Real-time: fixed-priority preemptive scheduler, priority inheritance
- Security: memory isolation, ARM hardware mitigations
- Connectivity: Ethernet and a minimal UDP/IP stack
- Low power: CPU idle sleep

## Platforms

- QEMU `virt` (AArch64): development and CI
- STM32MP257F-DK (Cortex-A35 + Cortex-M33): hardware target

## Layout

| Directory | Purpose |
| --- | --- |
| `arch/aarch64/` | CPU-specific code: boot, exceptions, MMU |
| `platform/` | Board-specific code |
| `core/`, `sched/`, `fs/` | Kernel core (C) |
| `mm/`, `drivers/`, `net/` | Memory, drivers and networking (Rust) |
| `include/` | C headers, the C/Rust interface contract |
| `user/` | Userspace |
| `firmware/m33/` | Real-time firmware for the Cortex-M33 |
| `docs/` | Architecture and decision records (ADR) |

## Status

Work in progress — milestone M0 (boot on QEMU).
