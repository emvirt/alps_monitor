#
#  SafeG Global Makefile
#
# Usage:
#    make OBJ=obj_safeg_xxxx [VARIABLE=value] [clean] [showtargets] [showsupport]
#
#    VARIABLES:
#        OBJ=application-directory-with-trust/-and-non-trust/-dirs)
#        [TOOLPREFIX=prefix-of-arm-toolchain]
#        [OBJDIR=path-to-asp-obj-dir]
#        [BINDIR=path-to-export-safeg-binaries]
#        [BUILD=debug or optimized]
#        [TRUST=asp or fmp]
#        [NONTRUST=linux or asp or fmp]
#        [UBOOTBIN=uboot-binary-file]
#        [BOARD=BOARDNAME]

SAFEG_PB1176_OBJ          = obj_safeg_btask
SAFEG_PB1176_BINDIR       = safeg
SAFEG_PB1176_BUILD        = debug
SAFEG_PB1176_TRUST        = asp
SAFEG_PB1176_NONTRUST     = asp
SAFEG_PB1176_OBJDIR       = asp/obj
SAFEG_PB1176_TOOLPREFIX   = arm-none-eabi-
SAFEG_PB1176_UBOOTBIN     = res/prebuilt/board_pb1176/prebuilt-pb1176-nt-uboot
SAFEG_PB1176_CPU          = arm1176jz-s

SAFEG_IDEA6410_OBJ        = obj_safeg_idea
SAFEG_IDEA6410_BINDIR     = safeg
SAFEG_IDEA6410_BUILD      = debug
SAFEG_IDEA6410_TRUST      = asp
SAFEG_IDEA6410_NONTRUST   = asp
SAFEG_IDEA6410_OBJDIR     = asp/obj
SAFEG_IDEA6410_TOOLPREFIX = arm-none-eabi-
SAFEG_IDEA6410_UBOOTBIN   = res/prebuilt/board_idea6410/prebuilt-idea6410-nt-uboot
SAFEG_IDEA6410_CPU        = arm1176jz-s

SAFEG_MINI6410_OBJ        = obj_safeg_mini
SAFEG_MINI6410_BINDIR     = safeg
SAFEG_MINI6410_BUILD      = debug
SAFEG_MINI6410_TRUST      = asp
SAFEG_MINI6410_NONTRUST   = asp
SAFEG_MINI6410_OBJDIR     = asp/obj
SAFEG_MINI6410_TOOLPREFIX = arm-none-eabi-
SAFEG_MINI6410_UBOOTBIN   = res/prebuilt/board_mini6410/prebuilt-mini6410-nt-uboot
SAFEG_MINI6410_CPU        = arm1176jz-s

SAFEG_KZMCA9_OBJ        = obj_safeg_btask
SAFEG_KZMCA9_BINDIR     = safeg
SAFEG_KZMCA9_BUILD      = debug
SAFEG_KZMCA9_TRUST      = fmp
SAFEG_KZMCA9_NONTRUST   = fmp
SAFEG_KZMCA9_OBJDIR     = fmp/obj
SAFEG_KZMCA9_TOOLPREFIX = arm-none-eabi-
SAFEG_KZMCA9_UBOOTBIN   = res/prebuilt/board_kzmca9/prebuilt-kzmca9-nt-uboot
SAFEG_KZMCA9_CPU        = cortex-a9

BOARD      ?= PB1176
OBJ        ?= ${SAFEG_${BOARD}_OBJ}
BINDIR     ?= ${SAFEG_${BOARD}_BINDIR}
BUILD      ?= ${SAFEG_${BOARD}_BUILD}
TRUST      ?= ${SAFEG_${BOARD}_TRUST}
NONTRUST   ?= ${SAFEG_${BOARD}_NONTRUST}
OBJDIR     ?= ${SAFEG_${BOARD}_OBJDIR}
TOOLPREFIX ?= ${SAFEG_${BOARD}_TOOLPREFIX}
UBOOTBIN   ?= ${SAFEG_${BOARD}_UBOOTBIN}

BIN     = $(BINDIR)/trust.elf $(BINDIR)/non-trust.elf $(BINDIR)/safeg.elf

CC      = $(TOOLPREFIX)gcc
CXX     = $(TOOLPREFIX)cpp
AS      = $(TOOLPREFIX)as
LD      = $(TOOLPREFIX)ld
OBJCOPY = $(TOOLPREFIX)objcopy
READELF = $(TOOLPREFIX)readelf
NM      = $(TOOLPREFIX)nm

CPU      = ${SAFEG_${BOARD}_CPU}
OBJFLAGS = -O srec --srec-forceS3 --srec-len 32
CFLAGS   = -DSAFEG_${BOARD} -mcpu=$(CPU) -msoft-float -Wall -Wextra -Werror -I .
ASFLAGS  = -mfloat-abi=soft --fatal-warnings
LDFLAGS  = -mcpu=$(CPU) -msoft-float -static -nostdlib -lgcc

ifeq ($(BUILD),debug)
	CFLAGS  += -g -O0
	ASFLAGS += -g
else
	CFLAGS += -O2
endif

export CC CXX AS OBJCOPY OBJFLAGS CFLAGS LDFLAGS ASFLAGS CPU

.PHONY: help all clean

all: $(BIN) $(BINDIR)/safeg.sym

$(BINDIR)/safeg.elf: $(BINDIR)/trust.elf $(BINDIR)/non-trust.elf safeg/entry.h
	cd safeg; $(MAKE)
ifneq ($(BINDIR),safeg)
	mv safeg/safeg.elf $(BINDIR)/safeg.elf
	mv safeg/safeg.bin $(BINDIR)/safeg.bin
