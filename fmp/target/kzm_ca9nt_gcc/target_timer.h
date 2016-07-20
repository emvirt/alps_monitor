/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2008 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_timer.h 599 2010-05-07 08:00:38Z ertl-honda $
 */

/*
 *  タイマドライバ（KZM_CA9用）
 */

#ifndef TOPPERS_TARGET_TIMER_H
#define TOPPERS_TARGET_TIMER_H

/*
 *  チップ依存モジュール（MPCORE用）
 */
//#include "arm_gcc/mpcore/chip_timer.h"

#include <sil.h>
//#include "mpcore.h"
#include "kzm_ca9.h"

/*
 *  タイマー関連
 */
//#define TIMER_BASE_REG         0x100E4000
//#define OVRTIMER_BASE_REG    ?

#define TIMER_SP804_1LOAD		0x000
#define TIMER_SP804_1VALUE		0x004
#define TIMER_SP804_1CONTROL	        0x008
#define TIMER_SP804_1CLEAR		0x00C
#define TIMER_SP804_1RIS		0x010
#define TIMER_SP804_2LOAD		0x020
#define TIMER_SP804_2VALUE		0x024
#define TIMER_SP804_2CONTROL	        0x028
#define TIMER_SP804_2CLEAR		0x02C

#define TIMER_SP804_CTRL_TIMER_EN     0x00000080
#define TIMER_SP804_CTRL_PERIODIC     0x00000040
#define TIMER_SP804_CTRL_INT_EN       0x00000020
#define TIMER_SP804_CTRL_PRESCALE_1   0x00000000
#define TIMER_SP804_CTRL_PRESCALE_16  0x00000004
#define TIMER_SP804_CTRL_PRESCALE_256 0x00000008
#define TIMER_SP804_CTRL_TIMESIZE_16  0x00000000
#define TIMER_SP804_CTRL_TIMESIZE_32  0x00000002
#define TIMER_SP804_CTRL_ONE_SHOT     0x00000001

#define INHNO_TIMER     DIC_IRQNO_TM_SP804_1  /* 割込みハンドラ番号 */
#define INTNO_TIMER     DIC_IRQNO_TM_SP804_1  /* 割込み番号 */

#define INTPRI_TIMER    -6            /* 割込み優先度 */
#define INTATR_TIMER    0U            /* 割込み属性 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  タイマ値の内部表現の型
 */
typedef uint32_t    CLOCK;

/*
 *  タイマ値の内部表現とミリ秒・μ秒単位との変換
 *  1μ秒毎(1MHz) にカウントダウンする
 */
#define TIMER_CLOCK          100000U
#define TO_CLOCK(nume, deno) (TIMER_CLOCK * (nume) / (deno))
#define TO_USEC(clock)       (((SYSUTM) clock) * 1000U / TIMER_CLOCK)
#define TO_NSEC(clock)		 (((SYSUTM) clock) * 100000U / TIMER_CLOCK) / 4

/*
 *  設定できる最大のタイマ周期（単位は内部表現）
 */
#define MAX_CLOCK        ((CLOCK) 0xffffffffU)

/*
 *  タイマの現在値を割込み発生前の値とみなすかの判断
 */
#define GET_TOLERANCE    100U    /* 処理遅れの見積り値（単位は内部表現）*/
#define BEFORE_IREQ(clock) \
            ((clock) >= TO_CLOCK(TIC_NUME, TIC_DENO) - GET_TOLERANCE)

/*
 *  タイマの起動処理
 *
 *  タイマを初期化し，周期的なタイマ割込み要求を発生させる．
 */
extern void    target_timer_initialize(intptr_t exinf);

/*
 *  タイマの停止処理
 *
 *  タイマの動作を停止させる．
 */
extern void    target_timer_terminate(intptr_t exinf);

/*
 *  タイマの現在値の読出し
 */
Inline CLOCK
target_timer_get_current(void)
{
  return(TO_CLOCK(TIC_NUME, TIC_DENO) - sil_rew_mem((void *)(TIMER_BASE_REG + TIMER_SP804_2VALUE)));
}

/*
 *  タイマ割込み要求のチェック
 */
Inline bool_t
target_timer_probe_int(void)
{
    return(x_probe_int(INTNO_TIMER));

}

/*
 *  タイマ割込みハンドラ
 */
extern void    target_timer_handler(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_TARGET_TIMER_H */
