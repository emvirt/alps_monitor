/*
 *	TOPPERS/FMP Kernel
 *		Toyohashi Open Platform for Embedded Real-Time Systems/
 *		Advanced Standard Profile Kernel
 *	
 *	Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *									Toyohashi Univ. of Technology, JAPAN
 *	Copyright (C) 2007		by Embedded and Real-Time Systems Laboratory
 *				Graduate School of Information Science, Nagoya Univ., JAPAN
 *	Copyright (C) 2008-2010 by Industrial Technology Institute,
 *								Miyagi Prefectural Government, JAPAN
 *	
 *	上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *	ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *	変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *	(1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *		権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *		スコード中に含まれていること．
 *	(2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *		用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *		者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *		の無保証規定を掲載すること．
 *	(3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *		用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *		と．
 *	  (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *		  作権表示，この利用条件および下記の無保証規定を掲載すること．
 *	  (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *		  報告すること．
 *	(4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *		害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *		また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *		由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *		免責すること．
 *	
 *	本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *	よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *	に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *	アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *	の責任を負わない．
 * 
 *  @(#) $Id$
 */

/*
 *	カーネル対応のスタートアップモジュール(SH2A-DUAL用)
 */

#define TOPPERS_MACRO_ONLY
#define UINT_C(val) 	(val)		 /* uint_t型の定数を作るマクロ */
#define ULONG_C(val)	(val)		 /* ulong_t型の定数を作るマクロ */
#define CAST(type, val) (val)		 /* 型キャストを行うマクロ */
#include "kernel_impl.h"
#include "prc_asm.inc"

/*
 *	TOPPERS_MASTER_PRCID==1を想定している。
 *	　（マスタプロセッサのID=1）
 */
#if TOPPERS_MASTER_PRCID != 1
#error TOPPERS_MASTER_PRCID != 1
#endif

/*
 *	初期化時のシーケンス
 *	
 *	以下のシーケンスでプロセッサ間の同期を取る。
 *	
 *	　　　　　　　　　CPU0　　　　　　　　CPU1
 *	ターゲット依存（target_support.S）
 *	　　　　　　　　セマフォ制御レジスタSEM0をロック
 *	
 *	　　　　　　　　クロック設定		クロック設定
 *	　　　　　　　　FRQCR1以外　　　　　FRQCR1のみ
 *	
 *	　　　　　　　　高速内蔵RAM0へのアクセス設定（SYSCRn）
 *	
 *	　　　　　　　　　　　　　　　　　　高速内蔵RAM1へのアクセス設定（SYSCRn）
 *	　　　　　　　　　　　　　　　　　　高速内蔵RAM1上の変数cpu1_flag←0
 *	　　　　　　　　　　　　　　　　　　高速内蔵RAM1上に命令コードをコピー
 *	　　　　　　　　　　　　　　　　　　コピーした命令コードに分岐
 *	　　　　　　　　SEM0を待つ		←	SEM0をリリース...(1)
 *	
 *	　　　　　　　　外部メモリへのアクセス設定
 *	　　　　　　　　（バスステートコンローラの初期化）
 *										　
 *	アーキテクチャ依存（start.S）
 *	　　　　　　　　bss/dataセクションの初期化
 *	　　　　　　　　software_init_hook()の呼び出し
 *	　　　　　　　　target_mprc_initialize()の呼び出し
 *	
 *	　　　　　　　　cpu1_flag←1	→	cpu1_flag=1となるまで待つ...(2)
 *	　　　　　　　　　　　カーネル起動
 *	　　　　　　　　　　　（sta_ker()の呼び出し）
 *	
 *	
 *  　他プロセッサと同期を取るには、
 *  　　・sleep命令＋プロセッサ間割込み
 *  　　・セマフォレジスタを使った排他制御
 *  　　・内部RAMへのtest & set命令
 *  　の３つが考えられる。
 *  　
 *  　処理内容や使用状況により、以下の制限事項がある。
 *  　
 *  　・全割込みロック状態
 *  　　　プロセッサ間割込みは使用できない。
 *  　
 *  　・クロック周波数の設定
 *  　　　FRQCR1レジスタはCPU1からのみ設定可能
 *  　
 *  　・高速内蔵RAMへのアクセス設定（SYSCRnの初期化）
 *  　　　他プロセッサは高速内蔵メモリへのアクセス不可
 *  　　　（高速内蔵メモリに配置された命令への命令フェッチも不可）
 *  　
 *  　・外部メモリへのアクセス設定
 *  　　（バスステートコントローラの初期化）
 *  　　　他プロセッサは外部メモリへのアクセス不可
 *  　　　（外部メモリに配置された命令への命令フェッチも不可）
 *  　
 *  　ここでは、
 *	　　・CPU0が待つ場合：セマフォ制御レジスタ
 *	　　・CPU1が待つ場合：高速内蔵RAM1上のフラグ変数
 *  　を用いている。
 *	　フラグを用いるのは、
 *	　　・CPU1をCPUバスだけで動作させるため
 *	　　・target_mprc_initialize()と干渉しないため
 *	　　　　（内部でセマフォ制御レジスタを初期化している。）
 */
	.text
	.globl _start
	.align 2