endif

$(BINDIR)/trust.elf:
	(cd $(OBJDIR)/$(OBJ)/trust; $(MAKE) SAFEG=enable)
	cp $(OBJDIR)/$(OBJ)/trust/$(TRUST).axf $(BINDIR)/trust.elf
	cp $(OBJDIR)/$(OBJ)/trust/$(TRUST).bin $(BINDIR)/trust.bin

$(BINDIR)/non-trust.elf:
ifeq ($(NONTRUST),linux)
	cp $(UBOOTBIN).elf $(BINDIR)/non-trust.elf
	cp $(UBOOTBIN).bin $(BINDIR)/non-trust.bin
else
$(BINDIR)/non-trust.elf:
	(cd $(OBJDIR)/$(OBJ)/non_trust/;$(MAKE) SAFEG=enable)
	cp $(OBJDIR)/$(OBJ)/non_trust/$(NONTRUST).axf $(BINDIR)/non-trust.elf
	cp $(OBJDIR)/$(OBJ)/non_trust/$(NONTRUST).bin $(BINDIR)/non-trust.bin
endif

$(BINDIR)/safeg.sym: $(BIN)
	$(NM) $(BINDIR)/safeg.elf > $(BINDIR)/safeg.sym
	$(NM) $(BINDIR)/trust.elf >> $(BINDIR)/safeg.sym
	$(NM) $(BINDIR)/non-trust.elf >> $(BINDIR)/safeg.sym

safeg/entry.h: $(BINDIR)/trust.elf $(BINDIR)/non-trust.elf
	$(READELF) -h $(BINDIR)/trust.elf | grep "Entry point address" | gawk '{print "#define secure_start_address\t"$$4}' > safeg/entry.h
	$(READELF) -h $(BINDIR)/non-trust.elf | grep "Entry point address" | gawk '{print "#define normal_start_address\t"$$4}' >> safeg/entry.h

help:
	@exec echo -e "\n\tUsage:";
	@exec echo -e "\t   make OBJ=obj_safeg_xxxx [VARIABLE=value] [clean] [showtargets] [showsupport]";
	@exec echo -e "\t   VARIABLES:";
	@exec echo -e "\t       OBJ=application-directory-with-trust/-and-non-trust/-dirs)";
	@exec echo -e "\t       [TOOLPREFIX=prefix-of-arm-toolchain]";
	@exec echo -e "\t       [OBJDIR=path-to-asp-obj-dir]";
	@exec echo -e "\t       [BINDIR=path-to-export-safeg-binaries]";
	@exec echo -e "\t       [BUILD=debug or optimized]";
	@exec echo -e "\t       [NONTRUST=linux or asp]";
	@exec echo -e "\t       [UBOOTBIN=uboot-binary-file]";
	@exec echo -e "\t       [BOARD=BOARDNAME (see make showsupport)]\n";

showtargets:
	@find $(OBJDIR) \( -name 'obj_safeg_*' \) -print

showsupport:
	@exec echo -e "\n\tSupported boards:";
	@exec echo -e "\t   ARM PB1176JZF-S RealView Platform Baseboard (make BOARD=PB1176)";
	@exec echo -e "\t       Trust:";
	@exec echo -e "\t           TOPPERS/ASP 1.6";
	@exec echo -e "\t       Non-Trust:";
	@exec echo -e "\t           TOPPERS/ASP 1.6";
	@exec echo -e "\t           Linux 2.6.24";
	@exec echo -e "\t           Linux 2.6.33";
	@exec echo -e "\t   IDEA6410 Samsung S3C410 (make BOARD=IDEA6410)";
	@exec echo -e "\t       Trust:";
	@exec echo -e "\t           TOPPERS/ASP 1.6";
	@exec echo -e "\t           T-BareMetal";
	@exec echo -e "\t       Non-Trust:";
	@exec echo -e "\t           TOPPERS/ASP 1.6";
	@exec echo -e "\t           Linux 2.6.33";
	@exec echo -e "\t           Android 2.1";
	@exec echo -e "\t   MINI6410 Samsung S3C410 (make BOARD=MINI6410)";
	@exec echo -e "\t       Trust:";
	@exec echo -e "\t           TOPPERS/ASP 1.6";
	@exec echo -e "\t           T-BareMetal";
	@exec echo -e "\t       Non-Trust:";
	@exec echo -e "\t           TOPPERS/ASP 1.6";
	@exec echo -e "\t           Linux 2.6.38";
	@exec echo -e "\t           Android 2.1";
	@exec echo -e "\t           Qtopia 2.2.0";
	@exec echo -e "\t           Ubuntu 9.10";
	@exec echo -e "\t   Kyoto KZM-CA9-01 board (make BOARD=KZMCA9)";
	@exec echo -e "\t       Trust:";
	@exec echo -e "\t           TOPPERS/FMP 1.6";
	@exec echo -e "\t       Non-Trust:";
	@exec echo -e "\t           TOPPERS/FMP 1.6";
	@exec echo -e "\t           Linux 2.6.28";

clean:
	$(RM) -f $(BINDIR)/*.elf $(BINDIR)/safeg.sym
	@$(MAKE) -C safeg/ clean;
	@find \( -name '*~*' \) -print -delete
ifneq ($(OBJ),)
	@$(MAKE) -C $(OBJDIR)/$(OBJ)/trust/ clean;
ifneq ($(NONTRUST),linux)
	@$(MAKE) -C $(OBJDIR)/$(OBJ)/non_trust/ clean;
endif
endif
