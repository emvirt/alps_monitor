/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2008-2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_config.c 458 2009-11-04 09:05:06Z ertl-honda $
 */

/*
 * チップ依存モジュール（NIOS2_DEV用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "jtag_uart.h"
#include "mp.h"

/*
 *  target_fput_log() で用いるUARTのベースアドレス
 */
const uint32_t target_fput_log_base_table[TNUM_PRCID] = {
	JTAG_UART_1_BASE,
#if TNUM_PRCID >= 2
	JTAG_UART_2_BASE,
#endif /* TNUM_PRCID >= 2 */
#if TNUM_PRCID >= 3
	JTAG_UART_3_BASE,
#endif /* TNUM_PRCID >= 3 */
#if TNUM_PRCID >= 4
	JTAG_UART_4_BASE,
#endif /* TNUM_PRCID >= 4 */
};

/*
 *  プロセッサ間割込みHWのベースアドレス
 */
const uint32_t target_prc_int_base_table[TNUM_PRCID] = {
	PRC_INT_1_BASE,
#if TNUM_PRCID >= 2
	PRC_INT_2_BASE,
#endif /* TNUM_PRCID >= 2 */
#if TNUM_PRCID >= 3
	PRC_INT_3_BASE,
#endif /* TNUM_PRCID >= 3 */
#if TNUM_PRCID >= 4
	PRC_INT_4_BASE,
#endif /* TNUM_PRCID >= 4 */
};

/*
 *  タスクロックのテーブル
 */
const uint32_t target_tsk_lock_table[TNUM_PRCID] = {
	TSK_MUTEX_1_BASE,
#if TNUM_PRCID >= 2
	TSK_MUTEX_2_BASE,
#endif /* TNUM_PRCID >= 2 */
#if TNUM_PRCID >= 3
	TSK_MUTEX_3_BASE,
#endif /* TNUM_PRCID >= 3 */
#if TNUM_PRCID >= 4
	TSK_MUTEX_4_BASE,
#endif /* TNUM_PRCID >= 4 */
};

/*
 *  オブジェクトロックのテーブル
 */
const uint32_t target_obj_lock_table[TNUM_PRCID] = {
	OBJ_MUTEX_1_BASE,
#if TNUM_PRCID >= 2
	OBJ_MUTEX_2_BASE,
#endif /* TNUM_PRCID >= 2 */
#if TNUM_PRCID >= 3
	OBJ_MUTEX_3_BASE,
#endif /* TNUM_PRCID >= 3 */
#if TNUM_PRCID >= 4
	OBJ_MUTEX_4_BASE,
#endif /* TNUM_PRCID >= 4 */
};

/*
 *  str_ker() の前でマスタプロセッサで行う初期化
 */
void
target_mprc_initialize(void)
{

}

/*
 *  ターゲット依存の初期化
 */
void
target_initialize(void)
{   
	/*
	 *  Nios2依存の初期化
	 */
	prc_initialize();
}

/*
 *  ターゲット依存の終了処理
 */
void
target_exit(void)
{
	/*
	 *  Nios2依存の終了処理
	 */
	prc_terminate();

	while(1);
}

/*
 *  ターゲット依存の文字出力
 */
void
target_fput_log(char_t c)
{
	uint32_t base = target_fput_log_base_table[x_prc_index()];

	if (c == '\n') {
		jtag_uart_putc(base, '\r');
	}
	jtag_uart_putc(base, c);
}
