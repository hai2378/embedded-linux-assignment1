# Embedded Linux — Assignment 01

**How to Write a Device Driver** · Linux 5.15 · ARMv7 (Cortex-A9) · QEMU `vexpress-a9`

This repository organizes the **original Assignment 01 source code, ARM binaries, test transcripts and technical report** from the saved assignment archive. The materials are preserved for reference; the GitHub package preparation has **not rerun QEMU or rebuilt the modules**.

## Project overview

| Component | Description |
|---|---|
| Task 1: `asgn1_driver` | Character device at `/dev/asgn1` (major 241), 256-byte buffer, mutex and four ioctl commands |
| Task 2: `counter_driver` | Misc character device at `/dev/counter` (major 10), atomic counter and `/proc/counter_info` |
| Target | Linux Kernel `5.15.0`, ARM EABI5, BusyBox initramfs on QEMU ARM Cortex-A9 |
| Evidence | Original test transcripts dated **22 October 2026**, plus the existing 12-page report PDF |

## Architecture

```mermaid
flowchart TD
    U[Userspace shell and ioctl client] --> V[Linux system calls / VFS]
    V --> A[asgn1_driver: file_operations]
    V --> C[counter_driver: miscdevice]
    A --> I[unlocked_ioctl: RESET / STATS / MODE / VERSION]
    A --> B[256-byte buffer + mutex]
    C --> T[atomic_t counter and stats]
    C --> P[/proc/counter_info]
```

## Repository layout

```text
embedded-linux-assignment1/
├── task1_ioctl/
│   ├── asgn1_driver.c
│   ├── asgn1_ioctl.h
│   ├── test_ioctl.c
│   ├── test_ioctl               # ARM executable from original archive
│   ├── test_asgn1.sh
│   ├── asgn1_driver.ko          # Original ARMv7 module
│   └── Makefile
├── task2_misc/
│   ├── counter_driver.c
│   ├── test_counter.sh
│   ├── counter_driver.ko        # Original ARMv7 module
│   └── Makefile
├── logs/
│   ├── test_asgn1_output.txt
│   └── test_counter_output.txt
├── bao_cao/
│   └── Bao_cao_Assignment_01_Embedded_Linux.pdf
├── docs/
│   ├── TEST_RESULTS.md
│   └── SUBMISSION_CHECKLIST.md
├── .gitignore
└── README.md
```

## ioctl interface (Task 1)

| Command | Behavior |
|---|---|
| `ASGN1_RESET_BUFFER` | Clears the buffer and resets read/write counters |
| `ASGN1_GET_STATS` | Returns open/write/read counts, buffer length and last write size |
| `ASGN1_SET_MODE` | `0` adds `[KERNEL]`; `1` writes data unchanged (default) |
| `ASGN1_GET_VERSION` | Returns a version string (up to 64 bytes) |

The kernel and userspace programs share `task1_ioctl/asgn1_ioctl.h`.

## Recorded test results

The provided **original** logs show the following excerpts:

```text
Task 1:
Read: Hello Linux
Version: asgn1_driver v1.0
Read: [KERNEL]Kernel Test
Buffer bytes after reset: 0
===== TEST FINISHED =====

Task 2:
Counter: 0
Counter: 1
Counter: 2
Read: 100
Read after reset: 0
PASS: negative rejected
PASS: text rejected
===== TEST FINISHED =====
```

Full terminal transcripts are stored unchanged in [`logs/`](./logs/). See [test interpretation and limitations](./docs/TEST_RESULTS.md).

## Rebuilding on Ubuntu (not Windows)

The `.ko` files are for the **ARM target**, not the Ubuntu x86-64 host. Use the same Linux 5.15 kernel tree and cross-toolchain used for the original assignment.

```bash
# Task 1: the original Makefile's 'arm' target needs an explicit ARM KDIR
cd task1_ioctl
make arm KDIR="$HOME/embedded_lab1/kernel/linux-5.15"

# Task 2: the original Makefile already selects the ARM compiler
cd ../task2_misc
make
```

To rebuild the ARM userspace ioctl client when needed:

```bash
cd task1_ioctl
arm-linux-gnueabi-gcc -static -o test_ioctl test_ioctl.c
```

The tests assume the required modules and binaries have been copied into their expected locations inside a **QEMU guest rootfs**. See the scripts themselves; do not run them directly on the host.

## Important before course submission

- The report cover still has placeholders for the student name and ID.
- `MODULE_AUTHOR` is still `Student` in the original source and compiled modules. If you update it, **rebuild the modules** to keep sources and binaries consistent.
- A standalone `logs/dmesg_full.txt` was not present in the original archive (relevant `dmesg` entries do appear inside both test transcripts).
- The existing technical report covers the implementation and design, but **does not include the separate Task 4 theory answers**.
- A test for an invalid ioctl and simultaneous-process stress testing are not documented in the captured evidence.

See [`docs/SUBMISSION_CHECKLIST.md`](./docs/SUBMISSION_CHECKLIST.md) before handing this in as a graded assignment.

## Provenance

`task1_ioctl/`, `task2_misc/` and `logs/` come from `assignment01_ho_so.tar.gz` supplied for this conversation; the report PDF was copied from the existing Assignment 01 report file. This GitHub-ready package removes only transient Kbuild object files and adds documentation. The original source code and retained binaries/logs are not claimed as newly tested or fixed.
