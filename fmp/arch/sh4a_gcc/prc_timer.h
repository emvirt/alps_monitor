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
 *  @(#) $Id: prc_timer.h 512 2010-02-01 05:33:27Z ertl-honda $
 */

/*
 *  タイマドライバ（SH4A用）
 */
#ifndef TOPPERS_PRC_TIMER_H
#define TOPPERS_PRC_TIMER_H

#include <sil.h>

/*
 *  タイマ割込みハンドラ登録のための定数
 */
#define INHNO_TIMER0     TMU0_INTEVT   /* 割込みハンドラ番号 */
#define INTNO_TIMER0     TMU0_INTEVT   /* 割込み番号 */
#define INHNO_TIMER1     TMU1_INTEVT   /* 割込みハンドラ番号 */
#define INTNO_TIMER1     TMU1_INTEVT   /* 割込み番号 */
#define INHNO_TIMER2     TMU2_INTEVT   /* 割込みハンドラ番号 */
#define INTNO_TIMER2     TMU2_INTEVT   /* 割込み番号 */
#define INHNO_TIMER3     TMU3_INTEVT   /* 割込みハンドラ番号 */
#define INTNO_TIMER3     TMU3_INTEVT   /* 割込み番号 */

#define INTPRI_TIMER    (-6)          /* 割込み優先度 */
#define INTATR_TIMER    0U            /* 割込み属性 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  タイマ値の内部表現の型
 */
typedef uint32_t    CLOCK;

/*
 *  タイマ値の内部表現とミリ秒・μ秒単位との変換
 *
 */
#define TO_CLOCK(nume, deno)    ((CLOCK)(TIMER_CLOCK * (nume) / (deno)))

/*
 *  ナノ単位に変換　10×-9
 */
#define TO_USEC(clock)          (((SYSUTM) clock) * 80U) 

/*
 *  設定できる最大のタイマ周期（単位は内部表現）
 */
#define MAX_CLOCK    ((CLOCK) 0xffffU)

/*
 *  タイマの起動処理
 *
 *  タイマを初期化し，周期的なタイマ割込み要求を発生させる．
 */
extern void target_timer_initialize(intptr_t exinf);

/*
 *  タイマの停止処理
 *
 *  タイマの動作を停止させる．
 */
extern void target_timer_terminate(intptr_t exinf);

/*
 *  タイマの現在値の読出し
 */
Inline CLOCK
target_timer_get_current(void)
{
	return(0xffffffff - sil_rew_mem((void*)TMU_TCNT4));
}

/*
 *  性能評価用ナノ秒時間取得
 */
Inline void
get_ntim(SYSUTM *p_sysutm)
{
	uint32_t	clock1;

	clock1 = target_timer_get_current();

	/*
	 *  1カウント80n秒
	 */
	*p_sysutm = clock1 * 80;
}

/*
 *  タイマ割込み要求のチェック(性能評価時)
 */
Inline bool_t
target_timer_probe_int(void)
{
	return(sil_reh_mem((void*)TMU_TCR4) & TCR_UNF);
}

/*
 *  タイマ割込みハンドラ
 */
extern void target_timer_handler(intptr_t exinf);
extern void perf_timer_initialize(intptr_t exinf);
extern void synco_asm(void);

#ifdef TOPPERS_TRACE_ENABLE
extern SYSTIM target_time;
#endif /* TOPPERS_TRACE_ENABLE */

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_PRC_TIMER_H */