_start:
	/*
	 *	SRを初期化する。
	 *	（全割込みロックとする。）
	 */
	movi20 #IIPM_DISALL, r0
	ldc    r0, sr

	/*
	 *	非タスクコンテキスト用スタックの初期化
	 */
	my_prc_index r10 r1 labal_skip1 _cpuidr_val		/*  r10←prcindex  */
	mov    r10, r11
	shll2  r11								/*  r11:テーブルのインデックス  */
	mov.l  _istkpt_table_p, r1				/*  テーブルの先頭アドレス  */
	add    r11, r1
	mov.l  @r1, r15							/*  r15←my_istkpt  */

	/*
	 *	_hardware_init_hook を呼び出す。(0でない場合)
	 *
	 *	ターゲットハードウェアに依存して必要な初期化処理がある場合は，
	 *	hardware_init_hookという関数を用意する．
	 *
	 *	以下の状態で分岐する。
	 *	　r4:プロセッサINDEX
	 *	　r5:テーブルのインデックス=プロセッサINDEX*4
	 */
start_0:
	mov.l  _hardware_init_hook_start, r0
	tst    r0, r0
	bt	   start_1
	mov    r10, r4
	mov    r11, r5
	jsr/n  @r0

    /*
     *  マスタプロセッサでなければ、カーネルを起動する
     *  　_hardware_init_hookでCPU1はcpu1_flag=1となるまで待っている。
     *  　（同期ポイント(2)）
     */
start_1:
	mov    r10, r0							/*  r0←prcindex  */
	cmp/eq #(TOPPERS_MASTER_PRCID - 1), r0
	bf     start_8

    /*******************************
     *  マスタプロセッサの場合の処理
     ********************************/

	/*
	 * bssセクションをクリア
	 *
	 *	各セクションは4バイト境界に配置されているので、
	 *	_endが4バイト境界でないとしても、
	 *	隣のセクションを上書きすることはない。
	 *	（下記のdataセクションの初期化も同様）
	 */
	mov.l  _bss_start_p, r1
	mov.l  _end_p, r2
	cmp/eq r1, r2
	bt	   start_3
	mov    #0, r0
	/*
	 *	下記の遅延スロット（(a)行）は、ループを脱出する際にも
	 *	実行されるので、予め、ループの回数を1回減らしておく。
	 *	（注意：_endの番地が3以下だと演算結果が負になり、NG）
	 */
	add    #-4, r2
start_2:
	cmp/hi r1, r2
	bt/s   start_2
	 mov.l  r0, @r1+	/*  (a) ソースレジスタがr0限定 */

	/*
	 *	dataセクションを初期化する（ROM化対応）．
	 *
	 *	__idata_start から __idata_end までを，__data_start 以降に
	 *	コピーする．
	 */
start_3:
	mov.l  __idata_start_p, r1
	mov.l  __idata_end_p, r2
	cmp/eq r1, r2
	bt	   start_5
	mov.l  __data_start_p, r3
	/*
	 *	下記の遅延スロット（(b)行）は、ループを脱出する際にも
	 *	実行されるので、予め、ループの回数を1回減らしておく。
	 *	（注意：__idata_endの番地が3以下だと演算結果が負になり、NG）
	 */
	add    #-4, r2
start_4:
	mov.l  @r1+, r0
	cmp/hi r1, r2
	bt/s   start_4
	 mov.l  r0, @r3+	/*  (b)ソースレジスタがr0限定  */

	/*
	 *	_software_init_hook を呼び出す（0 でない場合）．
	 *
	 *	ソフトウェア環境（特にライブラリ）に依存して必要な初期化処
	 *	理がある場合は，software_init_hook という関数を用意すれば
	 *	よい．
	 */
start_5:
	mov.l  _software_init_hook_start, r0
	tst    r0, r0
	bt	   start_6
	jsr/n  @r0

    /*
     *  target_mprc_initialize の呼び出し
     *
     *  sta_ker 以前にマスタプロセッサのみで行う必要のある初期化を実行
     */
start_6:
	mov.l  _target_mprc_initialize_start, r0
	tst    r0, r0
	bt	   start_7
	jsr/n  @r0

start_7:
	/*
	 *	CPU1との同期ポイント(2)
	 *　　CPU1に通知する。
	 */
	mov.l  _cpu1_flag_shadow_addr, r1
	mov    #1, r2
	mov.l  r2, @r1			/*  cpu1_flag_shadow←1  */

start_8:
	/*
	 *	カーネルを起動する
	 */
	mov.l  _sta_ker_start, r0
	jsr/n  @r0

	.align 2
_cpuidr_val:
	.long CPUIDR_ID_BIT
_istkpt_table_p:
	.long _istkpt_table

_bss_start_p:
	.long __bss_start
_end_p:
	.long _end
__idata_start_p:
	.long __idata_start
__idata_end_p:
	.long __idata_end
__data_start_p:
	.long __data_start

_hardware_init_hook_start:
	.long _hardware_init_hook
_software_init_hook_start:
	.long _software_init_hook
_target_mprc_initialize_start:
	.long _target_mprc_initialize
_sta_ker_start:
	.long _sta_ker

_cpu1_flag_shadow_addr:
	.long INNER_RAM1_PAGE0_SHADOW_START
