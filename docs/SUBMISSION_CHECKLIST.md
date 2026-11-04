# Before submitting Assignment 01

This is an inventory of tasks still requiring the student's verification. It is **not** a substitute for independently completing the assignment, running the tests or following the course academic-integrity rules.

- [ ] Verify the two `*.ko` files and ioctl test program against your final ARMv7 Linux 5.15 QEMU image.
- [ ] Replace `MODULE_AUTHOR("Student")` with your correct author/ID information as required by the course; rebuild both modules.
- [ ] Check Task 1's Makefile: default `make` builds using host KDIR; use `make arm KDIR=...` or adapt as appropriate, then confirm `make clean`.
- [ ] Add standalone `logs/dmesg_full.txt` after both tests if required.
- [ ] Review the code's comment requirements and verify all resources are cleaned up.
- [ ] Capture missing checks such as invalid ioctl and concurrency tests if required.
- [ ] Fill in name and ID on the PDF report title page.
- [ ] Independently complete/review Task 4 theory answers. The existing report explicitly omits them.
- [ ] Confirm assignment reporting format, page count and any additional lecturer requirements.
- [ ] If submitting to LMS, use its required `.tar.gz` naming scheme; this ZIP is for **GitHub browser upload**, not necessarily the final LMS submission.

The original instructions also warn about academic integrity and publishing/copying code. Consider keeping the repository **Private** until the course grading period is complete.
