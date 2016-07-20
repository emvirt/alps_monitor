/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2006-2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: mpcore.h 575 2010-04-21 13:06:06Z ertl-honda $
 */

/*
 *  MPCORE �Υϡ��ɥ������񸻤����
 */

#ifndef TOPPERS_MPCORE_H
#define TOPPERS_MPCORE_H

#include <sil.h>

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ���ץ�ؤΥ��������ޥ���
 */
#define CP15_CONTROL_READ(x)     Asm("mrc p15, 0, %0, c1, c0, 0":"=r"(x))
#define CP15_CONTROL_WRITE(x)    Asm("mcr p15, 0, %0, c1, c0, 0"::"r"(x))

#define CP15_AUXILIARY_READ(x)   Asm("mrc p15, 0, %0, c1, c0, 1":"=r"(x))
#define CP15_AUXILIARY_WRITE(x)  Asm("mcr p15, 0, %0, c1, c0, 1"::"r"(x))

#define CP15_ICACHE_INVALIDATE()           Asm("mcr p15, 0, %0, c7, c5, 0"::"r"(0))
#if __TARGET_ARCH_ARM == 6
#define CP15_DCACHE_INVALIDATE()           Asm("mcr p15, 0, %0, c7, c6, 0"::"r"(0))
#define CP15_DCACHE_CLEAN_AND_INVALIDATE() Asm("MCR p15, 0, %0, c7, c14, 0"::"r"(0))
#elif __TARGET_ARCH_ARM == 7
#define CP15_DCACHE_INVALIDATE(x)           Asm("mcr p15, 0, %0, c7, c6, 2"::"r"(x))
#define CP15_DCACHE_CLEAN_AND_INVALIDATE(x) Asm("MCR p15, 0, %0, c7, c14, 2"::"r"(x))
#endif /* __TARGET_ARCH_ARM == 7 */

#if __TARGET_ARCH_ARM == 6
#define CP15_PBUFFER_FLUSH()               Asm("mcr p15, 0, %0, c7, c5, 4"::"r"(0))
#define CP15_DATA_SYNC_BARRIER()           Asm("mcr p15, 0, %0, c7, c10, 4"::"r"(0))
#define CP15_DATA_MEMORY_BARRIER()         Asm("mcr p15, 0, %0, c7, c10, 5"::"r" (0))
#elif __TARGET_ARCH_ARM == 7
#define CP15_PBUFFER_FLUSH()               Asm("isb")
#define CP15_DATA_SYNC_BARRIER()           Asm("dsb")
#define CP15_DATA_MEMORY_BARRIER()         Asm("dmb")
#endif /* __TARGET_ARCH_ARM == 7 */

#define CP15_INVALIDATE_UNIFIED_TLB() Asm("mcr p15, 0, %0, c8, c7,  0"::"r"(0))

#define CP15_TTBCR_WRITE(x) Asm("mcr p15, 0, %0, c2, c0, 2"::"r"(x))
#define CP15_TTB0_READ(x)   Asm("mrc p15, 0, %0, c2, c0, 0":"=r"(x))
#define CP15_TTB0_WRITE(x)  Asm("mcr p15, 0, %0, c2, c0, 0"::"r"(x))

#define CP15_DOMAINS_WRITE(x) Asm("mcr p15, 0, %0, c3, c0, 0":: "r"(x))

#define CP15_CPUID_READ(x)     Asm("mrc p15, 0, %0, c0, c0, 5":"=r"(x))

#define CP15_CACHE_SIZE_ID_READ(x)         Asm("mrc p15, 1, %0, c0, c0, 0":"=r"(x))

#if __TARGET_ARCH_ARM == 7
#define CP15_CACHE_SIZE_SELECTION_WRITE(x) Asm("mcr p15, 2, %0, c0, c0, 0"::"r"(x))
#define CP15_CACHE_SIZE_SELECTION_READ(x)  Asm("mrc p15, 2, %0, c0, c0, 0":"=r"(x))
#endif /* __TARGET_ARCH_ARM == 7 */

#define CPU15_PONLY_THREAD_ID_READ(x)  Asm("mrc p15, 0, %0, c13, c0, 4":"=r"(x))
#define CPU15_PONLY_THREAD_ID_WRITE(x) Asm("mcr p15, 0, %0, c13, c0, 4":: "r"(x)) 

