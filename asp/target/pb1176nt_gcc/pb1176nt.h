/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2003      by Advanced Data Controls, Corp
 *  Copyright (C) 2004-2007 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ�
 *      ���դ��뤳�ȡ�
 *
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 *
 *  @(#) $Id: cm922t.h 264 2007-07-12 08:23:10Z hiro $
 */

#ifndef TOPPERS_PB1176NT_H
#define TOPPERS_PB1176NT_H

#include <sil.h>


/*
 * ������Ԥ�̿��
 */
#define ASM_TARGET_WAIT_INTERRUPT nop

/*
 *  GIC�˴ؤ������
 *  PB1176�Υƥ��ȥ��å׾��GIC�ϡ�MPCore��DIC��١����Ȥ��Ƥ��롥
 */

/*
 *  Distributed Interrupt Controller CPU Interface
 *
 *  ͥ���٤ϡ�0x0��0xF ��16�ʳ���
 *  0x0���ǹ�ͥ���٤ǡ�0xF������ͥ���١�
 */
#define GIC_BASE 0x10120000

#define GIC_CPUIF_CR   (GIC_BASE + 0x0000)  /* ����ȥ���쥸���� */
#define GIC_CPUIF_PMR  (GIC_BASE + 0x0004)  /* ͥ���٥ޥ����쥸���� */
#define GIC_CPUIF_BPR  (GIC_BASE + 0x0008)  /* �Х��ʥ�ݥ���ȥ쥸���� */
#define GIC_CPUIF_IAR  (GIC_BASE + 0x000C)  /* Interrupt Acknowledge Register */
#define GIC_CPUIF_EOI  (GIC_BASE + 0x0010)  /* End of Interrupt Register */
#define GIC_CPUIF_RII  (GIC_BASE + 0x0014)  /* Running Interrupt Register */

#define GIC_CPUIF_CR_EN      0x01
#define GIC_CPUIF_PMR_OFFSET 0x04
#define GIC_CPUIF_BPR_ALL    0x03  /* ���ƤΥӥåȤ�ͭ�� */
#define GIC_CPUIF_BPR_3      0x04  /* 3�ӥå�ͭ��        */
#define GIC_CPUIF_BPR_2      0x05  /* 2�ӥå�ͭ��        */
#define GIC_CPUIF_BPR_1      0x06  /* 1�ӥå�ͭ��        */

/*
 *  Distributed Interrupt Controller
 */
#define GIC_CR     (GIC_BASE + 0x1000)  /* ����ȥ���쥸���� */
#define GIC_SE     (GIC_BASE + 0x1100)  /* ����ߥ��͡��֥륻�å�   */
#define GIC_CE     (GIC_BASE + 0x1180)  /* ����ߥ��͡��֥륯�ꥢ�� */
#define GIC_SP     (GIC_BASE + 0x1200)  /* ����ߥ��åȥڥ�ǥ��� */
#define GIC_CP     (GIC_BASE + 0x1280)  /* ����ߥ��ꥢ���ڥ�ǥ��� */
#define GIC_IPR    (GIC_BASE + 0x1400)  /* �����ͥ���٥쥸���� */
#define GIC_ITR    (GIC_BASE + 0x1800)  /* ����ߥ������åȥ쥸���� */
#define GIC_ICR    (GIC_BASE + 0x1C00)  /* ����ߥ���ե�����졼�����쥸���� */
#define GIC_SIR    (GIC_BASE + 0x1F00)  /* ���եȥ���������ߥ쥸����  */

#define GIC_CR_ENABLE 0x01

/*
 *  �������Υץ��å��λ���
 */
#define GIC_ITR_CPU0  0x01
#define GIC_ITR_CPU1  0x02
#define GIC_ITR_CPU2  0x04
#define GIC_ITR_CPU3  0x08

#define GIC_SIR_CPU0  0x01
#define GIC_SIR_CPU1  0x02
#define GIC_SIR_CPU2  0x04
#define GIC_SIR_CPU3  0x08
#define GIC_SIR_CPUS  0x0f

#define GIC_SIR_CPU_OFFSET 16

/*
 *  �����ͥ����
 */
#define GIC_MAX_PRIORITY  0x0
#define GIC_MIN_PRIORITY  0xf

/*
 *  ����ե�����졼�����쥸������������
 */
#define GIC_ICR_EDGE     0x03   /* ���å������ */
#define GIC_ICR_LEVEL    0x01   /* ��٥����� */

/*
 *  GIC�ǥ��ݡ��Ȥ��Ƥ������߿�
 */
#define GIC_TMIN_INTNO      0U
#define GIC_TMAX_INTNO     63U
#define GIC_TNUM_INT       64U

#ifndef TOPPERS_MACRO_ONLY
/*
 *  GIC CPU���󥿥ե�������Ϣ�Υɥ饤��
 */
/*
 *  CPU�γ����ͥ���٥ޥ���������
 */
Inline void
gic_cpu_set_priority(int pri)
{
	sil_wrw_mem((void *)GIC_CPUIF_PMR,
                (pri << GIC_CPUIF_PMR_OFFSET));
}

/*
 *  CPU�γ����ͥ���٥ޥ��������
 */
Inline int
gic_cpu_current_priority(void)
{
    return (sil_rew_mem((void *)(GIC_CPUIF_PMR)) >>
            GIC_CPUIF_PMR_OFFSET);
}

/*
 *  ����ߥϥ�ɥ�νи���ɬ�פ�IRC���
 */
Inline void
gic_cpu_end_int(uint8_t id)
{
    sil_wrw_mem((void *)(GIC_CPUIF_EOI), id);
}

/*
 *  GIC�Υץ��å��γ����ͥ���٤ΤɤΥӥåȤ���Ѥ��뤫
 */
Inline void
gic_cpu_set_bp(int mask_bit)
{
	sil_wrw_mem((void *)GIC_CPUIF_BPR, mask_bit);
}

/*
 *  CPU���󥿥ե������ν����
 */
extern void gic_cpu_init(void);

/*
 *  CPU���󥿥ե������ν�λ
 */
extern void gic_cpu_stop(void);

/*
 *  GIC DIS���󥿥ե�������Ϣ�Υɥ饤��
 */

/*
 *  ����߶ػ�
 */
extern void gic_disable_int(uint8_t id);

/*
 *  ����ߵ���
 */
extern void gic_enable_int(uint8_t id);

/*
 *  ����ߥڥ�ǥ��󥰥��ꥢ
 */
extern void gic_clear_pending(uint8_t id);

/*
 *  ����ߥڥ�ǥ��󥰥��å�
 */
extern void gic_set_pending(uint8_t id);

/*
 *  ������׵�Υ����å�
 */
extern bool_t gic_probe_int(uint8_t id);

/*
 *  ���������Υ��å�
 */
extern void gic_config(uint8_t id, uint8_t config);

/*
 *  �����ͥ���٤Υ��å�
 *  ����ɽ�����Ϥ���
 */
extern void gic_set_priority(uint8_t id, int pri);

/*
 *  GIC����ߥ������åȤ�����
 *  CPU��OR�ǻ���
 */
extern void gic_set_target(uint8_t id, uint8_t cpus);

/*
 *  GIC���󥿥ե������ν����
 */
extern void gic_init(void);

/*
 *  GIC���󥿥ե������ν�λ
 */
extern void gic_stop(void);

#endif /* TOPPERS_MACRO_ONLY */

/*
 * ����ߥϥ�ɥ��ֹ椫�顤IRC���Τ���Υӥåȥѥ���������ޥ���
 */
#define INTNO_BITPAT(intno) (1U << intno)


/*
 *  UART�˴ؤ��������UART0����ѡ�
 */
#define UART_BASE_REG    0x1010c000

/*
 * UART Control Registers
 */
#define UART_PL011_DR			 0x000
#define UART_PL011_SR_CR		 0x004
#define UART_PL011_FR			 0x018
#define UART_PL011_LPR			 0x020
#define UART_PL011_IBRD			 0x024
#define UART_PL011_FBRD			 0x028
#define UART_PL011_LCR_H		 0x02C
#define UART_PL011_CR	 		 0x030
#define UART_PL011_IFLS			 0x034
#define UART_PL011_IMSC			 0x038
#define UART_PL011_RIS			 0x03C
#define UART_PL011_MIS			 0x040
#define UART_PL011_ICR			 0x044
#define UART_PL011_DMACR		 0x048

/*
 * Flag Register Constants : UARTx_FR
 */
/*[7] Transmit FIFO Empty*/
#define UFR_TXFE    0x80
/*[6] Receive FIFO Full*/
#define UFR_RXFF    0x40
/*[5] Transmit FIFO Full*/
#define UFR_TXFF    0x20
/*[4] Receive FIFO Empty*/
#define UFR_RXFE    0x10
/*[3] UART Busy*/
#define UFR_BUSY    0x08
/*[2] Data Carrier Detect*/
#define UFR_DCD     0x04
/*[1] Data Set Ready*/
#define UFR_DSR     0x02
/*[0] Clear To Send*/
#define UFR_CTS     0x01

#define UART_IMSC_RXIM 0x0010 /* ��������ߥޥ���          */
#define UART_IMSC_TXIM 0x0020 /* ��������ߥޥ���          */

/*
 *  UART������ֹ�
 */
#define IRQNO_UART       50    /* 18 for TZIC */


/*
 *  �����ޡ���Ϣ(Timer01�����)
 */
#define TIMER_BASE_REG         0x10104000

#define TIMER_CTIMER_1LOAD		0x000
#define TIMER_CTIMER_1VALUE		0x004
#define TIMER_CTIMER_1CONTROL	0x008
#define TIMER_CTIMER_1CLEAR		0x00C
#define TIMER_CTIMER_1RIS		0x010
#define TIMER_CTIMER_2LOAD		0x100
#define TIMER_CTIMER_2VALUE		0x104
#define TIMER_CTIMER_2CONTROL	0x108
#define TIMER_CTIMER_2CLEAR		0x10C

#define IRQNO_TIMER       40    /* 8 for TZIC */


#ifndef TOPPERS_MACRO_ONLY
/*
 * ���ץ�ؤΥ��������ޥ���
 */
#define CP15_CONTROL_READ(x)     Asm("mrc p15, 0, %0, c1, c0, 0":"=r"(x))
#define CP15_CONTROL_WRITE(x)    Asm("mcr p15, 0, %0, c1, c0, 0"::"r"(x))

#define CP15_AUXILIARY_READ(x)   Asm("mrc p15, 0, %0, c1, c0, 1":"=r"(x))
#define CP15_AUXILIARY_WRITE(x)  Asm("mcr p15, 0, %0, c1, c0, 1"::"r"(x))

#define CP15_CACHE_INVALIDATE_BOTH() Asm("mcr p15, 0, %0, c7, c7, 0":: "r"(0))
#define CP15_ICACHE_INVALIDATE() Asm("mcr p15, 0, %0, c7, c5, 0"::"r"(0))
#define CP15_DCACHE_INVALIDATE() Asm("mcr p15, 0, %0, c7, c6, 0"::"r"(0))
#define CP15_DCACHE_CLEAN_AND_INVALIDATE() \
	Asm("mcr p15, 0, %0, c7, c14, 0":: "r"(0))
#define CP15_PBUFFER_FLUSH()     Asm("mcr p15, 0, %0, c7, c5, 4"::"r"(0))
#define CP15_DATA_SYNC_BARRIER() Asm("mcr p15, 0, %0, c7, c10, 4"::"r"(0))
#define CP15_DATA_MEMORY_BARRIER() Asm("mcr p15, 0, %0, c7, c10, 5"::"r" (0));

#define CP15_INVALIDATE_UNIFIED_TLB() Asm("mcr p15, 0, %0, c8, c7, 0"::"r"(0))

#define CP15_TTBCR_WRITE(x) Asm("mcr p15, 0, %0, c2, c0, 2"::"r"(x))
#define CP15_TTB0_READ(x)   Asm("mrc p15, 0, %0, c2, c0, 0":"=r"(x))
#define CP15_TTB0_WRITE(x)  Asm("mcr p15, 0, %0, c2, c0, 0"::"r"(x))

#define CP15_DOMAINS_WRITE(x) Asm("mcr p15, 0, %0, c3, c0, 0":: "r"(x))

#define CP15_SET_VBAR(x)    Asm("mcr p15, 0, %0, c12, c0, 0"::"r"(x))

#define CP15_PM_CONTROL_READ(x) Asm("mrc p15, 0, %0, c15, c12, 0":"=r"(x))
#define CP15_PM_CONTROL_WRITE(x) Asm("mcr p15, 0, %0, c15, c12, 0"::"r"(x))
#define CP15_PM_CYCLE_READ(x) Asm("mrc p15, 0, %0, c15, c12, 1":"=r"(x))
#define CP15_PM_CYCLE_WRITE(x) Asm("mcr p15, 0, %0, c15, c12, 1"::"r"(x))
#define CP15_PM_CR0_READ(x) Asm("mrc p15, 0, %0, c15, c12, 2":"=r"(x))
#define CP15_PM_CR0_WRITE(x) Asm("mcr p15, 0, %0, c15, c12, 2"::"r"(x))
#define CP15_PM_CR1_READ(x) Asm("mrc p15, 0, %0, c15, c12, 3":"=r"(x))
#define CP15_PM_CR1_WRITE(x) Asm("mcr p15, 0, %0, c15, c12, 3"::"r"(x))

/*
 * ���ץ�Υӥå����
 */
#define CP15_CONTROL_XP_BIT   (1 << 23)
#define CP15_CONTROL_I_BIT    (1 << 12)
#define CP15_CONTROL_C_BIT    (1 <<  2)
#define CP15_CONTROL_M_BIT    (1 <<  0)

#define CP15_TTB0_C           (1 << 0)
#define CP15_TTB0_S           (1 << 1)
#define CP15_TTB0_RGN_WBWA    (1 << 3)

/*
 * TCM mapping addresses (continuously mapped +0x1000 (4Kb))
 * NOTE: must be the same as in pb1176t.h
 */

#define TCM_BASE_ADDRESS  0x70000000
#define ITCM0_ADDRESS     TCM_BASE_ADDRESS
#define ITCM1_ADDRESS     (TCM_BASE_ADDRESS + 0x1000)
#define DTCM0_ADDRESS     (TCM_BASE_ADDRESS + 0x2000)
#define DTCM1_ADDRESS     (TCM_BASE_ADDRESS + 0x3000)

/*
 * ���ץ���Ѥ����롼����
 */

/*
 *  Data Synchronization Barrier
 *  �����ȯ�Ԥ��줿 read �� write �ν�λ���Ԥ�
 *  ������å��塤�֥����ץ�ǥ��������TLB�����ν�λ���Ԥ�
 *  ������̿��θ�˽񤫤줿̿��ϼ¹Ԥ���ʤ�
 */
Inline void
pb1176_data_sync_barrier(void){
    CP15_DATA_SYNC_BARRIER();
}

/*
 * Data Memory Barrier
 * ���ץ����ε��Ҥ˽��äơ���˽񤫤줿̿��ǤΥ��ꥢ��������
 *   ��λ����ޤ��Ԥġ�
 *
 */
Inline void
pb1176_data_memory_barrier(void){
    CP15_DATA_MEMORY_BARRIER();
}

/*
 * TLB��̵����
 */
Inline void
pb1176_invalidate_unfied_tlb(void){
    CP15_DATA_SYNC_BARRIER();
}


/*
 * D����å����̵����
 */
Inline void
pb1176_dcache_invalidate(void){
    CP15_DCACHE_INVALIDATE();
}


/*
 * D����å���Υ��꡼���̵����
 */
Inline void
pb1176_dcache_clean_and_invalidate(void){
    CP15_DCACHE_CLEAN_AND_INVALIDATE();
}


/*
 * I����å����̵����
 */
Inline void
pb1176_icache_invalidate(void){
    CP15_ICACHE_INVALIDATE();
}

/*
 * �ץ�ե��å��Хåե��򥯥ꥢ
 */
Inline void
pb1176_pbuffer_flash(void){
    CP15_PBUFFER_FLUSH();
}

/*
 * �����ͥ뵯ư���Υ������Ѥν����
 */
Inline void
pb1176nt_init_uart(void){
	/* setup baud rate divisor */
	sil_wrw_mem((void *)(UART_BASE_REG + UART_PL011_IBRD), 0x027);
	sil_wrw_mem((void *)(UART_BASE_REG + UART_PL011_FBRD), 0x004);

	/* refer to the TRM for details */
	sil_wrw_mem((void *)(UART_BASE_REG + UART_PL011_CR), 0x301);
	sil_wrw_mem((void *)(UART_BASE_REG + UART_PL011_LCR_H), 0x060);
	sil_wrw_mem((void *)(UART_BASE_REG + UART_PL011_IFLS), 0x000);

	/* Mask all interrupt */
	sil_wrw_mem((void *)(UART_BASE_REG + UART_PL011_IMSC), 0);
}

/*
 *  UART����Υݡ���󥰽���
 */
Inline void
pb1176nt_putc(char_t c)
{
	while((sil_rew_mem((void *)(UART_BASE_REG + UART_PL011_FR)) & UFR_TXFF) == UFR_TXFF);
	sil_wrw_mem((void *)(UART_BASE_REG + UART_PL011_DR), c);
}


/*
 * ��λ����
 */
Inline void
pb1176nt_exit(void)
{

}

#endif /* TOPPPERS_MACRO_ONLY */
#endif /* TOPPERS_PB1176NT_H */
