.PHONY: clean posix all

# CC = gcc
CC = /home/dsl/kernels/rootfs/src-buildroot-pb1176-2011.08/output/host/usr/bin/arm-linux-gcc

all: linux-app-thread.exe

%.exe: %.c
	$(CC) -D_GNU_SOURCE -Wall -Wno-nonnull -O2 -lrt -lpthread -lgcc -Wall -o $@ $^

clean:
	@exec echo -e "\n>> Cleaning... ";
	@find \( -name '*.[oa]' -or -name '*~*' -or -name '*.exe' -or -name 'a.out' \) -print -delete
	@exec echo ">> [OK]"