/*
 *  ���ץ�Υӥå����
 */
#define CP15_CONTROL_XP_BIT   (1 << 23)
#define CP15_CONTROL_V_BIT    (1 << 13)
#define CP15_CONTROL_I_BIT    (1 << 12)
#define CP15_CONTROL_C_BIT    (1 <<  2)
#define CP15_CONTROL_M_BIT    (1 <<  0)

#if __TARGET_ARCH_ARM == 6
#define CP15_AUXILIARY_SA_BIT (1 << 5)
#define CP15_CPUID_BIT        (0x0f)
#else /* __TARGET_ARCH_ARM == 7 */
#define CP15_AUXILIARY_SA_BIT (1 << 6)
#define CP15_AUXILIARY_EX_BIT (1 << 7)
#define CP15_CPUID_BIT        (0x03)
#endif /* __TARGET_ARCH_ARM == 7 */

#define CP15_TTB0_RGN_S       (1 << 1)
#define CP15_TTB0_RGN_WBWA    (1 << 3)

#if __TARGET_ARCH_ARM == 7
#define CP15_TTB0_IRGN_WBWA   ((1 << 6)|(0))
#endif /* __TARGET_ARCH_ARM == 7 */


/*
 *  ���ץ���Ѥ����롼����
 */

/*
 *  �ץ��å�INDEX��0���ꥸ��ˤμ���
 */
Inline uint32_t
x_prc_index(void)
{
	uint32_t index;

	CP15_CPUID_READ(index);
	return((index & 0x0fU));
}

/*
 *  Data Synchronization Barrier
 *  �����ȯ�Ԥ��줿 read �� write �ν�λ���Ԥ�
 *  ������å��塤�֥����ץ�ǥ��������TLB�����ν�λ���Ԥ�
 *  ������̿��θ�˽񤫤줿̿��ϼ¹Ԥ���ʤ� 
 */
Inline void
mpcore_data_sync_barrier(void)
{
	CP15_DATA_SYNC_BARRIER();
}

/*
 *  Data Memory Barrier
 *  ���ץ����ε��Ҥ˽��äơ���˽񤫤줿̿��ǤΥ��ꥢ��������
 *     ��λ����ޤ��Ԥġ�
 *
 */
Inline void
mpcore_data_memory_barrier(void)
{
	CP15_DATA_MEMORY_BARRIER();
}

/*
 *  TLB��̵����
 */
Inline void
mpcore_invalidate_unfied_tlb(void)
{
	//CP15_DATA_SYNC_BARRIER();
	CP15_INVALIDATE_UNIFIED_TLB();
}

/*
 *  D����å����̵����
 */
Inline void
mpcore_dcache_invalidate(void)
{
#if __TARGET_ARCH_ARM == 6
	CP15_DCACHE_INVALIDATE();
#elif __TARGET_ARCH_ARM == 7
	uint32_t bits = 0;
	uint32_t ways = 0;
	uint32_t sets = 0;

	CP15_CACHE_SIZE_SELECTION_WRITE(0);
	CP15_PBUFFER_FLUSH();
	for (ways = 0; ways < 4; ways++){
		for (sets = 0; sets < 256; sets++){
			bits = ways << 30 | sets << 5;
			CP15_DCACHE_INVALIDATE(bits);
		}
	}
#endif /* __TARGET_ARCH_ARM == 7 */
}

/*
 *  D����å���Υ��꡼���̵����
 */
Inline void
mpcore_dcache_clean_and_invalidate(void)
{
#if __TARGET_ARCH_ARM == 6
	CP15_DCACHE_CLEAN_AND_INVALIDATE();
#elif __TARGET_ARCH_ARM == 7
	uint32_t bits = 0;
	uint32_t  ways = 0;
	uint32_t sets = 0;

	CP15_CACHE_SIZE_SELECTION_WRITE(0);
	CP15_PBUFFER_FLUSH();
	for (ways = 0; ways < 4; ways++){
		for (sets = 0; sets < 256; sets++){
			bits = ways << 30 | sets << 5;
			CP15_DCACHE_CLEAN_AND_INVALIDATE(bits);
		}
	}
#endif /* __TARGET_ARCH_ARM == 7 */
}

/*
 *  I����å����̵����
 */
Inline void
mpcore_icache_invalidate(void)
{
	CP15_ICACHE_INVALIDATE();
}

