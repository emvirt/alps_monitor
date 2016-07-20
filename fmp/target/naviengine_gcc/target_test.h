/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2007-2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_test.h 466 2009-11-11 08:43:27Z ertl-honda $
 */

/*
 *		テストプログラムのターゲット依存定義（NaviEngine用）
 */

#ifndef TOPPERS_TARGET_TEST_H
#define TOPPERS_TARGET_TEST_H

/*
 *  サンプルプログラムのための定義
 */
#define CPUEXC1		(0x10000|EXCH_NO_SWI)		/* ソフトウェア例外 */
#define CPUEXC2		(0x20000|EXCH_NO_SWI)		/* ソフトウェア例外 */
#define CPUEXC3		(0x30000|EXCH_NO_SWI)		/* ソフトウェア例外 */
#define CPUEXC4		(0x40000|EXCH_NO_SWI)		/* ソフトウェア例外 */
#define RAISE_CPU_EXCEPTION	Asm("svc #0")

/*
 *  性能評価プログラムのための定義
 */

/*
 *  カーネル内で宣言している関数の外部宣言
 */
extern void perf_timer_get(uint32_t *p_time);
extern uint32_t perf_timer_conv_tim(uint32_t time);

/*
 *  性能評価用のマクロ定義
 */
#define HISTTIM  uint32_t
#define HIST_GET_TIM(p_time)  perf_timer_get(p_time)
#define HIST_CONV_TIM(time)   perf_timer_conv_tim(time)

/*
 *  チップ依存モジュール（MPCORE用）
 */
#include "arm_gcc/mpcore/chip_test.h"

#endif /* TOPPERS_TARGET_TEST_H */
