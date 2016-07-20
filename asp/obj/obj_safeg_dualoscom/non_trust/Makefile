.PHONY: clean posix all

FILES = gpos_app.c dualoscom_config.h dualoscom.h dualoscom_types.h dualoscom_user_gpos.c

CC = /home/dsl/kernels/rootfs/src-buildroot-pb1176-2011.08/output/host/usr/bin/arm-linux-gcc

all: gpos_app.exe

gpos_app.exe: $(FILES) dualoscom_user_gpos.o
	$(CC) -Wall -Wno-nonnull -O2 -lrt -lpthread -lgcc -Wall -o gpos_app.exe gpos_app.c dualoscom_user_gpos.o

dualoscom_user_gpos.o:  $(FILES)
	$(CC) -c -Wall -Wno-nonnull -O2 -lrt -lpthread  -Wall -o dualoscom_user_gpos.o dualoscom_user_gpos.c

clean:
	@exec echo -e "\n>> Cleaning... ";
	@find \( -name '*.[oa]' -or -name '*~*' -or -name '*.exe' -or -name 'a.out' \) -print -delete
	@exec echo ">> [OK]"


