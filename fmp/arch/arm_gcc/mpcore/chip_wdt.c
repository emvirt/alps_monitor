/*
 *  TOPPERS/FMP Kernel  
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2006-2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: chip_wdt.c 64 2011-09-29 14:00:04Z ertl-tohta $
 */

/*
 *  タイマドライバ（ARM PrimeCell Watchdog Timer Module用）
 *  
 *  [Private Timerとの違い]
 *  Watchdog Timerには Timer mode と Watchdog modeの2種類のモードがある。
 *      Timer mode    : Watchdog Counter Register が0になり、かつ割込みを
 *                      許可している場合、割込みが発生する(割込み番号30)。
 *      Watchdog mode : Watchdog Counter Register が0になった場合、CPUに
 *                      リセット信号が入力される。
 *  
 *  本ドライバでは Watchdog Timerを通常のタイマとして使用するため、
 *  Timer mode に設定している。
 */

#include "kernel_impl.h"
#include "time_event.h"
#include <sil.h>
#include "target_timer.h"


/*
 *  タイマ割込み要求のクリア
 */
Inline void
target_timer_int_clear()
{
	/* ペンディングビットをクリア */
	sil_wrw_mem((void *)MPCORE_WDT_ISR, MPCORE_WDT_ISR_SCBIT);
}

/*
 *  タイマの起動処理
 */
void
target_timer_initialize(intptr_t exinf)
{
	uint32_t    cyc = TO_CLOCK(TIC_NUME, TIC_DENO);

	/* タイマ停止 */
	sil_wrw_mem((void *)MPCORE_WDT_CNT, 0x00);

    /* Watchdog Timer を通常タイマとして設定 */
    sil_wrw_mem((void *)MPCORE_WDT_DR, 0x12345678);
    sil_wrw_mem((void *)MPCORE_WDT_DR, 0x87654321);

	/* タイマ停止 */
	sil_wrw_mem((void *)MPCORE_WDT_CNT, 0x00);

	/* ペンディングビットクリア */
	target_timer_int_clear();

	/* ロード値を設定 */
	sil_wrw_mem((void *)MPCORE_WDT_LR, cyc - 1);

	/*
	 * タイマースタート(割込み許可)
	 * 1MHzでカウントするようにプリスケーラーを設定
	 */
	sil_wrw_mem((void *)MPCORE_WDT_CNT,
				(MPCORE_TM_PS_1MHZ << MPCORE_WDT_CNT_PS_OFFSET)
                //| MPCORE_WDT_CNT_WDTMODE
				| MPCORE_WDT_CNT_IEN
                | MPCORE_WDT_CNT_AR
                | MPCORE_WDT_CNT_EN
                );
}

/*
 *  タイマの停止処理
 */
void
target_timer_terminate(intptr_t exinf)
{
	/* 割込みクリア */
	target_timer_int_clear();

	/* タイマ停止 */
	sil_wrw_mem((void *)MPCORE_WDT_CNT, 0x00);
}

/*
 *  タイマ割込みハンドラ
 */
void
target_timer_handler(void)
{
	ID prcid;

	/* 割込みクリア */
	target_timer_int_clear();

	iget_pid(&prcid);
	i_begin_int((0x10000 << (prcid - 1))|DIC_IRQNO_WDT);
	signal_time();     /* タイムティックの供給 */
	i_end_int((0x10000 << (prcid - 1))|DIC_IRQNO_WDT);
}
