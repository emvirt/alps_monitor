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
 *  @(#) $Id: prc_config.c 461 2009-11-09 10:22:44Z ertl-honda $
 */

/*
 *  プロセッサ依存モジュール（Nios2用）
 */
#include "kernel_impl.h"
#include "check.h"
#include "task.h"
#include "nios2.h"
#include "mp.h"
#include "prc_ipi.h"

/*
 *  start.S での同期用
 */
volatile uint_t start_sync;

#ifndef NIOS2_USE_INT_VEC_INST
/*
 *  割込み要因の判定のためのビットマップサーチ命令
 */
const uint8_t int_bitmap_search_table[] = { 0, 1, 0, 2, 0, 1, 0,
										3, 0, 1, 0, 2, 0, 1, 0 };
#endif

/*
 *  プロセッサ依存の初期化
 */
void
prc_initialize()
{
	TPCB *p_tpcb = get_my_p_tpcb();
	uint32_t index = x_prc_index();

	/*
	 *  カーネル起動時は非タスクコンテキストとして動作させるため1に
	 */ 
	p_tpcb->except_nest_count = 1;

	/*
	 * 各割込みの割込み要求禁止フラグ全禁止
	 */
	p_tpcb->idf = ~0U;

	/*
	 * 割込み優先度マスクの初期値
	 */ 
	p_tpcb->iipm = 0U;

	/*
	 * 全ての割込みをマスク
	 */
	set_ienable(~0U);

	/*
	 *  割込み属性が設定されているかを判別するための変数を初期化する．
	 */
	p_tpcb->bitpat_cfgint = 0U;

	/*
	 *  非タスクコンテキスト用のスタックの初期値
	 */
	p_tpcb->istkpt = istkpt_table[index];

	/*
	 *  割込みマスクテーブルへのポインタの初期化
	 */
	p_tpcb->p_iipm_mask_table = p_iipm_mask_table[index];

	/*
	 *  割込みハンドラテーブルへのポインタの初期化
	 */
	p_tpcb->p_inh_table = p_inh_table[index];

	/*
	 *  割込み優先度テーブルへのポインタの初期化
	 */
	p_tpcb->p_inh_iipm_table = p_inh_iipm_table[index];

	/*
	 *  CPU例外ハンドラテーブルへのポインタの初期化
	 */
	p_tpcb->p_exch_table = p_exch_table[index];

	p_tpcb->prc_int_base = target_prc_int_base_table[index];
}

/*
 *  プロセッサ依存の終了処理
 */
void
prc_terminate(void)
{
	extern void    software_term_hook(void);
	void (*volatile fp)(void) = software_term_hook;

	/*
	 *  software_term_hookへのポインタを，一旦volatile指定のあるfpに代
	 *  入してから使うのは，0との比較が最適化で削除されないようにするた
	 *  めである．
	 */
	if (fp != 0) {
		(*fp)();
	}

	start_sync = 0;

	/*
	 *  すべての割込みをマスクする．
	 */
	set_ienable(~0U);
}

/*
 *  割込み要求ラインの属性の設定
 *
 *  ASPカーネルでの利用を想定して，パラメータエラーはアサーションでチェッ
 *  クしている．FI4カーネルに利用する場合には，エラーを返すようにすべき
 *  であろう．
 */
void
x_config_int(INTNO intno, ATR intatr, PRI intpri, uint_t affinity_mask)
{
	TPCB *p_tpcb;

	p_tpcb = get_my_p_tpcb();

	assert(VALID_INTNO(intno));
	assert(TMIN_INTPRI <= intpri && intpri <= TMAX_INTPRI);

	/*
	 *  割込み属性が設定されているかを判別するための変数の設定
	 */
	p_tpcb->bitpat_cfgint |= INTNO_BITPAT(INTNO_MASK(intno));

	/* 
	 * いったん割込みを禁止する
	 */    
	x_disable_int(intno);

	if ((intatr & TA_ENAINT) != 0U){
		(void)x_enable_int(INTNO_MASK(intno));
	}
}

/*
 *  CPU例外の発生状況のログ出力
 *
 *  CPU例外ハンドラの中から，CPU例外情報ポインタ（p_excinf）を引数とし
 *  て呼び出すことで，CPU例外の発生状況をシステムログに出力する．
 */
#ifdef SUPPORT_XLOG_SYS

void
xlog_sys(void *p_excinf)
{    
}

#endif /* SUPPORT_XLOG_SYS */

#ifndef OMIT_DEFAULT_EXC_HANDLER

/*
 * 未定義の例外が入った場合の処理
 */
void
default_exc_handler(void *p_excinf){
	ID prc_id = ID_PRC(x_prc_index());

	syslog(LOG_EMERG, "Processor %d : Unregistered Exception occurs.", prc_id);
	syslog(LOG_EMERG, " Excno = %d.",
		   *(((uint32_t*)p_excinf) + CPUEXC_FLM_EXCNO));
	syslog(LOG_EMERG, " ESTATUS = 0x%x.",
		   *(((uint32_t*)p_excinf) + CPUEXC_FLM_ESTATUS));
	syslog(LOG_EMERG, " Addr = 0x%x.",
		   *(((uint32_t*)p_excinf) + CPUEXC_FLM_PC));
	syslog(LOG_EMERG, " IPM = 0x%x.",
			 *(((uint32_t*)p_excinf) + CPUEXC_FLM_IPM));
	syslog(LOG_EMERG, " Interrupt/Exception Nest Count = %d",
			 *(((uint32_t*)p_excinf) + CPUEXC_FLM_NEST));
	target_exit();
}

#endif /* OMIT_DEFAULT_EXC_HANDLER */

#ifndef OMIT_DEFAULT_INT_HANDLER

/*
 * 未定義の割込みが入った場合の処理
 */
void
default_int_handler(void *p_excinf){
	ID prc_id = ID_PRC(x_prc_index());

	syslog(LOG_EMERG, "Processor %d : Unregistered Interrupt occurs.", prc_id);
	syslog(LOG_EMERG, " Intno = %d.", (int)p_excinf);
	target_exit();
}

void
perf_timer_initialize(intptr_t exinf)
{
	/* リセット */
	sil_wrw_iop((void*)(PERF_COUNTER_BASE), 1);
	/* スタート */
	sil_wrw_iop((void*)(PERF_COUNTER_BASE + 0x04), 0);
}

void
perf_timer_get(uint32_t *p_time)
{
	*p_time = sil_rew_iop((void*)(PERF_COUNTER_BASE));
}

uint32_t
perf_timer_conv_tim(time)
{
	return (time * 1000) / PERF_COUNTER_CLOCK;
}

#endif /* OMIT_DEFAULT_INT_HANDLER */
