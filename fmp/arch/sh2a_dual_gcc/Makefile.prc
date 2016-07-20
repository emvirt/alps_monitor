#
#  @(#) $Id$
#

#
#		Makefile のプロセッサ依存部（SH2A-DUAL用）
#

#
#  コンパイルオプション
#
COPTS := $(COPTS)
LDFLAGS := -nostdlib $(LDFLAGS)
CDEFS := $(CDEFS) -DTOPPERS_LABEL_ASM
LIBS := $(LIBS)  -lgcc

#
#  システムサービスに関する定義
#  　kernel_cfg_asm.Sをシステムサービスに含めるのは、適切ではないが、
#  　　・コンパイル・オプション（特にインクルードパス）
#  　　・カーネル・ライブラリの構成
#  　の都合で、便宜上、ここに含めている。
#
SYSSVC_ASMOBJS := $(SYSSVC_ASMOBJS) kernel_cfg_asm.o
REALCLEAN_FILES := $(REALCLEAN_FILES) kernel_cfg_asm.S

#
#  コンフィギュレータ関係の変数の定義
#
CFG_TABS := $(CFG_TABS) --cfg1-def-table $(SRCDIR)/arch/$(PRC)_$(TOOL)/prc_def.csv

#
#  カーネルに関する定義
#
KERNEL_DIR := $(KERNEL_DIR) $(SRCDIR)/arch/$(PRC)_$(TOOL)
KERNEL_ASMOBJS := $(KERNEL_ASMOBJS) prc_support.o
KERNEL_COBJS := $(KERNEL_COBJS) prc_config.o prc_timer.o
KERNEL_CFLAGS := $(KERNEL_CFLAGS) -fno-strict-aliasing

#
#  依存関係の定義
#
cfg1_out.c: $(SRCDIR)/arch/$(PRC)_$(TOOL)/prc_def.csv
kernel_cfg.timestamp: $(SRCDIR)/arch/$(PRC)_$(TOOL)/prc.tf
$(OBJFILE): $(SRCDIR)/arch/$(PRC)_$(TOOL)/prc_check.tf