/*
 *  �ץ�ե��å��Хåե��򥯥ꥢ
 */
Inline void
mpcore_pbuffer_flash(void)
{
	CP15_PBUFFER_FLUSH();
}

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  ��¢�����޴�Ϣ�����
 */
// Private Timer
#define MPCORE_TM_LR     (MPCORE_PMR_BASE + 0x0600)  /* ���ɥ쥸����           */
#define MPCORE_TM_COUNT  (MPCORE_PMR_BASE + 0x0604)  /* �����󥿡��쥸����       */
#define MPCORE_TM_CNT    (MPCORE_PMR_BASE + 0x0608)  /* ����ȥ���쥸����     */
#define MPCORE_TM_ISR    (MPCORE_PMR_BASE + 0x060C)  /* ����ߥ��ơ������쥸���� */

#define MPCORE_TM_CNT_EN        0x1
#define MPCORE_TM_CNT_AR        0x2
#define MPCORE_TM_CNT_IEN       0x4
#define MPCORE_TM_CNT_PS_OFFSET   8

#define MPCORE_TM_ISR_SCBIT    0x01

// Watchdog Timer 
#define MPCORE_WDT_LR     (MPCORE_PMR_BASE + 0x0620)  /* ���ɥ쥸����            */
#define MPCORE_WDT_COUNT  (MPCORE_PMR_BASE + 0x0624)  /* �����󥿡��쥸����        */
#define MPCORE_WDT_CNT    (MPCORE_PMR_BASE + 0x0628)  /* ����ȥ���쥸����      */
#define MPCORE_WDT_ISR    (MPCORE_PMR_BASE + 0x062C)  /* ����ߥ��ơ������쥸����  */
#define MPCORE_WDT_RSR    (MPCORE_PMR_BASE + 0x0630)  /* �ꥻ�åȾ��֥쥸����      */
#define MPCORE_WDT_DR     (MPCORE_PMR_BASE + 0x0634)  /* Watchdog Disable Register */

#define MPCORE_WDT_CNT_EN        0x1
#define MPCORE_WDT_CNT_AR        0x2
#define MPCORE_WDT_CNT_IEN       0x4
#define MPCORE_WDT_CNT_WDTMODE   0x8
#define MPCORE_WDT_CNT_PS_OFFSET   8

#define MPCORE_WDT_ISR_SCBIT    0x01


/*
 *  Snoop Control Unit
 */
#define MPCORE_SCU_CTRL    (MPCORE_PMR_BASE + 0x0000)  /* ����ȥ���쥸���� */
#define MPCORE_SCU_CONFIG  (MPCORE_PMR_BASE + 0x0004)  /* ����ե�����졼�����쥸���� */
#define MPCORE_SCU_CPUST   (MPCORE_PMR_BASE + 0x0008)  /* CPU���ơ����� */
#define MPCORE_SCU_IALL    (MPCORE_PMR_BASE + 0x000C)  /* ����Х�åɥ����� */
#define MPCORE_SCU_PM_CTRL (MPCORE_PMR_BASE + 0x0010)  /* �ѥե����ޥ󥹥�˥�����쥸���� */
#define MPCORE_SCU_PM_EVT0 (MPCORE_PMR_BASE + 0x0014)  /* ��˥������󥿥��٥��0 */
#define MPCORE_SCU_PM_EVT1 (MPCORE_PMR_BASE + 0x0018)  /* ��˥������󥿥��٥��1 */
#define MPCORE_SCU_PM_MN0  (MPCORE_PMR_BASE + 0x001C)  /* ��˥�������0 */
#define MPCORE_SCU_PM_MN1  (MPCORE_PMR_BASE + 0x0020)  /* ��˥�������1 */
#define MPCORE_SCU_PM_MN2  (MPCORE_PMR_BASE + 0x0024)  /* ��˥�������2 */
#define MPCORE_SCU_PM_MN3  (MPCORE_PMR_BASE + 0x0028)  /* ��˥�������3 */
#define MPCORE_SCU_PM_MN4  (MPCORE_PMR_BASE + 0x002C)  /* ��˥�������4 */
#define MPCORE_SCU_PM_MN5  (MPCORE_PMR_BASE + 0x0030)  /* ��˥�������5 */
#define MPCORE_SCU_PM_MN6  (MPCORE_PMR_BASE + 0x0034)  /* ��˥�������6 */
#define MPCORE_SCU_PM_MN7  (MPCORE_PMR_BASE + 0x0038)  /* ��˥�������7 */
#define MPCORE_SCU_SNSAC  (MPCORE_PMR_BASE + 0x0054)  /* �Υ󥻥��奢������������ȥ���쥸����*/

