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
 *  @(#) $Id: start.S 476 2009-11-20 03:46:09Z ertl-honda $
 */

/*
 *  カーネル対応のスタートアップモジュール（Nios2用）
 */

#define TOPPERS_MACRO_ONLY
#define UINT_C(val)		(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)	(val)		/* ulong_t型の定数を作るマクロ */
#define CAST(type, val)	(val)		/* 型キャストを行うマクロ */
#include "kernel_impl.h"
#include "prc_asm.inc"
#include "nios2_system.h"

/*
 *  同期用のマジックナンバー
 */
#define MAGIC_START 0x87654321

/*
 *  リセットルーチン
 */
	.section .entry, "xa"
	.align 2
	.global __reset
__reset:
	/*
	 *  Iキャッシュの初期化
	 */
#if NIOS2_ICACHE_SIZE > 0
#if NIOS2_ICACHE_SIZE > 0x8000
	movhi r2, %hi(NIOS2_ICACHE_SIZE)
#else 
	movui r2, NIOS2_ICACHE_SIZE
#endif
0:
	initi r2
	addi r2, r2, -NIOS2_ICACHE_LINE_SIZE
	bgt r2, zero, 0b
1:
	.pushsection .debug_alt_sim_info
	.int 1, 1, 0b, 1b
	.popsection
#endif /* NIOS2_ICACHE_SIZE > 0 */

	/*
	 *  startへジャンプ
	 */
	movhi r3, %hiadj(start)
	addi r3, r3, %lo(start)
	jmp r3


	.section .text
	.align 2
	.global start
	.type start, @function
start:
	/*
	 *  Dキャッシュの初期化
	 */
#if NIOS2_DCACHE_SIZE > 0
#if NIOS2_DCACHE_SIZE > 0x8000
	movhi r2, %hi(NIOS2_DCACHE_SIZE)
#else
	movui r2, NIOS2_DCACHE_SIZE
#endif
0:
	initd 0(r2)
	addi r2, r2, -NIOS2_DCACHE_LINE_SIZE
	bgt r2, zero, 0b
1:
	.pushsection .debug_alt_sim_info
	.int 2, 1, 0b, 1b
	.popsection
#endif /* NIOS2_DCACHE_SIZE > 0 */

	/*
	 *  STATUSレジスタの初期化（割込み禁止）
	 */
	rdctl r3, status
	movi  r4, ~STATUS_PIE
	and   r3, r3, r4
	wrctl status, r3

	/* 
	 *  グローバルポインターのセット
	 */
	movhi gp, %hiadj(_gp)
	addi  gp, gp, %lo(_gp)

	/*
	 *  スタックの初期化
	 */
	my_istkpt  sp, r3

	/*
	 *  hardware_init_hook を呼び出す。(0でない場合)
	 *
	 *  ハードウェア依存に必要な初期化処理がある場合は，
	 *  hardware_init_hook という関数を用意すればよい．
	 *  具体的にはROM化の際、RAMを使用可能にするための
	 *  バスコントローラの初期化等を行う。
	 *  sys_support.S 内で _hardware_init_hook を定義してい
	 *  る．
	 */
start_0:
	movhi r2, %hiadj(hardware_init_hook)
	addi  r2, r2, %lo(hardware_init_hook)
	beq   zero, r2, start_1
	callr r2

start_1:
	/*
     *  マスタプロセッサ以外は初期化待ち
	 */
	my_core_index r3
	movi r2, (TOPPERS_MASTER_PRCID - 1)
	bne  r2, r3, slave_start

	/*
	 *  bssセクションをクリア
	 */
	movhi r3, %hiadj(__bss_start)
	addi  r3, r3, %lo(__bss_start)
	movhi r2, %hiadj(__bss_end)
	addi  r2, r2, %lo(__bss_end)
	beq   r3, r2, start_3
start_2:
	stw  zero, (r3)
	addi r3, r3, 4
	bltu r3, r2, start_2

	/*
	 *  dataセクションの初期化（ROM化対応）
	 *
	 *  __idata_start から __idata_end までを，__data_start 以降に
	 *  コピーする．
	 */
start_3:
#ifndef NIOS2_OMIT_DATA_INIT
	movhi r3, %hiadj(__idata_start)
	addi  r3, r3, %lo(__idata_start)
	movhi r4, %hiadj(__idata_end)
	addi  r4, r4, %lo(__idata_end)
	beq   r3, r4, start_5  /* サイズ0なら何もしない */
	movhi r6, %hiadj(__data_start)
	addi  r6, r6, %lo(__data_start)
start_4:
	ldw   r2, 0(r3)
	stw   r2, 0(r6)
	addi  r3, r3, 4
	addi  r6, r6, 4
	bltu  r3, r4, start_4
#endif /* NIOS2_OMIT_DATA_INIT */

	/*
	 *  software_init_hook を呼び出す（0 でない場合）．
	 *
	 *  ソフトウェア環境（特にライブラリ）に依存して必要な初期化処
	 *  理がある場合は，software_init_hook という関数を用意すれば
	 *  よい．
	 */
start_5:
	movhi r2, %hiadj(software_init_hook)
	addi  r2, r2, %lo(software_init_hook)
	beq   zero, r2, start_6
	callr r2

	/*
	 *  target_mprc_initialize の呼び出し
	 *  sta_ker 以前にマスタプロセッサのみで行う必要のある初期化を実行
	 */
start_6:
    call  target_mprc_initialize

	/*
	 *  カーネルを起動する
	 */
start_7:
	/*
	 *  全コアの到着待ち
	 */
	movhi r2, %hiadj(MAGIC_START)
	addi  r2, r2, %lo(MAGIC_START)
#if TNUM_PRCID >= 2 
pe2_wait:
	movhi r4, %hiadj(SYSVER_REG6)
	addi  r4, r4, %lo(SYSVER_REG6)
	ldw   r3, 0(r4)
	bne   r2, r3, pe2_wait
#endif /* TNUM_PRCID >= 2  */

#if TNUM_PRCID >= 3 
pe3_wait:
	movhi r4, %hiadj(SYSVER_REG7)
	addi  r4, r4, %lo(SYSVER_REG7)
	ldw   r3, 0(r4)
	bne   r2, r3, pe3_wait
#endif /* TNUM_PRCID >= 3  */

#if TNUM_PRCID >= 4 
pe4_wait:
	movhi r4, %hiadj(SYSVER_REG8)
	addi  r4, r4, %lo(SYSVER_REG8)
	ldw   r3, 0(r4)
	bne   r2, r3, pe4_wait
#endif /* TNUM_PRCID >= 4  */

	/*
	 * 全コアの動作を確認したらカーネルをスタート
	 */
	movhi r2, %hiadj(MAGIC_START)
	addi  r2, r2, %lo(MAGIC_START)
	movhi r4, %hiadj(SYSVER_REG5)
	addi  r4, r4, %lo(SYSVER_REG5)
	stw   r2, 0(r4)
	call  sta_ker

	/*
	 *  マスタプロセッサ以外の初期化待ちルーチン
	 */
slave_start:
	/*
	 *  バリア同期のために同期用レジスタをセット(r3にプロセッサIDが含まれている)
	 */
	movhi r2, %hiadj(MAGIC_START)
	addi  r2, r2, %lo(MAGIC_START)
	slli  r3, r3, 2
	movhi r4, %hiadj(SYSVER_REG5)
	addi  r4, r4, %lo(SYSVER_REG5)
	add   r4, r4, r3
	stw   r2, 0(r4)

slave_wait:
	movhi r2, %hiadj(MAGIC_START)
	addi  r2, r2, %lo(MAGIC_START)
	movhi r4, %hiadj(SYSVER_REG5)
	addi  r4, r4, %lo(SYSVER_REG5)
	ldw   r3, 0(r4)
	bne   r2, r3, slave_wait
	call  sta_ker
