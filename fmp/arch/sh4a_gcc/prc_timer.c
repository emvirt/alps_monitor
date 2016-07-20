/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: prc_timer.c 512 2010-02-01 05:33:27Z ertl-honda $
 */

/*
 *  タイマドライバ（SH4A用）
 */
#include "kernel_impl.h"
#include "time_event.h"
#include <sil.h>
#include "target_timer.h"

#ifdef TOPPERS_TRACE_ENABLE
SYSTIM target_time;
#endif /* TOPPERS_TRACE_ENABLE */

/*
 *  タイマポート初期化ブロックの定義
 */
typedef struct tim_initialization_block {
	void  *tcor_w;   /* タイマコンスタントレジスタ */
	void  *tcnt_w;   /* タイマカウンタ */
	void  *tcr_h;    /* タイマコントロールレジスタ */
	void  *tstr_b;
	uint8_t str;
} TIMINIB;

/*
 *  タイマ初期化ブロック
 */
const TIMINIB timinib_table[6] = {
	{
		(void *)(TIM0_BASE + TIM_TCOR),
		(void *)(TIM0_BASE + TIM_TCNT),
		(void *)(TIM0_BASE + TIM_TCR),
		(void *)(TMU_TSTR0),
		TMU_STR0
	},
	{
		(void *)(TIM1_BASE + TIM_TCOR),
		(void *)(TIM1_BASE + TIM_TCNT),
		(void *)(TIM1_BASE + TIM_TCR),
		(void *)(TMU_TSTR0),
		TMU_STR1
	},
	{
		(void *)(TIM2_BASE + TIM_TCOR),
		(void *)(TIM2_BASE + TIM_TCNT),
		(void *)(TIM2_BASE + TIM_TCR),
		(void *)(TMU_TSTR0),
		TMU_STR2
	},
	{
		(void *)(TIM3_BASE + TIM_TCOR),
		(void *)(TIM3_BASE + TIM_TCNT),
		(void *)(TIM3_BASE + TIM_TCR),
		(void *)(TMU_TSTR1),
		TMU_STR0
	},
	{
		(void *)(TIM4_BASE + TIM_TCOR),
		(void *)(TIM4_BASE + TIM_TCNT),
		(void *)(TIM4_BASE + TIM_TCR),
		(void *)(TMU_TSTR1),
		TMU_STR1
	},
	{
		(void *)(TIM5_BASE + TIM_TCOR),
		(void *)(TIM5_BASE + TIM_TCNT),
		(void *)(TIM5_BASE + TIM_TCR),
		(void *)(TMU_TSTR1),
		TMU_STR2
	}
};

/*
 *  CPUIDから管理ブロックを取り出すためのマクロ
 */
#define get_timinib (&(timinib_table[x_prc_index()]))
#define get_timinib_atr(intatr) (&(timinib_table[intatr]))

int_t const tmu_intevt[6] = {
	TMU0_INTEVT,
	TMU1_INTEVT,
	TMU2_INTEVT,
	TMU3_INTEVT,
	TMU4_INTEVT,
	TMU5_INTEVT
};

/*
 *  タイマの起動処理
 */
void
target_timer_initialize(intptr_t exinf)
{
	CLOCK    cyc = TO_CLOCK(TIC_NUME, TIC_DENO);
	const TIMINIB *p_timinib;
	p_timinib =   &(timinib_table[(int)exinf - 1]);
	SIL_PRE_LOC;

	/*
	*  タイマ周期を設定し，タイマの動作を開始する．
	*/
	sil_wrb_mem(p_timinib->tstr_b,
                (sil_reb_mem(p_timinib->tstr_b) & ~p_timinib->str));  /* タイマ停止 */
	assert(cyc <= MAX_CLOCK);             /* タイマ上限値のチェック */

    /* 分周比設定、割り込み許可 */
	sil_wrh_mem(p_timinib->tcr_h,(0x020 | TCR0_TPSC));
	sil_wrw_mem(p_timinib->tcor_w, cyc); /* timer constantレジスタをセット */
	sil_wrw_mem(p_timinib->tcnt_w, cyc); /* カウンターをセット */    

	/* タイマスタート */
	/* タイマスタートレジスタは複数コアで共有しているので排他制御が必要 */

	SIL_LOC_SPN();
	sil_wrb_mem(p_timinib->tstr_b,
                (sil_reb_mem(p_timinib->tstr_b) | p_timinib->str));
	SIL_UNL_SPN();

	/*
	*  タイマ割込み要求をクリアする．
	*/
	
	/* 割り込み要求をクリア */
	sil_wrh_mem(p_timinib->tcr_h,
                (sil_reh_mem(p_timinib->tcr_h) & ~TCR_UNF));
 
#ifdef TOPPERS_TRACE_ENABLE
	target_time = 0;
#endif /* TOPPERS_TRACE_ENABLE */    
}

/*
 *  タイマの停止処理
 */
void
target_timer_terminate(intptr_t exinf)
{
	const TIMINIB *p_timinib;
	p_timinib =   &(timinib_table[(int)exinf - 1]);
  
	/* タイマを停止 */
	sil_wrb_mem(p_timinib->tstr_b,
                (sil_reb_mem(p_timinib->tstr_b) & ~p_timinib->str));
	/* 割り込み要求をクリア */
	sil_wrh_mem(p_timinib->tcr_h,0);
}

/*
 *  タイマ割込みハンドラ
 */
void
target_timer_handler(intptr_t exinf)
{
	const TIMINIB *p_timinib;

	p_timinib =   &(timinib_table[exinf - 1]);
  
	/* 割り込み要求をクリア */
	sil_wrh_mem(p_timinib->tcr_h,
                (sil_reh_mem(p_timinib->tcr_h) & ~TCR_UNF));

#ifdef TOPPERS_TRACE_ENABLE    
	target_time++;
#endif /* TOPPERS_TRACE_ENABLE */    

	i_begin_int(tmu_intevt[exinf - 1]);
	signal_time();                    /* タイムティックの供給 */
	i_end_int(tmu_intevt[exinf - 1]);
}


/*
 *  性能評価用タイマの起動処理
 */
void
perf_timer_initialize(intptr_t exinf)
{
	CLOCK    cyc = 0xffffffff;
	const TIMINIB *p_timinib;
	p_timinib =   &(timinib_table[4]);

	/*
	*  タイマ周期を設定し，タイマの動作を開始する．
	*/
	sil_wrb_mem(p_timinib->tstr_b,
                (sil_reb_mem(p_timinib->tstr_b) & ~p_timinib->str));  /* タイマ停止 */
	assert(cyc <= MAX_CLOCK);             /* タイマ上限値のチェック */

	/* 分周比設定、割り込み許可 */
 
	sil_wrh_mem(p_timinib->tcr_h,(0x020 | TCR0_TPSC)); //1/4分周
	sil_wrw_mem(p_timinib->tcor_w, cyc); /* timer constantレジスタをセット */
	sil_wrw_mem(p_timinib->tcnt_w, cyc); /* カウンターをセット */    

	/* タイマスタート */
	sil_wrb_mem(p_timinib->tstr_b,
                (sil_reb_mem(p_timinib->tstr_b) | p_timinib->str));

	/*
	*  タイマ割込み要求をクリアする．
	*/
	/* 割り込み要求をクリア */
	sil_wrh_mem(p_timinib->tcr_h,
                (sil_reh_mem(p_timinib->tcr_h) & ~TCR_UNF));
}

void
synco_asm(void)
{
	Asm("synco");
}
