#
#  @(#) $Id$
#

#
#		Makefile �Υץ��å���¸����SH2A-DUAL�ѡ�
#

#
#  ����ѥ��륪�ץ����
#
COPTS := $(COPTS)
LDFLAGS := -nostdlib $(LDFLAGS)
CDEFS := $(CDEFS) -DTOPPERS_LABEL_ASM
LIBS := $(LIBS)  -lgcc

#
#  �����ƥॵ���ӥ��˴ؤ������
#  ��kernel_cfg_asm.S�򥷥��ƥॵ���ӥ��˴ޤ��Τϡ�Ŭ�ڤǤϤʤ�����
#  ����������ѥ��롦���ץ������ä˥��󥯥롼�ɥѥ���
#  �����������ͥ롦�饤�֥��ι���
#  �����Թ�ǡ��ص��塢�����˴ޤ�Ƥ��롣
#
SYSSVC_ASMOBJS := $(SYSSVC_ASMOBJS) kernel_cfg_asm.o
REALCLEAN_FILES := $(REALCLEAN_FILES) kernel_cfg_asm.S

#
#  ����ե�����졼���ط����ѿ������
#
CFG_TABS := $(CFG_TABS) --cfg1-def-table $(SRCDIR)/arch/$(PRC)_$(TOOL)/prc_def.csv

#
#  �����ͥ�˴ؤ������
#
KERNEL_DIR := $(KERNEL_DIR) $(SRCDIR)/arch/$(PRC)_$(TOOL)
KERNEL_ASMOBJS := $(KERNEL_ASMOBJS) prc_support.o
KERNEL_COBJS := $(KERNEL_COBJS) prc_config.o prc_timer.o
KERNEL_CFLAGS := $(KERNEL_CFLAGS) -fno-strict-aliasing

#
#  ��¸�ط������
#
cfg1_out.c: $(SRCDIR)/arch/$(PRC)_$(TOOL)/prc_def.csv
kernel_cfg.timestamp: $(SRCDIR)/arch/$(PRC)_$(TOOL)/prc.tf
$(OBJFILE): $(SRCDIR)/arch/$(PRC)_$(TOOL)/prc_check.tf
