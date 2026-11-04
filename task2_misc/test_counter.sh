#!/bin/sh
set -e

MODULE=/lib/modules/5.15.0/extra/counter_driver.ko
DEVICE=/dev/counter

echo "===== COUNTER DRIVER TEST ====="

echo "[1] Load driver"
insmod "$MODULE"

MINOR=$(awk '$2 == "counter" {print $1}' /proc/misc)
[ -n "$MINOR" ]

mknod "$DEVICE" c 10 "$MINOR"

echo "[2] Test auto increment"
for expected in 0 1 2
do
    result=$(cat "$DEVICE")
    echo "Counter: $result"
    [ "$result" = "$expected" ]
done

echo "[3] Stats before write"
cat /proc/counter_info

echo "[4] Set counter to 100"
echo 100 > "$DEVICE"
cat /proc/counter_info

result=$(cat "$DEVICE")
echo "Read: $result"
[ "$result" = "100" ]

echo "[5] Reset counter"
echo 0 > "$DEVICE"
cat /proc/counter_info

result=$(cat "$DEVICE")
echo "Read after reset: $result"
[ "$result" = "0" ]

echo "[6] Test invalid values"
if echo -5 > "$DEVICE"; then
    echo "FAIL: negative accepted"
    exit 1
else
    echo "PASS: negative rejected"
fi

if echo abc > "$DEVICE"; then
    echo "FAIL: text accepted"
    exit 1
else
    echo "PASS: text rejected"
fi

echo "[7] Final statistics"
cat /proc/counter_info

echo "[8] Unload driver"
rmmod counter_driver
rm -f "$DEVICE"

echo "[9] Kernel logs"
dmesg | tail -n 20

echo "===== TEST FINISHED ====="
