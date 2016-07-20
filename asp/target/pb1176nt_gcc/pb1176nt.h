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

#ifndef TOPPERS_PB1176NT_H
#define TOPPERS_PB1176NT_H

#include <sil.h>


/*
 * 割込み待ち命令
 */
#define ASM_TARGET_WAIT_INTERRUPT nop

/*
 *  GICに関する定義
 *  PB1176のテストチップ上のGICは，MPCoreのDICをベースとしている．
 */

/*
 *  Distributed Interrupt Controller CPU Interface
 *
 *  優先度は，0x0〜0xF の16段階．
 *  0x0が最高優先度で，0xFが最低優先度．
 */
#define GIC_BASE 0x10120000

#define GIC_CPUIF_CR   (GIC_BASE + 0x0000)  /* コントロールレジスタ */
#define GIC_CPUIF_PMR  (GIC_BASE + 0x0004)  /* 優先度マスクレジスタ */
#define GIC_CPUIF_BPR  (GIC_BASE + 0x0008)  /* バイナリポイントレジスタ */
#define GIC_CPUIF_IAR  (GIC_BASE + 0x000C)  /* Interrupt Acknowledge Register */
#define GIC_CPUIF_EOI  (GIC_BASE + 0x0010)  /* End of Interrupt Register */
#define GIC_CPUIF_RII  (GIC_BASE + 0x0014)  /* Running Interrupt Register */

#define GIC_CPUIF_CR_EN      0x01
#define GIC_CPUIF_PMR_OFFSET 0x04
#define GIC_CPUIF_BPR_ALL    0x03  /* 全てのビットが有効 */
#define GIC_CPUIF_BPR_3      0x04  /* 3ビット有効        */
#define GIC_CPUIF_BPR_2      0x05  /* 2ビット有効        */
#define GIC_CPUIF_BPR_1      0x06  /* 1ビット有効        */

/*
 *  Distributed Interrupt Controller
 */
#define GIC_CR     (GIC_BASE + 0x1000)  /* コントロールレジスタ */
#define GIC_SE     (GIC_BASE + 0x1100)  /* 割込みイネーブルセット   */
#define GIC_CE     (GIC_BASE + 0x1180)  /* 割込みイネーブルクリアー */
#define GIC_SP     (GIC_BASE + 0x1200)  /* 割込みセットペンディング */
#define GIC_CP     (GIC_BASE + 0x1280)  /* 割込みクリアーペンディング */
#define GIC_IPR    (GIC_BASE + 0x1400)  /* 割込み優先度レジスタ */
#define GIC_ITR    (GIC_BASE + 0x1800)  /* 割込みターゲットレジスタ */
#define GIC_ICR    (GIC_BASE + 0x1C00)  /* 割込みコンフィギュレーションレジスタ */
#define GIC_SIR    (GIC_BASE + 0x1F00)  /* ソフトウェア割込みレジスタ  */

#define GIC_CR_ENABLE 0x01

/*
 *  割込み先のプロセッサの指定
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
 *  割込み優先度
 */
#define GIC_MAX_PRIORITY  0x0
#define GIC_MIN_PRIORITY  0xf

/*
 *  コンフィギュレーションレジスタの設定値
 */
#define GIC_ICR_EDGE     0x03   /* エッジ割込み */
#define GIC_ICR_LEVEL    0x01   /* レベル割込み */

/*
 *  GICでサポートしている割込み数
 */
#define GIC_TMIN_INTNO      0U
#define GIC_TMAX_INTNO     63U
#define GIC_TNUM_INT       64U

#ifndef TOPPERS_MACRO_ONLY
/*
 *  GIC CPUインタフェース関連のドライバ
 */
/*
 *  CPUの割込み優先度マスクを設定
 */
Inline void
gic_cpu_set_priority(int pri)
{
	sil_wrw_mem((void *)GIC_CPUIF_PMR,
                (pri << GIC_CPUIF_PMR_OFFSET));
}

