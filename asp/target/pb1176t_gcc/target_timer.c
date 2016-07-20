/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2006 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_timer.c 264 2007-07-12 08:23:10Z hiro $
 */

/*
 *  タイマドライバ（PB1176T用）
 */

#include "kernel_impl.h"
#include "time_event.h"
#include <sil.h>
#include "target_timer.h"

#if ASP_MEASURE_TIMER_LAT == 1
unsigned long int safeg_latency[MAX_COUNT];
unsigned int latency_counter = 0;
#endif /* MEASURE_ASP_IRQ_LATENCY */

/*
 *  タイマの起動処理
 */
void
target_timer_initialize(intptr_t exinf)
{
	CLOCK    cyc = TO_CLOCK(TIC_NUME, TIC_DENO);
	assert(cyc <= MAX_CLOCK);

#if ASP_MEASURE_TIMER_LAT == 1
	/*
	 * Load the performance monitor cycle counter with the value
	 * 210000 (as the board is 210MHz, this is 1ms period). Note that
	 * the counter goes up, so we substract the value from 0xFFFFFFFF.
	 * Then enable the counter and configure it to cause interrupts.
	 */
	Asm("mcr p15, 0, %0, c15, c12, 1\n"
	    "mcr p15, 0, %1, c15, c12, 0\n"
	    ::"r"(0xffffffff - cyc), "r"(0x41));
#else
	/*
	 *  タイマ周期を設定し，タイマの動作を開始する．
	 */
	sil_wrw_mem((void *)(TIMER_BASE_REG + TIMER_SP804_1LOAD), 922); /* ロード値セット */
	sil_wrw_mem((void *)(TIMER_BASE_REG + TIMER_SP804_1CONTROL), 0x000000E2); /* タイマースタート */
#endif
}

/*
 *  タイマの停止処理
 */
void
target_timer_terminate(intptr_t exinf)
{

}

/*
 *  タイマ割込みハンドラ
 */
void
target_timer_handler(void)
{
#if ASP_MEASURE_TIMER_LAT == 1
	CLOCK cyc = TO_CLOCK(TIC_NUME, TIC_DENO);
	assert(cyc <= MAX_CLOCK);
	static uint32_t result = 0;
	/*
	 * Read the performance counter in result. Disable interrupts.
	 * Clear Instruction and Data cache. Enable interrupts again.
	 * Save the result in the array safeg_latency which can be read
	 * from realview to obtain the data.
	 */
	Asm("mrc p15, 0, %0, c15, c12, 1\n"
	    "mrs r0, cpsr\n"
	    "cpsid ifa\n"
	    "mcr p15, 0, %1, c7, c5, 0\n"
	    "mcr p15, 0, %1, c7, c5, 0\n"
	    "mcr p15, 0, %1, c7, c5, 0\n"
	    "mcr p15, 0, %1, c7, c5, 0\n"
	    "mcr p15, 0, %1, c7, c14, 0\n"
	    "msr cpsr, r0\n"
	    :"=r"(result)
	    :"r"(0)
	    :"r0");

	safeg_latency[latency_counter++] = result;
	latency_counter %= MAX_COUNT;

	/* Stop counter, load 1ms, clean overflow bit, restart it*/
	Asm("mcr p15, 0, %2, c15, c12, 0\n"
	    "mcr p15, 0, %0, c15, c12, 1\n"
	    "mcr p15, 0, %1, c15, c12, 0\n"
	    ::"r"(0xffffffff - cyc), "r"(0xFF00441), "r"(0x0));
#else
	/* タイマクリア */
	sil_wrw_mem((void *)(TIMER_BASE_REG + TIMER_SP804_1CLEAR), 0x01);
#endif
	i_begin_int(INTNO_TIMER);
	signal_time();                    /* タイムティックの供給 */
	i_end_int(INTNO_TIMER);
}
