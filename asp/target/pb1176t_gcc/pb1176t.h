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
 *  @(#) $Id: cm922t.h 264 2007-07-12 08:23:10Z hiro $
 */

#ifndef TOPPERS_PB1176T_H
#define TOPPERS_PB1176T_H

#include <sil.h>
#include "../../../safeg/safeg_measures.h" /* TODO: improve this */

/*
 * 割込み待ち命令
 */
#define ASM_TARGET_WAIT_INTERRUPT nop


/*
 *  TZICに関する定義
 */
#define TZIC_BASE 0x1011f000

#define TZIC_STATUS      (TZIC_BASE + 0x00)
#define TZIC_RAWSTATUS   (TZIC_BASE + 0x04)
#define TZIC_INTSELECT   (TZIC_BASE + 0x08)
#define TZIC_ENABLE      (TZIC_BASE + 0x0C)
#define TZIC_CLEAR       (TZIC_BASE + 0x10)
#define TZIC_PROTECTION  (TZIC_BASE + 0x18)
#define TZIC_LOCK        (TZIC_BASE + 0x1C)
#define TZIC_LOCK_STATUS (TZIC_BASE + 0x20)

#define TZIC_UNLOCK_CODE 0x0ACCE550
#define TZIC_LOCK_CODE   0x00000000

/*
 * FPGA tzpc registers
 */
#define FPGA_TZPCDECPROT0Stat    0x10003800
#define FPGA_TZPCDECPROT0Set     0x10003804
#define FPGA_TZPCDECPROT0Clr     0x10003808
#define FPGA_TZPCDECPROT1Stat    0x1000380C
#define FPGA_TZPCDECPROT1Set     0x10003810
#define FPGA_TZPCDECPROT1Clr     0x10003814
#define FPGA_TZPCDECPROT2Stat    0x10003818
#define FPGA_TZPCDECPROT2Set     0x1000381C
#define FPGA_TZPCDECPROT2Clr     0x10003820
#define FPGA_TZPCDECPROT3Stat    0x10003824
#define FPGA_TZPCDECPROT3Set     0x10003828
#define FPGA_TZPCDECPROT3Clr     0x1000382C

/*
* dev-chip tzpc registers
*/
#define TZPCR0SIZE          0x10101000
#define TZPCDECPROT0Stat    0x10101800
#define TZPCDECPROT0Set     0x10101804
#define TZPCDECPROT0Clr     0x10101808
#define TZPCDECPROT1Stat    0x1010180C
#define TZPCDECPROT1Set     0x10101810
#define TZPCDECPROT1Clr     0x10101814
#define TZPCDECPROT2Stat    0x10101818
#define TZPCDECPROT2Set     0x1010181C
#define TZPCDECPROT2Clr     0x10101820

/*
 * 割込みハンドラ番号から，IRC操作のためのビットパターンを求めるマクロ
 */
#define INTNO_BITPAT(intno) (1U << intno)


/*
 *  UARTに関する定義
 */
#if 0
#define UART0_BASE_REG    0x1010c000
#define UART1_BASE_REG    0x1010d000
#define UART2_BASE_REG    0x1010e000
#define UART3_BASE_REG    0x1010f000
#endif

/*
 *  linux が UART0 を使用するため, ボードの UART1, UART2 を
 *  ASP の SIO0, SIO1 として使用する
 */
#define UART0_BASE_REG    0x1010d000
#define UART1_BASE_REG    0x1010e000

/*
 *  ポーリング出力で使用するポート
 */
#define PB1176_PUTC_BASE_REG UART0_BASE_REG

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

#define UART_IMSC_RXIM 0x0010 /* 受信割込みマスク          */
#define UART_IMSC_TXIM 0x0020 /* 送信割込みマスク          */

/*
 *  UART割込み番号
 */
#define IRQNO_UART0    18
#define IRQNO_UART1    19
#define IRQNO_UART2    20
#define IRQNO_UART3    21

/*
 *  タイマー関連
 */
#define TIMER_BASE_REG         0x10105000
#define OVRTIMER_BASE_REG      0x10106000

#define TIMER_SP804_1LOAD		0x000
#define TIMER_SP804_1VALUE		0x004
#define TIMER_SP804_1CONTROL	0x008
#define TIMER_SP804_1CLEAR		0x00C
#define TIMER_SP804_1RIS		0x010
#define TIMER_SP804_2LOAD		0x020
#define TIMER_SP804_2VALUE		0x024
#define TIMER_SP804_2CONTROL	0x028
#define TIMER_SP804_2CLEAR		0x02C

#define OVRTIMER_SP804_CONTROL_BITS  0x23
#define OVRTIMER_SP804_CONTROL_ENA   0x80

#if ASP_MEASURE_TIMER_LAT == 1
#define IRQNO_TIMER            7 /* performance measurement timer interrupt */
#else
#define IRQNO_TIMER            9
#endif
#define IRQNO_OVRTIMER         10

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

#define CP15_PM_CONTROL_READ(x) Asm("mrc p15, 0, %0, c15, c12, 0":"=r"(x))
#define CP15_PM_CONTROL_WRITE(x) Asm("mcr p15, 0, %0, c15, c12, 0"::"r"(x))
#define CP15_PM_CYCLE_READ(x) Asm("mrc p15, 0, %0, c15, c12, 1":"=r"(x))
#define CP15_PM_CYCLE_WRITE(x) Asm("mcr p15, 0, %0, c15, c12, 1"::"r"(x))
#define CP15_PM_CR0_READ(x) Asm("mrc p15, 0, %0, c15, c12, 2":"=r"(x))
#define CP15_PM_CR0_WRITE(x) Asm("mcr p15, 0, %0, c15, c12, 2"::"r"(x))
#define CP15_PM_CR1_READ(x) Asm("mrc p15, 0, %0, c15, c12, 3":"=r"(x))
#define CP15_PM_CR1_WRITE(x) Asm("mcr p15, 0, %0, c15, c12, 3"::"r"(x))

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

#define CP15_VALIDATION_REG_WRITE(x)  Asm("mcr p15, 0, %0, c15, c9, 0"::"r"(x))

/*
 * TCM mapping addresses (continuously mapped +0x1000 (4Kb))
 * NOTE: must be the same as in pb1176nt.h
 */

#define TCM_BASE_ADDRESS  0x70000000
#define ITCM0_ADDRESS     TCM_BASE_ADDRESS
#define ITCM1_ADDRESS     (TCM_BASE_ADDRESS + 0x1000)
#define DTCM0_ADDRESS     (TCM_BASE_ADDRESS + 0x2000)
#define DTCM1_ADDRESS     (TCM_BASE_ADDRESS + 0x3000)

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
 * コプロを用いたルーチン
 */

/*
 *  Data Synchronization Barrier
 *  ・先に発行された read と write の終了を待つ
 *  ・キャッシュ，ブランチプリディクション，TLBの操作の終了を待つ
 *  ・この命令の後に書かれた命令は実行されない
 */
Inline void
pb1176_data_sync_barrier(void){
    CP15_DATA_SYNC_BARRIER();
}

/*
 * Data Memory Barrier
 * ・プログラムの記述に従って，先に書かれた命令でのメモリアクセスが
 *   終了するまで待つ．
 *
 */
Inline void
pb1176_data_memory_barrier(void){
    CP15_DATA_MEMORY_BARRIER();
}

/*
 * TLBの無効化
 */
Inline void
pb1176_invalidate_unfied_tlb(void){
    CP15_DATA_SYNC_BARRIER();
}


/*
 * Dキャッシュの無効化
 */
Inline void
pb1176_dcache_invalidate(void){
    CP15_DCACHE_INVALIDATE();
}


/*
 * Dキャッシュのクリーンと無効化
 */
Inline void
pb1176_dcache_clean_and_invalidate(void){
    CP15_DCACHE_CLEAN_AND_INVALIDATE();
}


/*
 * Iキャッシュの無効化
 */
Inline void
pb1176_icache_invalidate(void){
    CP15_ICACHE_INVALIDATE();
}

/*
 * プリフェッチバッファをクリア
 */
Inline void
pb1176_pbuffer_flash(void){
    CP15_PBUFFER_FLUSH();
}

/*
 * TZIC操作関数
 */

/*
 * レジスタのアンロック
 */
Inline void
tzic_unlock(void){
	sil_wrw_mem((void *)TZIC_LOCK,  TZIC_UNLOCK_CODE);
}

/*
 * レジスタのロック
 */
Inline void
tzic_lock(void){
	sil_wrw_mem((void *)TZIC_LOCK,  TZIC_LOCK_CODE);
}

/*
 * 割込み要求をスルーしてirqから出力する
 */
Inline void
tzic_pass_through_irqout(uint32_t mask){
	tzic_unlock();

	sil_wrw_mem((void *)TZIC_INTSELECT,
				sil_rew_mem((void *)TZIC_INTSELECT) &  ~mask);
	tzic_lock();
}

/*
 * 割込み要求のマスク
 */
Inline void
tzic_disable_int(uint32_t mask)
{
	tzic_unlock();
	sil_wrw_mem((void *)TZIC_CLEAR,
				sil_rew_mem((void *)TZIC_CLEAR) | mask);
	tzic_lock();
}

/*
 * 割込み要求のマスクの解除
 */
Inline void
tzic_enable_int(uint32_t mask)
{
	tzic_unlock();
    sil_wrw_mem((void *)TZIC_ENABLE,
				sil_rew_mem((void *)TZIC_ENABLE) | mask);
	tzic_lock();
}

/*
 * 割込み要求のクリア
 */
Inline void
tzic_clear_int(uint32_t mask)
{

}

/*
 * 割込み要求のチェック
 */
Inline bool_t
tzic_probe_int(uint32_t mask)
{
    return((sil_rew_mem((void *)TZIC_STATUS) & mask) == mask);
}

/*
 * Interrupt Raw status (before masked)
 */
Inline bool_t
tzic_probe_raw_int(uint32_t mask)
{
    return((sil_rew_mem((void *)TZIC_RAWSTATUS) & mask) == mask);
}

Inline uint32_t
tzic_get_raw_status()
{
    return sil_rew_mem((void *)TZIC_RAWSTATUS);
}

/*
 * FIQの発生源として用いる
 */
Inline void
tzic_use_fiq_gen(uint32_t mask){
    tzic_unlock();
    sil_wrw_mem((void *)TZIC_INTSELECT,
				sil_rew_mem((void *)TZIC_INTSELECT)| mask);
    tzic_lock();
}

Inline void
pb1176t_tzpc_init(void)
{
	/*
	*  all the 512KB SRAM for SafeG + T-ASP is secure
	*  realview: ce ((unsigned long *)S:0x10101000)[0]=0xFF
	*/
	*((unsigned int *) TZPCR0SIZE) = 0xFF;
	
	/*
	* dev-chip tzpc is limited nonsecure setting
	* timers 2/3/4/5, uart 1,2 and tzic are secure
	* realview:
	* ce ((unsigned long *)S:0x10101804)[0]=0x9F9D
	* ce ((unsigned long *)S:0x10101810)[0]=0x3FFF
	* ce ((unsigned long *)S:0x1010181C)[0]=0x007F
	*/
	*((unsigned int *) TZPCDECPROT0Set) = 0x9F9D;
	*((unsigned int *) TZPCDECPROT1Set) = 0x3FFF;
	*((unsigned int *) TZPCDECPROT2Set) = 0x007F;
	
	/*
	* fpga tzpc is all non-secure setting
	* realview:
	* ce ((unsigned long *)S:0x10003804)[0]=0x00FF
	* ce ((unsigned long *)S:0x10003810)[0]=0x00FF
	* ce ((unsigned long *)S:0x1000381C)[0]=0x000F
	*/
	*((unsigned int *) FPGA_TZPCDECPROT0Set) = 0x00FF;
	*((unsigned int *) FPGA_TZPCDECPROT1Set) = 0x00FF;
	*((unsigned int *) FPGA_TZPCDECPROT2Set) = 0x000F;
}

/*
 * カーネル起動時のログ出力用の初期化
 */
Inline void
pb1176t_init_uart(void)
{
	/*
	 * setup baud rate divisor (38400bps)
	 */
	sil_wrw_mem((void *)(PB1176_PUTC_BASE_REG + UART_PL011_IBRD), 0x27);
	sil_wrw_mem((void *)(PB1176_PUTC_BASE_REG + UART_PL011_FBRD), 0x04);

	/* refer to the TRM for details */
	sil_wrw_mem((void *)(PB1176_PUTC_BASE_REG + UART_PL011_CR), 0x301);
	sil_wrw_mem((void *)(PB1176_PUTC_BASE_REG + UART_PL011_LCR_H), 0x060);
	sil_wrw_mem((void *)(PB1176_PUTC_BASE_REG + UART_PL011_IFLS), 0x000);

	/* Mask all interrupt */
	sil_wrw_mem((void *)(PB1176_PUTC_BASE_REG + UART_PL011_IMSC), 0);
}

/*
 *  UARTからのポーリング出力
 */
Inline void
pb1176t_putc(char_t c)
{
	while((sil_rew_mem((void *)(PB1176_PUTC_BASE_REG + UART_PL011_FR)) & UFR_TXFF) == UFR_TXFF);
	sil_wrw_mem((void *)(PB1176_PUTC_BASE_REG + UART_PL011_DR), c);
}

/*
 * TCM initialization (after initialization, cache must be invalidated)
 */
Inline void
pb1176t_init_tcm(void)
{
	// Set both DTCM and ITCM to Non-Secure
	CP15_TCM_SEL_WRITE(0);
	CP15_DTCM_NS_WRITE(1);
	CP15_ITCM_NS_WRITE(1);

	CP15_TCM_SEL_WRITE(1);
	CP15_DTCM_NS_WRITE(1);
	CP15_ITCM_NS_WRITE(1);

	// Map TCM and enable
	CP15_TCM_SEL_WRITE(0);
	CP15_DTCM_REG_WRITE(DTCM0_ADDRESS | 1);
	CP15_ITCM_REG_WRITE(ITCM0_ADDRESS | 1);

	CP15_TCM_SEL_WRITE(1);
	CP15_DTCM_REG_WRITE(DTCM1_ADDRESS | 1);
	CP15_ITCM_REG_WRITE(ITCM1_ADDRESS | 1);
}

/*
 * 終了処理
 */
Inline void
pb1176t_exit(void)
{

}

#endif /* TOPPPERS_MACRO_ONLY */
#endif /* TOPPERS_CM922T_H */