#define MPCORE_SCU_CTRL_EN (1 << 0)   /* SCU���͡��֥� */

#define MPCORE_SCU_PM_CTRL_ENA  (1 << 0)    /* �ѥե����ޥ󥹥�˥�ͭ���� */
#define MPCORE_SCU_PM_CTRL_RST  (1 << 1)    /* �ѥե����ޥ󥹥�˥��ꥻ�å� */

#define MPCORE_SCU_PM_EVT_CYC_COUNT0_4 (31 << 0) /* �������륫����Ȼ��� */

#define MPCORE_SCU_IALL_WAYS   (0x0000ffffU)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  SCU�ν����
 */
extern void scu_init(void);

#if __TARGET_ARCH_ARM == 6
/*
 * SCU�����󥿤ν������MN0����ѡ�
 */
extern void scu_counter_init(void);

/*
 * SCU�����ޤΥꥻ�å�
 */
Inline void
scu_counter_reset(void)
{
	sil_wrw_mem((void *)MPCORE_SCU_PM_CTRL,
			sil_rew_mem((void *)MPCORE_SCU_PM_CTRL) | MPCORE_SCU_PM_CTRL_RST);
}

/*
 * SCU�����ޤθ����ͤ��ɤ�
 */
Inline uint_t
scu_counter_get_current(void)
{
	return (sil_rew_mem((void *)MPCORE_SCU_PM_MN0));
}
#endif /* __TARGET_ARCH_ARM == 6 */
#endif /* TOPPERS_MACRO_ONLY */

/*
 *  Distributed Interrupt Controller CPU Interface
 *
 *  ͥ���٤ϡ�0x0��0xF ��16�ʳ���
 *  0x0���ǹ�ͥ���٤ǡ�0xF������ͥ���١�
 */
#define DIC_CPUIF_CR   (MPCORE_PMR_BASE + 0x0100)  /* ����ȥ���쥸���� */
#define DIC_CPUIF_PMR  (MPCORE_PMR_BASE + 0x0104)  /* ͥ���٥ޥ����쥸���� */
#define DIC_CPUIF_BPR  (MPCORE_PMR_BASE + 0x0108)  /* �Х��ʥ�ݥ���ȥ쥸���� */
#define DIC_CPUIF_IAR  (MPCORE_PMR_BASE + 0x010C)  /* Interrupt Acknowledge Register */
#define DIC_CPUIF_EOI  (MPCORE_PMR_BASE + 0x0110)  /* End of Interrupt Register */
#define DIC_CPUIF_RII  (MPCORE_PMR_BASE + 0x0114)  /* Running Interrupt Register */

#define DIC_CPUIF_CR_EN		     0x01  /* non-secure only */
#define DIC_CPUIF_CR_EN_S        0x01  /* secure only */
#define DIC_CPUIF_CR_EN_NS       0x02  /* secure only */
#define DIC_CPUIF_CR_EN_ACKCTL   0x04  /* secure only */
#define DIC_CPUIF_CR_FIQEN       0x08  /* secure only */
#define DIC_CPUIF_CR_EN_SBPR     0x10  /* secure only */


#if __TARGET_ARCH_ARM == 6
#define DIC_CPUIF_PMR_OFFSET 0x04
#elif __TARGET_ARCH_ARM == 7
#define DIC_CPUIF_PMR_OFFSET 0x03
#endif /* __TARGET_ARCH_ARM == 7 */
#define DIC_CPUIF_BPR_ALL    0x03  /* ���ƤΥӥåȤ�ͭ�� */
#define DIC_CPUIF_BPR_3      0x04  /* 3�ӥå�ͭ��        */
#define DIC_CPUIF_BPR_2      0x05  /* 2�ӥå�ͭ��        */
#define DIC_CPUIF_BPR_1      0x06  /* 1�ӥå�ͭ��        */

/*
 *  Distributed Interrupt Controller
 */
