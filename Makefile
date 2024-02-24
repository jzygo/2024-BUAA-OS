ENDIAN         := EL

QEMU           := qemu-system-mipsel
CROSS_COMPILE  := mips-linux-gnu-
CC             := $(CROSS_COMPILE)gcc
CFLAGS         += --std=gnu99 -$(ENDIAN) -G 0 -mno-abicalls -fno-pic \
                  -ffreestanding -fno-stack-protector -fno-builtin \
                  -Wa,-xgot -Wall -mxgot -mno-fix-r4000 -march=4kc -g -ggdb
LD             := $(CROSS_COMPILE)ld
LDFLAGS        += -$(ENDIAN) -G 0 -static -n -nostdlib --fatal-warnings
INCLUDES       := -I./include

qemu_flags     += -cpu 4Kc -m 64 -nographic -M malta -no-reboot

qemu_files     += $(target)

link_script    := linker.lds
target         := runbin

objs           := blib.o machine.o start.o test.o

all: $(objs)
	$(LD) $(LDFLAGS) -o $(target) -N -T $(link_script) $(objs)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

dbg_run: qemu_flags += -s -S
dbg_run: run

dbg:
	make dbg_run >/dev/null 2>&1 &
	gdb-multiarch $(target) -ex "target remote localhost:1234"
	killall $(QEMU)

run:
	$(QEMU) $(qemu_flags) -kernel $(qemu_files)

clean:
	rm -f *.o $(target)

.PHONY: all clean run

