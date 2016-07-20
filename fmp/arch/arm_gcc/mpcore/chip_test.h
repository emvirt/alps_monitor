/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel  
 * 
 *  Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: chip_test.h 498 2010-01-20 09:55:11Z ertl-honda $
 */

/*
 *		テストプログラムのチップ依存定義（MPCore用）
 */

#ifndef TOPPERS_CHIP_TEST_H
#define TOPPERS_CHIP_TEST_H

/*
 *  性能評価プログラムのための定義
 */

/*
 *  タイマの割込み番号
 */
#define _INTNO_TIMER  29

/*
 *  性能評価前後の処理
 */
#ifndef CPU1_PERF_PRE_HOOK
#define CPU1_PERF_PRE_HOOK    dis_int(_INTNO_TIMER);
#endif  /* CPU1_PERF_PRE_HOOK */

#ifndef CPU1_PERF_POST_HOOK
#define CPU1_PERF_POST_HOOK   ena_int(_INTNO_TIMER);
#endif  /* CPU1_PERF_POST_HOOK */

#ifdef TOPPERS_SYSTIM_LOCAL

#ifndef CPU2_PERF_PRE_HOOK
#define CPU2_PERF_PRE_HOOK    dis_int(_INTNO_TIMER);
#endif  /* CPU2_PERF_PRE_HOOK */
#ifndef CPU2_PERF_POST_HOOK
#define CPU2_PERF_POST_HOOK   ena_int(_INTNO_TIMER);
#endif  /* CPU2_PERF_POST_HOOK */

#endif /* TOPPERS_SYSTIM_LOCAL */

/*
 *  テスト用の関数
 */

/*
 *  ティック更新の停止（全プロセッサ）
 */ 
extern void ttsp_target_stop_tick(void);

/*
 *  ティック更新の停止（特定プロセッサ）
 */ 
extern void ttsp_target_stop_tick_pe(ID prcid);

/*
 *  ティック更新の再開（全プロセッサ）
 */ 
extern void ttsp_target_start_tick(void);

/*
 *  ティック更新の再開（特定プロセッサ）
 */ 
extern void ttsp_target_start_tick_pe(ID prcid);

/*
 *  ティックの更新（全プロセッサ）
 */
extern void ttsp_target_gain_tick(void);

/*
 *  ティックの更新（特定プロセッサ）
 */
extern void ttsp_target_gain_tick_pe(ID prcid);

/*
 *  ティック用タイマ割込み要求の発生（自プロセッサ）
 */ 
extern void ttsp_target_raise_tick_int(void);

/*
 *  割込みの発生
 */
extern void ttsp_highpri_int_raise(void);
extern void ttsp_midpri_int_raise(void);
extern void ttsp_lowpri_int_raise(void);

/*
 *  割込みハンドラ
 */
extern void target_highpri_int_handle(void);
extern void target_midpri_int_handle(void);
extern void target_lowpri_int_handle(void);

#endif /* TOPPERS_CHIP_TEST_H */
