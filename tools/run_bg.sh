#!/bin/sh
gdb_pid=$1
trap '' SIGINT
[ ! -p /tmp/qemu_stdout ] && mkfifo /tmp/qemu_stdout
$QEMU $QEMU_FLAGS -kernel $mos_elf </dev/null -s -S >/tmp/qemu_stdout &
qemu_pid=$!
cat /tmp/qemu_stdout | tee -a .qemu_log &
while kill -0 $gdb_pid 2>/dev/null && kill -0 $qemu_pid 2>/dev/null ; do
    sleep 1;
done
kill -9 $qemu_pid 2>/dev/null;true
[ -f .qemu_log ] && rm .qemu_log
