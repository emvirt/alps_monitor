/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2007-2010 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id$
 */

/*
 *  ターゲット依存モジュール（APSH2AD用）
 *
 *  カーネルのターゲット依存部のインクルードファイル．kernel_impl.hのター
 *  ゲット依存部の位置付けとなる．
 */

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H

/*
 *  ターゲットシステムのハードウェア資源の定義
 */
#include "apsh2ad.h"

/*
 *  トレースログに関する設定
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"

/*
 *  出力するログ種別の定義
 *  　必要に応じてasp/arch/logtrace/trace_dump.cのtrace_print()に
 *  　対応するログ種別を追記する。
 */
#define LOG_INH_ENTER			/*   17(0x11)  */
#define LOG_INH_LEAVE			/*  145(0x91)  */
#define LOG_EXC_ENTER			/*   22(0x16)  */
#define LOG_EXC_LEAVE			/*  150(0x96)  */

#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  ターゲット依存の文字出力に使用するポートの定義
 *  (システムログを複数のプロセッサで共有する場合）
 */
#define TARGET_PUTC_PORTID		1

/*
 *  FMPカーネル動作時のメモリマップと関連する定義
 */

/*
 *  デフォルトの非タスクコンテキスト用のスタック領域の定義
 */
#ifdef ROM_BOOT				/*  ROM化  */
#define DEFAULT_PRC1_ISTKSZ      (1024 * 8)   /* 8KB */
#define DEFAULT_PRC2_ISTKSZ      (1024 * 8)   /* 8KB */
#else	/*  ROM_BOOT  */	/*  デバッグ時  */
#define DEFAULT_PRC1_ISTKSZ      (1024 * 8)   /* 8KB */
#define DEFAULT_PRC2_ISTKSZ      (1024 * 8)   /* 8KB */
#endif /*  ROM_BOOT  */

/*	スタック領域の底 */
#define DEFAULT_PRC1_ISTK_BOTTOM		\
		(INNER_RAM0_PAGE3_ADDRESS_START + INNER_RAM_PAGE_SIZE)
#define DEFAULT_PRC2_ISTK_BOTTOM		\
		(INNER_RAM1_PAGE1_ADDRESS_START + INNER_RAM_PAGE_SIZE)

/*  スタック領域の先頭番地（スタック領域の底ではないので、注意） */
#define DEFAULT_PRC1_ISTK      			\
		(void *)(DEFAULT_PRC1_ISTK_BOTTOM - DEFAULT_PRC1_ISTKSZ)
#define DEFAULT_PRC2_ISTK      			\
		(void *)(DEFAULT_PRC2_ISTK_BOTTOM - DEFAULT_PRC2_ISTKSZ)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  str_ker() の前でマスタプロセッサで行う初期化
 */
extern void target_mprc_initialize(void);

/*
 *  ターゲットシステム依存の初期化
 */
extern void target_initialize(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．
 */
extern void target_exit(void) NoReturn;

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  微少時間待ちのための定義（本来はSILのターゲット依存部）
 *  　　要検討：値は調整していない
 */
#ifdef ROM_BOOT		/*  ROM化の場合  */
#define SIL_DLY_TIM1     75
#define SIL_DLY_TIM2     21

#else				/*  RAM上でデバッグする場合  */
#define SIL_DLY_TIM1     1670
#define SIL_DLY_TIM2      906

#endif

/*
 *  プロセッサ依存モジュール（SH7205用）
 */
#include "sh2a_dual_gcc/sh7205_config.h"

#endif  /* TOPPERS_TARGET_CONFIG_H */