#define DIC_CR     (MPCORE_PMR_BASE + 0x1000)  /* ����ȥ���쥸���� */
#define DIC_ICT    (MPCORE_PMR_BASE + 0x1004)  /* Interrupt Controller �����ץ쥸���� */
#if __TARGET_ARCH_ARM == 7
#define DIC_IID    (MPCORE_PMR_BASE + 0x1008)  /* Distributor Implementer ID */
#define DIC_SEC    (MPCORE_PMR_BASE + 0x1080)  /* ����ߥ������ƥ� */
#endif /* __TARGET_ARCH_ARM == 7 */
#define DIC_SE     (MPCORE_PMR_BASE + 0x1100)  /* ����ߥ��͡��֥륻�å�   */
#define DIC_CE     (MPCORE_PMR_BASE + 0x1180)  /* ����ߥ��͡��֥륯�ꥢ�� */
#define DIC_SP     (MPCORE_PMR_BASE + 0x1200)  /* ����ߥ��åȥڥ�ǥ��� */
#define DIC_CP     (MPCORE_PMR_BASE + 0x1280)  /* ����ߥ��ꥢ���ڥ�ǥ��� */
#define DIC_IPR    (MPCORE_PMR_BASE + 0x1400)  /* �����ͥ���٥쥸���� */
#define DIC_ITR    (MPCORE_PMR_BASE + 0x1800)  /* ����ߥ������åȥ쥸����/CA9��SPI�������åȥ쥸���� */
#define DIC_ICR    (MPCORE_PMR_BASE + 0x1C00)  /* ����ߥ���ե�����졼�����쥸���� */
#define DIC_SIR    (MPCORE_PMR_BASE + 0x1F00)  /* ���եȥ���������ߥ쥸����  */
#if __TARGET_ARCH_ARM == 7
#define DIC_PPIS   (MPCORE_PRM_BASE + 0x1D00)  /* PPI���ơ����� */
#define DIC_SPIS   (MPCORE_PRM_BASE + 0x1D04)  /* SPI���ơ����� */
#endif /* __TARGET_ARCH_ARM == 7 */
/* ����ȥ���쥸������Ϣ���� */
#define DIC_CR_ENABLE 0x01
#define DIC_CR_NS_ENABLE 0x02


/*
 *  �������Υץ��å��λ���
 */
#define DIC_ITR_CPU0  0x01
#define DIC_ITR_CPU1  0x02
#define DIC_ITR_CPU2  0x04
#define DIC_ITR_CPU3  0x08

#define DIC_SIR_CPU0  0x01
#define DIC_SIR_CPU1  0x02
#define DIC_SIR_CPU2  0x04
#define DIC_SIR_CPU3  0x08
#define DIC_SIR_CPUS  0x0f

#define DIC_SIR_CPU_OFFSET 16

/*
 *  �����ͥ����
 */
#define DIC_MAX_PRIORITY  0x00
#if __TARGET_ARCH_ARM == 6
#define DIC_MIN_PRIORITY  0x0f
#elif __TARGET_ARCH_ARM == 7
#define DIC_MIN_PRIORITY  0x1f
#endif /* __TARGET_ARCH_ARM == 7 */

/*
 *  ����ե�����졼�����쥸������������
 */
#define DIC_ICR_EDGE     0x03   /* ���å������ */
#define DIC_ICR_LEVEL    0x01   /* ��٥����� */

/*
 *  DIC�ǥ��ݡ��Ȥ��Ƥ������߿�
 */
#define DIC_TMIN_INTNO      0U

#ifndef DIC_TMAX_INTNO
#define DIC_TMAX_INTNO    255U
#endif /* DIC_TMAX_INTNO */

#ifndef DIC_TNUM_INT
#define DIC_TNUM_INT      256U
#endif /* DIC_TNUM_INT */

/*
 *  �����Х����ߤγ����ֹ�
 */
#define TMIN_GLOBAL_INTNO   32U

/*
 * ������ֹ�(DIC�Ǥ��ֹ�)
 */
#define DIC_IRQNO_IPI0     0
#define DIC_IRQNO_IPI1     1
#define DIC_IRQNO_IPI2     2
#define DIC_IRQNO_IPI3     3
#define DIC_IRQNO_TM      29
#define DIC_IRQNO_WDT     30

#ifndef TOPPERS_MACRO_ONLY


