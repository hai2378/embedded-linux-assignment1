# Captured test results — Assignment 01

The details below are derived from the original terminal transcripts under `logs/`, dated **22 October 2026**. No new device or kernel testing was performed during preparation of this GitHub repository.

## Task 1: ioctl character driver

| Test | Evidence in original transcript | Status in recorded run |
|---|---|---|
| Module loaded | `asgn1: driver loaded, major=241` | Observed |
| Echo mode | `Read: Hello Linux` | Observed |
| Get version | `Version: asgn1_driver v1.0` | Observed |
| Kernel mode | `Read: [KERNEL]Kernel Test` | Observed |
| Statistics | `Open count: 3`, `Write count: 1`, `Read count: 1` | Observed |
| Reset buffer | `Buffer bytes after reset: 0` | Observed |
| Cleanup | `asgn1: driver unloaded`; `===== TEST FINISHED =====` | Observed |

## Task 2: misc counter driver

| Test | Evidence in original transcript | Status in recorded run |
|---|---|---|
| Auto increment | `Counter: 0`, `Counter: 1`, `Counter: 2` | Observed |
| Set to 100 | `Read: 100` | Observed |
| Reset to 0 | `Read after reset: 0` | Observed |
| Reject negative value | `PASS: negative rejected` | Observed |
| Reject nonnumeric input | `PASS: text rejected` | Observed |
| End of test | `counter: driver unloaded`; `===== TEST FINISHED =====` | Observed |

## Limits of the evidence

- Both transcripts record shell/kernel activity, **not independent full automated grading**.
- A standalone `dmesg_full.txt` log and independent invalid-ioctl/stress-test transcripts were not in the source archive.
- `counter_read` increments before the userspace copy succeeds; an unsuccessful copy could still increment the counter. This behavior was noted in the accompanying technical report.
- `test_asgn1.sh` and `test_counter.sh` need the module files and (for the ioctl test) the test client staged at the paths expected by the scripts in the QEMU rootfs.
