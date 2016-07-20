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
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 *  @(#) $Id: idea6410.h 264 2007-07-12 08:23:10Z hiro $
 */

#ifndef TOPPERS_IDEA6410T_H
#define TOPPERS_IDEA6410T_H

#include <sil.h>

/*
 * 割込み待ち命令
 */
#define ASM_TARGET_WAIT_INTERRUPT nop


/*
 *  TZICに関する定義
 */
#define TZIC0_BASE 0x71000000
#define TZIC1_BASE 0x71100000

#define TZIC0_STATUS      (TZIC0_BASE + 0x00)
#define TZIC0_RAWSTATUS   (TZIC0_BASE + 0x04)
#define TZIC0_INTSELECT   (TZIC0_BASE + 0x08)
#define TZIC0_ENABLE      (TZIC0_BASE + 0x0C)
#define TZIC0_CLEAR       (TZIC0_BASE + 0x10)
#define TZIC0_PROTECTION  (TZIC0_BASE + 0x18)
#define TZIC0_LOCK        (TZIC0_BASE + 0x1C)
#define TZIC0_LOCK_STATUS (TZIC0_BASE + 0x20)

#define TZIC1_STATUS      (TZIC1_BASE + 0x00)
#define TZIC1_RAWSTATUS   (TZIC1_BASE + 0x04)
#define TZIC1_INTSELECT   (TZIC1_BASE + 0x08)
#define TZIC1_ENABLE      (TZIC1_BASE + 0x0C)
#define TZIC1_CLEAR       (TZIC1_BASE + 0x10)
#define TZIC1_PROTECTION  (TZIC1_BASE + 0x18)
#define TZIC1_LOCK        (TZIC1_BASE + 0x1C)
#define TZIC1_LOCK_STATUS (TZIC1_BASE + 0x20)

#define TZIC_UNLOCK_CODE 0x0ACCE550
#define TZIC_LOCK_CODE   0x00000000

/*
 *  TZPC
 */
#define TZPCR0SIZE          0x7F000000
#define TZPCDECPROT0Stat    0x7F000800
#define TZPCDECPROT0Set     0x7F000804
#define TZPCDECPROT0Clr     0x7F000808
#define TZPCDECPROT1Stat    0x7F00080C
#define TZPCDECPROT1Set     0x7F000810
#define TZPCDECPROT1Clr     0x7F000814
#define TZPCDECPROT2Stat    0x7F000818
#define TZPCDECPROT2Set     0x7F00081C
#define TZPCDECPROT2Clr     0x7F000820

/*
 * 割込みハンドラ番号から，IRC操作のためのビットパターンを求めるマクロ
 */
#define INTNO_BITPAT(intno) (1U << intno)

#define UART0_BASE_REG    0x7F005000

/*
 *  ポーリング出力で使用するポート
 */
#define IDEA6410_PUTC_BASE_REG UART0_BASE_REG

/*
 *  Timer definitions (taken from linux regs-timer.h)
 */
#define S3C_TIMERREG(x) (0x7F006000 + (x))
#define S3C_TIMERREG2(tmr,reg) S3C_TIMERREG((reg)+0x0c+((tmr)*0x0c))

#define S3C_TCFG0   S3C_TIMERREG(0x00)
#define S3C_TCFG1   S3C_TIMERREG(0x04)
#define S3C_TCON    S3C_TIMERREG(0x08)

#define S3C_TINT_CSTAT  S3C_TIMERREG(0x44)

#define S3C_TCFG_PRESCALER0_MASK (255<<0)
#define S3C_TCFG_PRESCALER1_MASK (255<<8)
#define S3C_TCFG_PRESCALER1_SHIFT (8)
#define S3C_TCFG_PRESCALER0_SHIFT (0)
#define S3C_TCFG_DEADZONE_MASK   (255<<16)
#define S3C_TCFG_DEADZONE_SHIFT  (16)

#define S3C_TCFG1_MUX4_DIV1   (0<<16)
#define S3C_TCFG1_MUX4_DIV2   (1<<16)
#define S3C_TCFG1_MUX4_DIV4   (2<<16)
#define S3C_TCFG1_MUX4_DIV8   (3<<16)
#define S3C_TCFG1_MUX4_DIV16  (4<<16)
#define S3C_TCFG1_MUX4_TCLK1  (5<<16)
#define S3C_TCFG1_MUX4_MASK   (15<<16)
#define S3C_TCFG1_MUX4_SHIFT  (16)

#define S3C_TCFG1_MUX3_DIV1   (0<<12)
#define S3C_TCFG1_MUX3_DIV2   (1<<12)
#define S3C_TCFG1_MUX3_DIV4   (2<<12)
#define S3C_TCFG1_MUX3_DIV8   (3<<12)
#define S3C_TCFG1_MUX3_DIV16  (4<<12)
#define S3C_TCFG1_MUX3_TCLK1  (5<<12)
#define S3C_TCFG1_MUX3_MASK   (15<<12)

#define S3C_TCFG1_MUX2_DIV1   (0<<8)
#define S3C_TCFG1_MUX2_DIV2   (1<<8)
#define S3C_TCFG1_MUX2_DIV4   (2<<8)
#define S3C_TCFG1_MUX2_DIV8   (3<<8)
#define S3C_TCFG1_MUX2_DIV16  (4<<8)
#define S3C_TCFG1_MUX2_TCLK1  (5<<8)
#define S3C_TCFG1_MUX2_MASK   (15<<8)

#define S3C_TCFG1_MUX1_DIV1   (0<<4)
#define S3C_TCFG1_MUX1_DIV2   (1<<4)
#define S3C_TCFG1_MUX1_DIV4   (2<<16)
#define S3C_TCFG1_MUX1_DIV8   (3<<16)
#define S3C_TCFG1_MUX1_DIV16  (4<<16)
#define S3C_TCFG1_MUX1_TCLK1  (5<<16)
#define S3C_TCFG1_MUX1_MASK   (15<<16)

#define S3C_TCFG1_MUX0_DIV2	  (0<<0)
#define S3C_TCFG1_MUX0_DIV4	  (1<<0)
#define S3C_TCFG1_MUX0_DIV8	  (2<<0)
#define S3C_TCFG1_MUX0_DIV16  (3<<0)
#define S3C_TCFG1_MUX0_TCLK0  (4<<0)
#define S3C_TCFG1_MUX0_MASK	  (15<<0)

#define S3C_TCFG1_MUX_DIV2	  (0<<0)
#define S3C_TCFG1_MUX_DIV4	  (1<<0)
#define S3C_TCFG1_MUX_DIV8	  (2<<0)
#define S3C_TCFG1_MUX_DIV16   (3<<0)
#define S3C_TCFG1_MUX_TCLK    (4<<0)
#define S3C_TCFG1_MUX_MASK	  (15<<0)

#define S3C_TCFG1_MUX4_MASK   (15<<16)
#define S3C_TCFG1_SHIFT(x)	  ((x) * 4)

/* WARNING - timer 4 has no buffer reg, and it's observation is at +4 */
#define S3C_TCNTB(tmr)    S3C_TIMERREG2(tmr, 0x00)
#define S3C_TCMPB(tmr)    S3C_TIMERREG2(tmr, 0x04)
#define S3C_TCNTO(tmr)    S3C_TIMERREG2(tmr, (((tmr) == 4) ? 0x04 : 0x08))

#define S3C_TCON_T4RELOAD	  (1<<22)
#define S3C_TCON_T4MANUALUPD  (1<<21)
#define S3C_TCON_T4START	  (1<<20)

#define S3C_TCON_T3RELOAD	  (1<<19)
#define S3C_TCON_T3INVERT	  (1<<18)
#define S3C_TCON_T3MANUALUPD  (1<<17)
#define S3C_TCON_T3START	  (1<<16)

#define S3C_TCON_T2RELOAD	  (1<<15)
#define S3C_TCON_T2INVERT	  (1<<14)
#define S3C_TCON_T2MANUALUPD  (1<<13)
#define S3C_TCON_T2START	  (1<<12)

#define S3C_TCON_T1RELOAD	  (1<<11)
#define S3C_TCON_T1INVERT	  (1<<10)
#define S3C_TCON_T1MANUALUPD	  (1<<9)
#define S3C_TCON_T1START	  (1<<8)

#define S3C_TCON_T0DEADZONE	  (1<<4)
#define S3C_TCON_T0RELOAD	  (1<<3)
#define S3C_TCON_T0INVERT	  (1<<2)
#define S3C_TCON_T0MANUALUPD  (1<<1)
#define S3C_TCON_T0START	  (1<<0)

#define S3C_TINT_CSTAT_T4INT    (1<<9)
#define S3C_TINT_CSTAT_T3INT    (1<<8)
#define S3C_TINT_CSTAT_T2INT    (1<<7)
#define S3C_TINT_CSTAT_T1INT    (1<<6)
#define S3C_TINT_CSTAT_T0INT    (1<<5)
#define S3C_TINT_CSTAT_T4INTEN  (1<<4)
#define S3C_TINT_CSTAT_T3INTEN  (1<<3)
#define S3C_TINT_CSTAT_T2INTEN  (1<<2)
#define S3C_TINT_CSTAT_T1INTEN  (1<<1)
#define S3C_TINT_CSTAT_T0INTEN  (1<<0)

/*
 * Timer interrupt values for FIQ
 */
#define TIMER_FIQ0	23
#define TIMER_FIQ1	24
#define TIMER_FIQ2	25
#define TIMER_FIQ3	27
#define TIMER_FIQ4	28

/*
 * Target Timer configuration
 * (Note: in Linux Timer2 and 4 are used so we may have conflicts)
 * - preescalers=1 divider=1/1 (clock = PCLK = 66MHz)
 */
#define TARGET_USE_TIMER0
// #define TARGET_USE_TIMER1
// #define TARGET_USE_TIMER2
// #define TARGET_USE_TIMER3
// #define TARGET_USE_TIMER4

#ifdef TARGET_USE_TIMER0
#define IRQNO_TIMER							TIMER_FIQ0
#define TARGET_TIMER_PREESCALER_MASK		S3C_TCFG_PRESCALER0_MASK
#define TARGET_PREESCALER_VALUE				(0)
#define TARGET_TIMER_DIVIDER_MASK			S3C_TCFG1_MUX0_MASK
#define TARGET_TIMER_DIVIDER_VALUE			(0)
#define TARGET_TIMER_TCNTB					S3C_TCNTB(0)
#define TARGET_TIMER_CONTROL_MASK			(0xF)
#define TARGET_TIMER_CONTROL_UPDATE			(S3C_TCON_T0RELOAD|S3C_TCON_T0MANUALUPD)
#define TARGET_TIMER_INT_ENABLE				S3C_TINT_CSTAT_T0INTEN
#define TARGET_TIMER_CONTROL_START			(S3C_TCON_T0RELOAD|S3C_TCON_T0START)
#define TARGET_TIMER_TCNTO					S3C_TCNTO(0)
#define TARGET_TIMER_INT_CLEAR				S3C_TINT_CSTAT_T0INT
#endif

#ifdef TARGET_USE_TIMER1
#define IRQNO_TIMER							TIMER_FIQ1
#define TARGET_TIMER_PREESCALER_MASK		S3C_TCFG_PRESCALER0_MASK
#define TARGET_PREESCALER_VALUE				(0)
#define TARGET_TIMER_DIVIDER_MASK			S3C_TCFG1_MUX1_MASK
#define TARGET_TIMER_DIVIDER_VALUE			(0)
#define TARGET_TIMER_TCNTB					S3C_TCNTB(1)
#define TARGET_TIMER_CONTROL_MASK			(0xF<<8)
#define TARGET_TIMER_CONTROL_UPDATE			(S3C_TCON_T1RELOAD|S3C_TCON_T1MANUALUPD)
#define TARGET_TIMER_INT_ENABLE				S3C_TINT_CSTAT_T1INTEN
#define TARGET_TIMER_CONTROL_START			(S3C_TCON_T1RELOAD|S3C_TCON_T1START)
#define TARGET_TIMER_TCNTO					S3C_TCNTO(1)
#define TARGET_TIMER_INT_CLEAR				S3C_TINT_CSTAT_T1INT
#endif

#ifdef TARGET_USE_TIMER2
#define IRQNO_TIMER							TIMER_FIQ2
#define TARGET_TIMER_PREESCALER_MASK		S3C_TCFG_PRESCALER1_MASK
#define TARGET_PREESCALER_VALUE				(0)
#define TARGET_TIMER_DIVIDER_MASK			S3C_TCFG1_MUX2_MASK
#define TARGET_TIMER_DIVIDER_VALUE			(0)
#define TARGET_TIMER_TCNTB					S3C_TCNTB(2)
#define TARGET_TIMER_CONTROL_MASK			(0xF<<12)
#define TARGET_TIMER_CONTROL_UPDATE			(S3C_TCON_T2RELOAD|S3C_TCON_T2MANUALUPD)
#define TARGET_TIMER_INT_ENABLE				S3C_TINT_CSTAT_T2INTEN
#define TARGET_TIMER_CONTROL_START			(S3C_TCON_T2RELOAD|S3C_TCON_T2START)
#define TARGET_TIMER_TCNTO					S3C_TCNTO(2)
#define TARGET_TIMER_INT_CLEAR				S3C_TINT_CSTAT_T2INT
#endif

#ifdef TARGET_USE_TIMER3
#define IRQNO_TIMER							TIMER_FIQ3
#define TARGET_TIMER_PREESCALER_MASK		S3C_TCFG_PRESCALER1_MASK
#define TARGET_PREESCALER_VALUE				(0)
#define TARGET_TIMER_DIVIDER_MASK			S3C_TCFG1_MUX3_MASK
#define TARGET_TIMER_DIVIDER_VALUE			(0)
#define TARGET_TIMER_TCNTB					S3C_TCNTB(3)
#define TARGET_TIMER_CONTROL_MASK			(0xB<<16)
#define TARGET_TIMER_CONTROL_UPDATE			(S3C_TCON_T3RELOAD|S3C_TCON_T3MANUALUPD)
#define TARGET_TIMER_INT_ENABLE				S3C_TINT_CSTAT_T3INTEN
#define TARGET_TIMER_CONTROL_START			(S3C_TCON_T3RELOAD|S3C_TCON_T3START)
#define TARGET_TIMER_TCNTO					S3C_TCNTO(3)
#define TARGET_TIMER_INT_CLEAR				S3C_TINT_CSTAT_T3INT
#endif

#ifdef TARGET_USE_TIMER4
#define IRQNO_TIMER							TIMER_FIQ4
#define TARGET_TIMER_PREESCALER_MASK		S3C_TCFG_PRESCALER1_MASK
#define TARGET_PREESCALER_VALUE				(0)
#define TARGET_TIMER_DIVIDER_MASK			S3C_TCFG1_MUX4_MASK
#define TARGET_TIMER_DIVIDER_VALUE			(0)
#define TARGET_TIMER_TCNTB					S3C_TCNTB(4)
#define TARGET_TIMER_CONTROL_MASK			(0x7<<20)
#define TARGET_TIMER_CONTROL_UPDATE			(S3C_TCON_T4RELOAD|S3C_TCON_T4MANUALUPD)
#define TARGET_TIMER_INT_ENABLE				S3C_TINT_CSTAT_T4INTEN
#define TARGET_TIMER_CONTROL_START			(S3C_TCON_T4RELOAD|S3C_TCON_T4START)
#define TARGET_TIMER_TCNTO					S3C_TCNTO(4)
#define TARGET_TIMER_INT_CLEAR				S3C_TINT_CSTAT_T4INT
#endif

/*
 * Overrun timer
 */

// #define OVRTIMER_SP804_CONTROL_BITS  0x23
// #define OVRTIMER_SP804_CONTROL_ENA   0x80
// #define IRQNO_OVRTIMER         28

#ifndef TOPPERS_MACRO_ONLY

/*
 * コプロへのアクセスマクロ
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

#define CP15_PM_CONTROL_READ(x)  Asm("mrc p15, 0, %0, c15, c12, 0":"=r"(x))
#define CP15_PM_CONTROL_WRITE(x) Asm("mcr p15, 0, %0, c15, c12, 0"::"r"(x))
#define CP15_PM_CYCLE_READ(x)    Asm("mrc p15, 0, %0, c15, c12, 1":"=r"(x))
#define CP15_PM_CYCLE_WRITE(x)   Asm("mcr p15, 0, %0, c15, c12, 1"::"r"(x))
#define CP15_PM_CR0_READ(x)      Asm("mrc p15, 0, %0, c15, c12, 2":"=r"(x))
#define CP15_PM_CR0_WRITE(x)     Asm("mcr p15, 0, %0, c15, c12, 2"::"r"(x))
#define CP15_PM_CR1_READ(x)      Asm("mrc p15, 0, %0, c15, c12, 3":"=r"(x))
#define CP15_PM_CR1_WRITE(x)     Asm("mcr p15, 0, %0, c15, c12, 3"::"r"(x))

#define CP15_PPMR_WRITE(x)       Asm("mcr p15, 0, %0, c15, c2, 4"::"r"(x))

#define CP15_TCM_STATUS_READ(x) Asm("mrc p15, 0, %0, c0, c0, 2":"=r"(x))
#define CP15_TCM_SEL_READ(x)    Asm("mrc p15, 0, %0, c9, c2, 0":"=r"(x))
#define CP15_TCM_SEL_WRITE(x)   Asm("mcr p15, 0, %0, c9, c2, 0"::"r"(x))
#define CP15_DTCM_REG_READ(x)   Asm("mrc p15, 0, %0, c9, c1, 0":"=r"(x))
#define CP15_DTCM_REG_WRITE(x)  Asm("mcr p15, 0, %0, c9, c1, 0"::"r"(x))
#define CP15_ITCM_REG_READ(x)   Asm("mrc p15, 0, %0, c9, c1, 1":"=r"(x))
#define CP15_ITCM_REG_WRITE(x)  Asm("mcr p15, 0, %0, c9, c1, 1"::"r"(x))
#define CP15_DTCM_NS_READ(x)    Asm("mrc p15, 0, %0, c9, c1, 2":"=r"(x))
#define CP15_DTCM_NS_WRITE(x)   Asm("mcr p15, 0, %0, c9, c1, 2"::"r"(x))
#define CP15_ITCM_NS_READ(x)    Asm("mrc p15, 0, %0, c9, c1, 3":"=r"(x))
#define CP15_ITCM_NS_WRITE(x)   Asm("mcr p15, 0, %0, c9, c1, 3"::"r"(x))

/*
 * TCM mapping addresses (continuously mapped +0x1000 (4Kb))
 * NOTE: must be the same as in idea6410nt.h
 */

#define TCM_BASE_ADDRESS  0x80000000
#define ITCM0_ADDRESS     TCM_BASE_ADDRESS
#define ITCM1_ADDRESS     (TCM_BASE_ADDRESS + 0x2000) /* +8KB */
#define DTCM0_ADDRESS     (TCM_BASE_ADDRESS + 0x4000) /* +8KB */
#define DTCM1_ADDRESS     (TCM_BASE_ADDRESS + 0x6000) /* +8KB */

/*
 * コプロのビット定義
 */
#define CP15_CONTROL_XP_BIT   (1 << 23)
#define CP15_CONTROL_I_BIT    (1 << 12)
#define CP15_CONTROL_C_BIT    (1 <<  2)
#define CP15_CONTROL_M_BIT    (1 <<  0)

#define CP15_TTB0_C           (1 << 0)
#define CP15_TTB0_S           (1 << 1)
#define CP15_TTB0_RGN_WBWA    (1 << 3)

/*
* UART Control Registers
*/
typedef volatile unsigned long	vu_long;
typedef volatile unsigned char	vu_char;

typedef vu_char		S3C64XX_REG8;
typedef vu_long		S3C64XX_REG32;

typedef struct {
	S3C64XX_REG32	ULCON;
	S3C64XX_REG32	UCON;
	S3C64XX_REG32	UFCON;
	S3C64XX_REG32	UMCON;
	S3C64XX_REG32	UTRSTAT;
	S3C64XX_REG32	UERSTAT;
	S3C64XX_REG32	UFSTAT;
	S3C64XX_REG32	UMSTAT;
	S3C64XX_REG8	UTXH;
	S3C64XX_REG8	res1[3];
	S3C64XX_REG8	URXH;
	S3C64XX_REG8	res2[3];
	S3C64XX_REG32	UBRDIV;
	S3C64XX_REG32	UDIVSLOT;
} S3C64XX_UART;

/*
 * コプロを用いたルーチン
 */

/*
 *  Data Synchronization Barrier
 *  ・先に発行された read と write の終了を待つ
 *  ・キャッシュ，ブランチプリディクション，TLBの操作の終了を待つ
 *  ・この命令の後に書かれた命令は実行されない
 */
Inline void
idea6410_data_sync_barrier(void){
    CP15_DATA_SYNC_BARRIER();
}

/*
 * Data Memory Barrier
 * ・プログラムの記述に従って，先に書かれた命令でのメモリアクセスが
 *   終了するまで待つ．
 *
 */
Inline void
idea6410_data_memory_barrier(void){
    CP15_DATA_MEMORY_BARRIER();
}

/*
 * TLBの無効化
 */
Inline void
idea6410_invalidate_unfied_tlb(void){
    CP15_DATA_SYNC_BARRIER();
}


/*
 * Dキャッシュの無効化
 */
Inline void
idea6410_dcache_invalidate(void){
    CP15_DCACHE_INVALIDATE();
}


/*
 * Dキャッシュのクリーンと無効化
 */
Inline void
idea6410_dcache_clean_and_invalidate(void){
    CP15_DCACHE_CLEAN_AND_INVALIDATE();
}


/*
 * Iキャッシュの無効化
 */
Inline void
idea6410_icache_invalidate(void){
    CP15_ICACHE_INVALIDATE();
}

/*
 * プリフェッチバッファをクリア
 */
Inline void
idea6410_pbuffer_flash(void){
    CP15_PBUFFER_FLUSH();
}

/*
 * TZIC操作関数
 */

/*
 * レジスタのアンロック
 */
Inline void
tzic0_unlock(void){
	sil_wrw_mem((void *)TZIC0_LOCK,  TZIC_UNLOCK_CODE);
}

Inline void
tzic1_unlock(void){
	sil_wrw_mem((void *)TZIC1_LOCK,  TZIC_UNLOCK_CODE);
}

/*
 * レジスタのロック
 */
Inline void
tzic0_lock(void){
	sil_wrw_mem((void *)TZIC0_LOCK,  TZIC_LOCK_CODE);
}

Inline void
tzic1_lock(void){
	sil_wrw_mem((void *)TZIC1_LOCK,  TZIC_LOCK_CODE);
}

/*
 * 割込み要求をスルーしてirqから出力する
 */
Inline void
tzic_pass_through_irqout(uint64_t mask){
	uint32_t mask0 = (uint32_t)(mask);
	uint32_t mask1 = (uint32_t)(mask >> 32);

	tzic0_unlock();
	sil_wrw_mem((void *)TZIC0_INTSELECT,
				sil_rew_mem((void *)TZIC0_INTSELECT) &  ~mask0);
	tzic0_lock();

	tzic1_unlock();
	sil_wrw_mem((void *)TZIC1_INTSELECT,
				sil_rew_mem((void *)TZIC1_INTSELECT) &  ~mask1);
	tzic1_lock();
}

/*
 * 割込み要求のマスク
 */
Inline void
tzic_disable_int(uint64_t mask)
{
	uint32_t mask0 = (uint32_t)(mask);
	uint32_t mask1 = (uint32_t)(mask >> 32);
	
	tzic0_unlock();
	sil_wrw_mem((void *)TZIC0_CLEAR,
				sil_rew_mem((void *)TZIC0_CLEAR) | mask0);
	tzic0_lock();

	tzic1_unlock();
	sil_wrw_mem((void *)TZIC1_CLEAR,
				sil_rew_mem((void *)TZIC1_CLEAR) | mask1);
	tzic1_lock();
}

/*
 * 割込み要求のマスクの解除
 */
Inline void
tzic_enable_int(uint64_t mask)
{
	uint32_t mask0 = (uint32_t)(mask);
	uint32_t mask1 = (uint32_t)(mask >> 32);

	tzic0_unlock();
    sil_wrw_mem((void *)TZIC0_ENABLE,
				sil_rew_mem((void *)TZIC0_ENABLE) | mask0);
	tzic0_lock();

	tzic1_unlock();
    sil_wrw_mem((void *)TZIC1_ENABLE,
				sil_rew_mem((void *)TZIC1_ENABLE) | mask1);
	tzic1_lock();
}

/*
 * 割込み要求のクリア
 */
Inline void
tzic_clear_int(uint64_t mask)
{

}

/*
 * 割込み要求のチェック
 */
Inline bool_t
tzic_probe_int(uint64_t mask)
{
	uint32_t mask0 = (uint32_t)(mask);
	uint32_t mask1 = (uint32_t)(mask >> 32);

    return ((sil_rew_mem((void *)TZIC0_STATUS) & mask0) == mask0) ||
           ((sil_rew_mem((void *)TZIC1_STATUS) & mask1) == mask1);
}

/*
 * Interrupt Raw status (before masked)
 */
Inline bool_t
tzic_probe_raw_int(uint64_t mask)
{
	uint32_t mask0 = (uint32_t)(mask);
	uint32_t mask1 = (uint32_t)(mask >> 32);

    return ((sil_rew_mem((void *)TZIC0_RAWSTATUS) & mask0) == mask0) ||
		   ((sil_rew_mem((void *)TZIC1_RAWSTATUS) & mask1) == mask1);
}

Inline uint64_t
tzic_get_raw_status()
{
	uint32_t status0 = sil_rew_mem((void *)TZIC0_RAWSTATUS);
	uint32_t status1 = sil_rew_mem((void *)TZIC1_RAWSTATUS);
	
	return (uint64_t) status1 << 32 | status0;
}

/*
 * FIQの発生源として用いる
 */
Inline void
tzic_use_fiq_gen(uint64_t mask)
{
	uint32_t mask0 = (uint32_t)(mask);
	uint32_t mask1 = (uint32_t)(mask >> 32);

	tzic0_unlock();
	sil_wrw_mem((void *)TZIC0_INTSELECT,
				sil_rew_mem((void *)TZIC0_INTSELECT) | mask0);
	tzic0_lock();

	tzic1_unlock();
	sil_wrw_mem((void *)TZIC1_INTSELECT,
				sil_rew_mem((void *)TZIC1_INTSELECT) | mask1);
	tzic1_lock();
}

#define PCLK_GATE	(0x7e00f034)

Inline void
idea6410t_tzpc_init(void)
{
	uint32_t tzpc_set;

    *((volatile unsigned int *)PCLK_GATE) = 0xFFFFFFFF;
	
	// Set TZPC protection size (Note: don't know which memory is protecting)
	*((volatile unsigned int *) TZPCR0SIZE) = 0x1FF;;

	// Set all devices as non-trust (TODO: modify this)
	// Note: there should be only 3 registers but it didnt work until i set
	// all of the memory until the next peripheral to 0xFFFFFFFFF
	for (tzpc_set = TZPCDECPROT0Set; tzpc_set < 0x7F000FE0; tzpc_set += 12)
		*((volatile unsigned int *)tzpc_set) = 0xFFFFFFFF;
}

/*
 * カーネル起動時のログ出力用の初期化
 */
Inline void
idea6410t_init_uart(void){
	S3C64XX_UART *const uart =  (S3C64XX_UART *)IDEA6410_PUTC_BASE_REG;
	uart->UFCON = 0;   // FIFO disabled
	uart->UMCON = 0;   // RTS AFC disabled
	uart->ULCON = 0x3; // 8-N-1
	uart->UCON = 0xe45;  // polling mode - external clock: DIV_VAL = (53213366÷(115200×16))‐1=27.87
	uart->UBRDIV = 0x22; // integer part of div_val = 27
	uart->UDIVSLOT = 0x1FFF; // defined(CONFIG_UART_66)
}

/*
 *  UARTからのポーリング出力
 */
Inline void
idea6410t_putc(char_t c)
{
	S3C64XX_UART *const uart =  (S3C64XX_UART *)IDEA6410_PUTC_BASE_REG;
	while (!(uart->UTRSTAT & 0x2));
	uart->UTXH = c;
}

/*
*  受信した文字の取り出し
*/
Inline uint8_t
idea6410t_getc()
{
	S3C64XX_UART *const uart =  (S3C64XX_UART *)IDEA6410_PUTC_BASE_REG;
	while (!(uart->UTRSTAT & 0x1));
	return uart->URXH & 0xff;
}

/*
 * TCM initialization (after initialization, cache must be invalidated)
 */
Inline void
idea6410_init_tcm(void)
{
	// Set DTCM and ITCM
	CP15_TCM_SEL_WRITE(0);
	CP15_DTCM_NS_WRITE(0);
	CP15_ITCM_NS_WRITE(0);

	CP15_TCM_SEL_WRITE(1);
	CP15_DTCM_NS_WRITE(0);
	CP15_ITCM_NS_WRITE(0);

	// Map TCM and enable
	CP15_TCM_SEL_WRITE(0);
	CP15_ITCM_REG_WRITE(ITCM0_ADDRESS | 1);
	CP15_DTCM_REG_WRITE(DTCM0_ADDRESS | 1);
	
	CP15_TCM_SEL_WRITE(1);
	CP15_ITCM_REG_WRITE(ITCM1_ADDRESS | 1);
	CP15_DTCM_REG_WRITE(DTCM1_ADDRESS | 1);
}

/*
 * 終了処理
 */
Inline void
idea6410t_exit(void)
{

}

#endif /* TOPPPERS_MACRO_ONLY */
#endif /* TOPPERS_IDEA6410T_H */