/*
 *  CPUの割込み優先度マスクを取得
 */
Inline int
gic_cpu_current_priority(void)
{
    return (sil_rew_mem((void *)(GIC_CPUIF_PMR)) >>
            GIC_CPUIF_PMR_OFFSET);
}

/*
 *  割込みハンドラの出口で必要なIRC操作
 */
Inline void
gic_cpu_end_int(uint8_t id)
{
    sil_wrw_mem((void *)(GIC_CPUIF_EOI), id);
}

/*
 *  GICのプロセッサの割込み優先度のどのビットを使用するか
 */
Inline void
gic_cpu_set_bp(int mask_bit)
{
	sil_wrw_mem((void *)GIC_CPUIF_BPR, mask_bit);
}

/*
 *  CPUインタフェースの初期化
 */
extern void gic_cpu_init(void);

/*
 *  CPUインタフェースの終了
 */
extern void gic_cpu_stop(void);

/*
 *  GIC DISインタフェース関連のドライバ
 */

/*
 *  割込み禁止
 */
extern void gic_disable_int(uint8_t id);

/*
 *  割込み許可
 */
extern void gic_enable_int(uint8_t id);

/*
 *  割込みペンディングクリア
 */
extern void gic_clear_pending(uint8_t id);

/*
 *  割込みペンディングセット
 */
extern void gic_set_pending(uint8_t id);

/*
 *  割込み要求のチェック
 */
extern bool_t gic_probe_int(uint8_t id);

/*
 *  割込み設定のセット
 */
extern void gic_config(uint8_t id, uint8_t config);

/*
 *  割込み優先度のセット
 *  内部表現で渡す．
 */
extern void gic_set_priority(uint8_t id, int pri);

/*
 *  GIC割込みターゲットの設定
 *  CPUはORで指定
 */
extern void gic_set_target(uint8_t id, uint8_t cpus);

/*
 *  GICインタフェースの初期化
 */
extern void gic_init(void);

/*
 *  GICインタフェースの終了
 */
extern void gic_stop(void);

#endif /* TOPPERS_MACRO_ONLY */

/*
 * 割込みハンドラ番号から，IRC操作のためのビットパターンを求めるマクロ
 */
#define INTNO_BITPAT(intno) (1U << intno)


/*
 *  UARTに関する定義（UART0を使用）
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

#define UART_IMSC_RXIM 0x0010 /* 受信割込みマスク          */
#define UART_IMSC_TXIM 0x0020 /* 送信割込みマスク          */

/*
 *  UART割込み番号
 */
#define IRQNO_UART       50    /* 18 for TZIC */


/*
 *  タイマー関連(Timer01を使用)
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
 * TCM mapping addresses (continuously mapped +0x1000 (4Kb))
 * NOTE: must be the same as in pb1176t.h
 */

#define TCM_BASE_ADDRESS  0x70000000
#define ITCM0_ADDRESS     TCM_BASE_ADDRESS
#define ITCM1_ADDRESS     (TCM_BASE_ADDRESS + 0x1000)
#define DTCM0_ADDRESS     (TCM_BASE_ADDRESS + 0x2000)
#define DTCM1_ADDRESS     (TCM_BASE_ADDRESS + 0x3000)

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
 * カーネル起動時のログ出力用の初期化
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
 *  UARTからのポーリング出力
 */
Inline void
pb1176nt_putc(char_t c)
{
	while((sil_rew_mem((void *)(UART_BASE_REG + UART_PL011_FR)) & UFR_TXFF) == UFR_TXFF);
	sil_wrw_mem((void *)(UART_BASE_REG + UART_PL011_DR), c);
}


/*
 * 終了処理
 */
Inline void
pb1176nt_exit(void)
{

}

#endif /* TOPPPERS_MACRO_ONLY */
#endif /* TOPPERS_PB1176NT_H */
