/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *
 *  Copyright (C) 2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: perf_act_tsk.c 170 2009-02-25 08:27:36Z ertl-kikuchi $
 */

/*
 *  sig_sem 性能測定プログラム
 */

#include <kernel.h>
#include <t_syslog.h>
#include <sil.h>
#include <histogram.h>
#include "kernel_cfg.h"
#include "perf_sig_sem.h"
#include "target_test.h"

/*
 *  計測回数と実行時間分布を記録する最大時間
 */
#define NO_MEASURE	20000U			/* 計測回数 */
#define MAX_TIME	400000U			/* 実行時間分布を記録する最大時間 */

/*
 *  実行時間分布を記録するメモリ領域
 */
static uint_t	histarea1[MAX_TIME + 1];

/*
 *  計測の前後でのフックルーチン
 */
#ifndef CPU1_PERF_PRE_HOOK
#define CPU1_PERF_PRE_HOOK
#endif  /* CPU1_PERF_PRE_HOOK */
#ifndef CPU1_PERF_POST_HOOK
#define CPU1_PERF_POST_HOOK
#endif  /* CPU1_PERF_POST_HOOK */
#ifndef CPU2_PERF_PRE_HOOK
#define CPU2_PERF_PRE_HOOK
#endif  /* CPU2_PERF_PRE_HOOK */
#ifndef CPU2_PERF_POST_HOOK
#define CPU2_PERF_POST_HOOK
#endif  /* CPU2_PERF_POST_HOOK */

void task1_1(intptr_t exinf)
{
	wai_sem(SEM1);
}

/*
 * 計測ルーチン
 */
void perf_eval(uint_t n)
{
	uint_t	i;

	init_hist(1, MAX_TIME, histarea1);

	sus_tsk(LOGTASK1);
	dly_tsk(1000);
	CPU1_PERF_PRE_HOOK;

	for ( i = 0; i < NO_MEASURE; i++ ) {
		switch (n) {
			//オーバーヘッドの測定
		case 0:
			begin_measure(1);
			end_measure(1);
			break;
			//【１】セマフォに対する待ちタスクが存在せず，セマフォ資源数に1加える．
		case 1:
			begin_measure(1);
			sig_sem(SEM1);
			end_measure(1);
			wai_sem(SEM1);
			break;
			//【２】セマフォに対する待ちタスクが存在する．sig_semを実行
			//      するタスク（実行タスク）と同じプロセッサに割り付けら
			//      れており，優先度は実行タスクより低い
		case 2:
			act_tsk(TASK1_1);
			chg_pri(0, HIGH_PRIORITY);
			begin_measure(1);
			sig_sem(SEM1);
			end_measure(1);
			chg_pri(0, MID_PRIORITY);
			break;
		}
	}

	CPU1_PERF_POST_HOOK;
	rsm_tsk(LOGTASK1);

	syslog(LOG_NOTICE, "==================================");
	syslog(LOG_NOTICE, "(%d)", n);
	syslog(LOG_NOTICE, "----------------------------------");
	print_hist(1);
}

/*
 *  PE1 メインタスク：中優先度
 */
void main_task1(intptr_t exinf)
{
	perf_eval(0);
	perf_eval(1);
	perf_eval(2);
}
