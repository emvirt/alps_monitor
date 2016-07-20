/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2009-2010 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_test.c 547 2010-02-26 02:31:40Z ertl-honda $
 */

#include "kernel_impl.h"
#include "time_event.h"
#include <sil.h>
#include "target_test.h"

static const uint8_t timer_str[] = {
	TMU_STR0,
	TMU_STR1
};

static void * const timer_tcr[] = {
	(void *)(TIM0_BASE + TIM_TCR),
	(void *)(TIM1_BASE + TIM_TCR)
};

/*
 *  ティック更新の停止(全プロセッサ)
 */
void
ttsp_target_stop_tick(void)
{
	uint_t i;

	for(i = 1; i <= TNUM_PRCID; i++) {
		ttsp_target_stop_tick_pe(i);
	}
}

/*
 *  ティック更新の停止(特定プロセッサ)
 */
void
ttsp_target_stop_tick_pe(ID prcid)
{
	uint_t prc_index = prcid - 1;
	SIL_PRE_LOC;

	SIL_LOC_SPN();
	sil_wrb_mem((void*)TMU_TSTR0,
				(sil_reb_mem((void*)TMU_TSTR0) & ~timer_str[prc_index]));
	SIL_UNL_SPN();
}

/*
 *  ティック更新の再開（全プロセッサ）
 */
void
ttsp_target_start_tick(void)
{
	uint_t i;

	for(i = 1; i <= TNUM_PRCID; i++) {
		ttsp_target_start_tick_pe(i);
	}
}

/*
 *  ティック更新の再開（特定プロセッサ）
 */
void
ttsp_target_start_tick_pe(ID prcid)
{
	uint_t prc_index = prcid - 1;
	SIL_PRE_LOC;

	SIL_LOC_SPN();
	sil_wrb_mem((void*)TMU_TSTR0,
				(sil_reb_mem((void*)TMU_TSTR0) | timer_str[prc_index]));
	SIL_UNL_SPN();
}

/*
 *  ティックの更新（全プロセッサ）(ティック更新の停止状態で呼び出す)
 */
void
ttsp_target_gain_tick(void) {
	uint_t i;

	for(i = 1; i <= TNUM_PRCID; i++) {
		ttsp_target_gain_tick_pe(i);
	}
}

/*
 *  ティックの更新（特定プロセッサ）(ティック更新の停止状態で呼び出す)
 */
void
ttsp_target_gain_tick_pe(ID prcid) {
	uint_t prc_index = prcid - 1;
	volatile PCB *p_pcb = get_mp_p_pcb(prcid);
	EVTTIM	current_time1, current_time2;
	ID pid;
	SIL_PRE_LOC;

	SIL_LOC_SPN();
	current_time1 = p_pcb->p_tevtcb->current_time;
	/* 割込みの禁止(UNIEをクリア) */
	sil_wrh_mem(timer_tcr[prc_index],
				sil_reh_mem(timer_tcr[prc_index]) & ~TCR_UNIE);

	/* タイマーのスタート */
	sil_wrb_mem((void*)TMU_TSTR0,
				(sil_reb_mem((void*)TMU_TSTR0) | timer_str[prc_index]));

	/* オーバーフロー待ち */
	while((sil_reh_mem(timer_tcr[prc_index]) & TCR_UNF) != TCR_UNF);

	/* タイマーの停止 */
	sil_wrb_mem((void*)TMU_TSTR0,
				(sil_reb_mem((void*)TMU_TSTR0) & ~timer_str[prc_index]));

	/* 割込みの許可(UNIEをセット) */
	sil_wrh_mem(timer_tcr[prc_index],
				sil_reh_mem(timer_tcr[prc_index]) | TCR_UNIE);
	SIL_UNL_SPN();

	/* 自PEに対しては，時間が進むまで待つ */
	sil_get_pid(&pid);
	if (pid == prcid) {
		do{
			current_time2 = p_pcb->p_tevtcb->current_time;
		}while(current_time1 == current_time2);
	}
}

/*
 *  ティック用タイマ割込み要求の発生（自プロセッサ）(ティック更新の停止状態で呼び出す)
 */ 
void
ttsp_target_raise_tick_int(void)
{
	uint_t prc_index = x_prc_index();

	/* 割込みの禁止(UNIEをクリア) */
	sil_wrh_mem(timer_tcr[prc_index],
				sil_reh_mem(timer_tcr[prc_index]) & ~TCR_UNIE);

	/* タイマーのスタート */
	sil_wrb_mem((void*)TMU_TSTR0,
				(sil_reb_mem((void*)TMU_TSTR0) | timer_str[prc_index]));

	/* オーバーフロー待ち */
	while((sil_reh_mem(timer_tcr[prc_index]) & TCR_UNF) != TCR_UNF);

	/* タイマーの停止 */
	sil_wrb_mem((void*)TMU_TSTR0,
				(sil_reb_mem((void*)TMU_TSTR0) & ~timer_str[prc_index]));

	/* 割込みの許可(UNIEをセット) */
	sil_wrh_mem(timer_tcr[prc_index],
				sil_reh_mem(timer_tcr[prc_index]) | TCR_UNIE);
}
