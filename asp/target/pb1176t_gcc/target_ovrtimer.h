/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005,2006 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_ovrtimer.h 264 2009-11-17 08:23:10Z dsl $
 */

/*
 *		オーバランタイマドライバ（PB1176用）
 */

#ifndef TOPPERS_TARGET_OVRTIMER_H
#define TOPPERS_TARGET_OVRTIMER_H

#include <sil.h>
#include "pb1176t.h"
#include "target_config.h"

/*
 *  オーバランタイマ割込みハンドラ登録のための定数
 */
#define INHNO_OVRTIMER	IRQNO_OVRTIMER		/* 割込みハンドラ番号 */
#define INTNO_OVRTIMER	IRQNO_OVRTIMER		/* 割込み番号 */
#define INTPRI_OVRTIMER	-5					/* 割込み優先度 */
#define INTATR_OVRTIMER	0					/* 割込み属性 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  タイマ値の内部表現の型
 */
typedef ulong_t		OVRCLOCK;

/*
 *  タイマ値の内部表現とプロセッサ時間との変換
 *
 *  PB1176では，タイマは1μ秒毎にカウントアップする．
 */
#define TO_OVRCLOCK(ovrtim)		((ulong_t)(ovrtim))
#define TO_OVRTIM(clk)			((OVRTIM)(clk))

/*
 *  オーバランタイマの初期化処理
 *
 *  オーバランタイマの初期化処理を行う．タイマの動作開始は行わない．割
 *  込みハンドラの登録は，target_ovrtimer.cfgをコンフィギュレータに処理させ
 *  ることで行う．
 */
Inline void
target_ovrtimer_initialize(void)
{
	sil_wrw_mem((void *)(OVRTIMER_BASE_REG + TIMER_SP804_1CONTROL),
		OVRTIMER_SP804_CONTROL_BITS);

	sil_wrw_mem((void *)(OVRTIMER_BASE_REG + TIMER_SP804_1CLEAR), 0x01);
}

/*
 *  オーバランタイマの動作開始
 *
 *  オーバランタイマに，残り時間ovrtimを設定し，動作開始する．ovrtimの
 *  単位はμ秒とする．
 */
Inline void
target_ovrtimer_start(OVRTIM ovrtim)
{
	assert(ovrtim > 0u);
	assert(ovrtim <= TMAX_OVRTIM);

	sil_wrw_mem((void *)(OVRTIMER_BASE_REG + TIMER_SP804_1LOAD),
		TO_OVRCLOCK(ovrtim));

	sil_wrw_mem((void *)(OVRTIMER_BASE_REG + TIMER_SP804_1CONTROL),
		OVRTIMER_SP804_CONTROL_BITS | OVRTIMER_SP804_CONTROL_ENA);
}

/*
 *  オーバランタイマの残り時間の取得
 *
 *  オーバランタイマの残り時間を読み出す．
 */
Inline OVRTIM
target_ovrtimer_get_current(void)
{
	return(TO_OVRTIM(
		sil_rew_mem((void *)(OVRTIMER_BASE_REG + TIMER_SP804_1VALUE))));
}

/*
 *  オーバランタイマの停止
 *
 *  オーバランタイマを停止し，タイマの残り時間を読み出す．もし残り時間
 *  が0（またはそれ未満）になっていた場合には，1を返す．また，オーバラ
 *  ンタイマからの割込みをクリアする．
 */
Inline OVRTIM
target_ovrtimer_stop(void)
{
	OVRCLOCK cyc;

	sil_wrw_mem((void *)(OVRTIMER_BASE_REG + TIMER_SP804_1CONTROL),
		OVRTIMER_SP804_CONTROL_BITS);

	cyc = sil_rew_mem((void *)(OVRTIMER_BASE_REG + TIMER_SP804_1VALUE));

	return TO_OVRTIM(cyc);
}

/*
 *  オーバランタイマ割込みハンドラ
 */
extern void	target_overtimer_handler(void);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_TARGET_OVRTIMER_H */
