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
 *  @(#) $Id: arm.h 2 2008-05-09 18:12:27Z ertl-honda $
 */

#include "kernel_impl.h"
#include "time_event.h"
#include <sil.h>
#include "chip_test.h"

/*
 *  タイマコントロールレジスタ
 */
static void * const tm_cnt[] = {
	(void *)(MPCORE_PMR_BASE + 0x0708),
	(void *)(MPCORE_PMR_BASE + 0x0808),
	(void *)(MPCORE_PMR_BASE + 0x0908),
	(void *)(MPCORE_PMR_BASE + 0x0a08),
};

/*
 *  タイマ割込みステータスレジスタ
 */
static void * const tm_isr[] = {
	(void *)(MPCORE_PMR_BASE + 0x070C),
	(void *)(MPCORE_PMR_BASE + 0x080C),
	(void *)(MPCORE_PMR_BASE + 0x090C),
	(void *)(MPCORE_PMR_BASE + 0x0a0C),
};

/*
 *  タイマ割込みロードレジスタ
 */
static void * const tm_lr[] = {
	(void *)(MPCORE_PMR_BASE + 0x0700),
	(void *)(MPCORE_PMR_BASE + 0x0800),
	(void *)(MPCORE_PMR_BASE + 0x0900),
	(void *)(MPCORE_PMR_BASE + 0x0a00),
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
	sil_wrw_mem(tm_cnt[prc_index], 00);
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
	sil_wrw_mem(tm_cnt[prc_index],
				(MPCORE_TM_PS_1MHZ << MPCORE_TM_CNT_PS_OFFSET)
				| MPCORE_TM_CNT_IEN | MPCORE_TM_CNT_AR | MPCORE_TM_CNT_EN);
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

	/* タイマーのスタート（オートリロードなし）*/
	sil_wrw_mem(tm_cnt[prc_index],
				(MPCORE_TM_PS_1MHZ << MPCORE_TM_CNT_PS_OFFSET)
				| MPCORE_TM_CNT_IEN | MPCORE_TM_CNT_EN);
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
	SIL_PRE_LOC;

	SIL_LOC_SPN();

	/* タイマーのスタート（オートリロードなし）*/
	sil_wrw_mem(tm_cnt[prc_index],
				(MPCORE_TM_PS_1MHZ << MPCORE_TM_CNT_PS_OFFSET)
				| MPCORE_TM_CNT_IEN | MPCORE_TM_CNT_EN);
	SIL_UNL_SPN();

	/* 割込み発生まで待つ */
	while((sil_rew_mem(tm_isr[prc_index]) & MPCORE_TM_ISR_SCBIT) != MPCORE_TM_ISR_SCBIT);
}
