/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2010 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: kzm_ca9.c 599 2010-05-07 08:00:38Z ertl-honda $
 */

#include "kernel_impl.h"
//#include "pdic/arm_primecell_uart/arm_primecell_uart.h"
#include "tl16c554a.h"
#include "target_timer.h"
#include "kzm_ca9.h"
#ifdef G_SYSLOG

//#define my_prc_uart_base	UART0_BASE
#define my_prc_uart_base	UART2_CSA_BASE

#else

/*
 *  低レベル出力でプロセッサ毎に使用するUARTのアドレス
 */
static const uint32_t prc_uart_base_table[TNUM_PRCID] = {
	UART0_BASE,
#if TNUM_PRCID >= 2
	UART1_BASE,
#endif /* TNUM_PRCID >= 2 */
#if TNUM_PRCID >= 3
	UART2_BASE,  
#endif /* TNUM_PRCID >= 3 */
#if TNUM_PRCID >= 4
	UART3_BASE,
#endif /* TNUM_PRCID >= 4 */
};

/*
 *  自プロセッサが使用するUARTのアドレス
 */
#define my_prc_uart_base	(prc_uart_base_tbl[x_prc_index()])

#endif /* G_SYSLOG */



/*
 *  UARTからのポーリング出力
 */
void
kzm_ca9_putc(char_t c){
	/* バッファが空くのを待つ */
	while ((sil_rew_mem((void *)(my_prc_uart_base + TL16C554A_LSR)) & LSR_THRE) == 0);
	/* 書き込み */
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_THR), c);

	/* バッファが空くのを待つ */
	while ((sil_rew_mem((void *)(my_prc_uart_base + TL16C554A_LSR)) & LSR_THRE) == 0);


}

/*
 * ポーリング出力のための初期化
 */
void
kzm_ca9_uart_init(void){

	/* 割り込み禁止 */
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_IER), 0x00);

	/* Access baud rate */
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_LCR), LCR_BKSE);
    
	/* ボーレートを設定 */
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_DLL), 12 & 0xff);
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_DLM), (12 >> 8) & 0xff);

    	/* 8 data, 1 stop, no parity */
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_LCR), LCR_8N1);

	/* no RTS/DTR */
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_MCR), MCR_VAL);

	/* Clear & enable FIFOs */
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_FCR), FCR_VAL);

}

/*
 *  TestChip内蔵タイマの初期化
 */
void
timer_sp804_init(void)
{
#if 0
	/* タイマ停止 */
	sil_wrw_mem((void *)(TIMER_BASE_REG + TIMER_SP804_1CONTROL), 0x00);
	
	/* カウント値をセット */
	sil_wrw_mem((void *)(TIMER_BASE_REG + TIMER_SP804_1LOAD), 0xffffffff);

	/* タイマスタート，フリーランニング，分周なし，32bitカウンタ */
	sil_wrw_mem((void *)(TIMER_BASE_REG + TIMER_SP804_1CONTROL),
				(TIMER_SP804_CTRL_TIMER_EN|TIMER_SP804_CTRL_TIMESIZE_32));
#endif
}
