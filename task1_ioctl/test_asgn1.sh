#!/bin/sh
set -e

MODULE=/lib/modules/5.15.0/extra/asgn1_driver.ko
DEVICE=/dev/asgn1

echo "===== ASSIGNMENT 01 TEST ====="

echo "[1] Loading driver"
insmod "$MODULE"
mknod "$DEVICE" c 241 0

echo "[2] Testing Echo mode"
echo "Hello Linux" > "$DEVICE"
RESULT=$(cat "$DEVICE")
echo "Read: $RESULT"
[ "$RESULT" = "Hello Linux" ]

echo "[3] Testing ioctl commands"
/test_ioctl

echo "[4] Testing Kernel mode"
echo "Kernel Test" > "$DEVICE"
RESULT=$(cat "$DEVICE")
echo "Read: $RESULT"
[ "$RESULT" = "[KERNEL]Kernel Test" ]

echo "[5] Testing Reset"
/test_ioctl
BYTES=$(cat "$DEVICE" | wc -c)
echo "Buffer bytes after reset: $BYTES"
[ "$BYTES" -eq 0 ]

echo "[6] Unloading driver"
rmmod asgn1_driver

echo "[7] Checking kernel logs"
dmesg | tail -n 25

rm -f "$DEVICE"

echo "===== TEST FINISHED ====="
