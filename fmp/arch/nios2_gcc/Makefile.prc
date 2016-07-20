#
#  @(#) $Id: Makefile.prc 447 2009-10-16 08:51:47Z ertl-honda $
# 

#
#		Makefile のチップ依存部（Nios2用）
#

#
#  チップ名，開発環境名の定義
#
PRC  = nios2
TOOL = gcc

#
#  コンパイルオプション
#
INCLUDES := $(INCLUDES) -I$(SRCDIR)/arch/$(PRC)_$(TOOL)
COPTS := $(COPTS)
LDFLAGS := $(LDFLAGS)
LIBS := $(LIBS) -lgcc -lc -lgcc

#
#  プロセッサ数に関する定義
#
ifdef TNUM_PRCID
	CDEFS := $(CDEFS) -DTNUM_PRCID=$(TNUM_PRCID)
endif

#
#  カーネルに関する定義
#
KERNEL_DIR := $(KERNEL_DIR) $(SRCDIR)/arch/$(PRC)_$(TOOL)
KERNEL_ASMOBJS := $(KERNEL_ASMOBJS) prc_support.o
KERNEL_COBJS := $(KERNEL_COBJS) prc_config.o avalon_timer.o

#
#  GNU開発環境のターゲットアーキテクチャの定義
#
GCC_TARGET = nios2-elf

#
#  スタートアップモジュールに関する定義
#
#  リンカスクリプトに「STARTUP(start.o)」を記述したため，スタートアップモジュー
#  ルの名前をHIDDEN_OBJSに定義する．また，LDFLAGSに-nostdlibを追加している．
#
HIDDEN_OBJS = start.o

$(HIDDEN_OBJS): %.o: %.S
	$(CC) -c $(CFLAGS) $(KERNEL_CFLAGS) $<

$(HIDDEN_OBJS:.o=.d): %.d: %.S
	@$(PERL) $(SRCDIR)/utils/makedep -C $(CC) \
		-O "$(CFLAGS) $(KERNEL_CFLAGS)" $< >> Makefile.depend

LDFLAGS := -nostdlib $(LDFLAGS)

#
#  依存関係の定義
#
kernel_cfg.timestamp: $(SRCDIR)/arch/$(PRC)_$(TOOL)/prc.tf
