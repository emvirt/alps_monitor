#
#  @(#) $Id: Makefile.prc 447 2009-10-16 08:51:47Z ertl-honda $
# 

#
#		Makefile �Υ��åװ�¸����Nios2�ѡ�
#

#
#  ���å�̾����ȯ�Ķ�̾�����
#
PRC  = nios2
TOOL = gcc

#
#  ����ѥ��륪�ץ����
#
INCLUDES := $(INCLUDES) -I$(SRCDIR)/arch/$(PRC)_$(TOOL)
COPTS := $(COPTS)
LDFLAGS := $(LDFLAGS)
LIBS := $(LIBS) -lgcc -lc -lgcc

#
#  �ץ��å����˴ؤ������
#
ifdef TNUM_PRCID
	CDEFS := $(CDEFS) -DTNUM_PRCID=$(TNUM_PRCID)
endif

#
#  �����ͥ�˴ؤ������
#
KERNEL_DIR := $(KERNEL_DIR) $(SRCDIR)/arch/$(PRC)_$(TOOL)
KERNEL_ASMOBJS := $(KERNEL_ASMOBJS) prc_support.o
KERNEL_COBJS := $(KERNEL_COBJS) prc_config.o avalon_timer.o

#
#  GNU��ȯ�Ķ��Υ������åȥ������ƥ���������
#
GCC_TARGET = nios2-elf

#
#  �������ȥ��åץ⥸�塼��˴ؤ������
#
#  ��󥫥�����ץȤˡ�STARTUP(start.o)�פ򵭽Ҥ������ᡤ�������ȥ��åץ⥸�塼
#  ���̾����HIDDEN_OBJS��������롥�ޤ���LDFLAGS��-nostdlib���ɲä��Ƥ��롥
#
HIDDEN_OBJS = start.o

$(HIDDEN_OBJS): %.o: %.S
	$(CC) -c $(CFLAGS) $(KERNEL_CFLAGS) $<

$(HIDDEN_OBJS:.o=.d): %.d: %.S
	@$(PERL) $(SRCDIR)/utils/makedep -C $(CC) \
		-O "$(CFLAGS) $(KERNEL_CFLAGS)" $< >> Makefile.depend

LDFLAGS := -nostdlib $(LDFLAGS)

#
#  ��¸�ط������
#
kernel_cfg.timestamp: $(SRCDIR)/arch/$(PRC)_$(TOOL)/prc.tf