/*
 *  DIC CPU���󥿥ե�������Ϣ�Υɥ饤��
 */
/*
 *  CPU�γ����ͥ���٥ޥ���������
 */ 
Inline void
dic_cpu_set_priority(int pri)
{
	sil_wrw_mem((void *)DIC_CPUIF_PMR,
				(pri << DIC_CPUIF_PMR_OFFSET));
}

/*
 *  CPU�γ����ͥ���٥ޥ��������
 */ 
Inline int
dic_cpu_current_priority(void)
{
	return (sil_rew_mem((void *)(DIC_CPUIF_PMR)) >>
			DIC_CPUIF_PMR_OFFSET);    
}

/*
 *  ����ߥϥ�ɥ�νи���ɬ�פ�IRC���
 */
Inline void
dic_cpu_end_int(uint8_t id)
{
	sil_wrw_mem((void *)(DIC_CPUIF_EOI), id);
}

/*
 *  DIC�Υץ��å��γ����ͥ���٤ΤɤΥӥåȤ���Ѥ��뤫
 */
Inline void
dic_cpu_set_bp(int mask_bit)
{
	sil_wrw_mem((void *)DIC_CPUIF_BPR, mask_bit);
}

/*
 *  CPU���󥿥ե������ν����
 */
extern void dic_cpu_init(void);

/*
 *  CPU���󥿥ե������ν�λ
 */
extern void dic_cpu_stop(void);


/*
 *  DIC DIS���󥿥ե�������Ϣ�Υɥ饤��
 */

/*
 *  ����߶ػ�
 */
extern void dic_disable_int(uint8_t id);

/*
 *  ����ߵ���
 */
extern void dic_enable_int(uint8_t id);

/*
 *  ����ߥڥ�ǥ��󥰥��ꥢ
 */
extern void dic_clear_pending(uint8_t id);

/*
 *  ����ߥڥ�ǥ��󥰥��å�
 */
extern void dic_set_pending(uint8_t id);

/*
 *  ������׵�Υ����å�
 */
extern bool_t dic_probe_int(uint8_t id);

/*
 *  ���������Υ��å�
 */
extern void dic_config(uint8_t id, uint8_t config);

/*
 *  �����ͥ���٤Υ��å�
 *  ����ɽ�����Ϥ��� 
 */
extern void dic_set_priority(uint8_t id, int pri);

/*
 *  DIC����ߥ������åȤ�����
 *  CPU��OR�ǻ���  
 */
extern void dic_set_target(uint8_t id, uint8_t cpus);

/*
 *  DIC���󥿥ե������ν����
 */
extern void dic_init(void);

extern void dic_sgi_sec_init(void);

/*
 *  DIC���󥿥ե������ν�λ
 */
extern void dic_stop(void);

/*
 *  ���եȥ���������ߤ�ȯ��
 */
Inline void
dic_swi(int cpu, int id)
{
	sil_wrw_mem((void *)DIC_SIR, cpu << DIC_SIR_CPU_OFFSET|id);
}

/*
 *  SMP�⡼�ɤ�ͭ����
 */
extern void mpcore_smp_mode_enable(void);

/*
 *  MMU��Ϣ
 */

/*
 * �Ѵ��ơ��֥�ؤ���������
 * va   : ���ۥ��ɥ쥹
 * pa   : ʪ�����ɥ쥹
 * size : ������ 
 * s    : ��ͭ����
 * tex  : C B�Ȥ��Ȥ߹�碌���Ѳ�
 * ap   : ����������
 * c    : ����å���
 * b    : �Хåե�
 */
typedef struct{
	uintptr_t   va;
	uintptr_t   pa;
	uint32_t    size;
	uint8_t     s;
	uint8_t     tex;
	uint8_t     ap;
	uint8_t     c;
	uint8_t     b;
}MEMORY_ATTRIBUTE;

/*
 *  MMU�ν����
 */
extern void mpcore_mmu_init(void);

/*
 *  MMU�ˤ�����Υޥåԥ�
 */
extern void mmu_map_memory(MEMORY_ATTRIBUTE *m_attribute);

/*
 *  I/D����å����ξ����ͭ����
 */
extern void mpcore_cache_enable(void);

/*
 *  I/D����å����ξ����̵����
 */
extern void mpcore_cache_disable(void);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_MPCORE_H */
