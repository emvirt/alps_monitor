/*
 *	TOPPERS/FMP Kernel
 *		Toyohashi Open Platform for Embedded Real-Time Systems/
 *		Advanced Standard Profile Kernel
 *	
 *	Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *								Toyohashi Univ. of Technology, JAPAN
 *	Copyright (C) 2007		by Embedded and Real-Time Systems Laboratory
 *				Graduate School of Information Science, Nagoya Univ., JAPAN
 *	Copyright (C) 2007-2010 by Industrial Technology Institute,
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
 *	プロセッサ依存モジュール アセンブリ言語部（SH2A-DUAL用）
 *		コーディングルール
 *	 　		インデントは遅延スロットを表す。
 *	 　		該当する命令：jmp、jsr、bra、bsr、rts、rte、bf/s、bt/s、bsrf、braf
 */

#define TOPPERS_MACRO_ONLY
#define UINT_C(val) 	(val)		 /* uint_t型の定数を作るマクロ */
#define ULONG_C(val)	(val)		 /* ulong_t型の定数を作るマクロ */
#define CAST(type, val) (val)		 /* 型キャストを行うマクロ */

/*
 *	アセンブラ用のマクロを有効にする
 */
#define TOPPERS_ASM_MACRO
#include "kernel_impl.h"
#include "offset.h"

/*
 *	注意
 *	  いずれも0x80以上になると符号拡張され、
 *	  期待した動作にならない。
 */
#if !(TCB_p_tinib < 0x80)
#error	Range error!  Check (TCB_p_tinib < 0x80)
#endif

#if !(TCB_texptn < 0x80)
#error	Range error!  Check (TCB_texptn < 0x80)
#endif

#if !(TCB_sp < 0x80)
#error	Range error!  Check (TCB_sp < 0x80)
#endif

#if !(TCB_pc < 0x80)
#error	Range error!  Check (TCB_pc < 0x80)
#endif

#if !(TCB_enatex < 0x80)
#error	Range error!  Check (TCB_enatex < 0x80)
#endif

#if !(TCB_enatex_bit < 8)
#error	Range error!  Check (TCB_enatex_bit < 8)
#endif


/*
 *	カーネル内での全割込み禁止時のIPMの値
 *
 *	kernel_cfg.c 内のアセンブラで使用
 */
	.globl _iipm_disall
_iipm_disall = IIPM_DISALL

/*
 *	タスクディスパッチャ
 *
 *	このルーチンは，
 *	 ・タスクコンテキスト(excnest_count = 0)
 *	 ・CPUロック状態
 *	 ・ディスパッチ許可状態
 *	 ・（モデル上の）割込み優先度マスク全解除状態
 *	で呼び出される．
 */

	.text
	.align 2
	.globl _dispatch
_dispatch:
	/*
	 *	スクラッチレジスタを除くすべてのレジスタを保存
	 *	　pr,r8〜r14をスタックに保存
	 *	　r0〜r7は呼び出し元で保存しているため保存する必要が無い
	 */
	movmu.l r8,@-r15
	stc gbr, r7						/*  r7←my_p_pcb  */
	mov.l @(PCB_p_runtsk,r7),r0		/*  r0←my_p_pcb->p_runtsk 	*/
	mov.l _dispatch_r_dis,r1		/*  r1←実行再開番地   		*/
	mov.l r15,@(TCB_sp,r0)  /* タスクスタックをTCBに保存 60以下ならOK */
	bra   _dispatcher
	 mov.l	r1,@(TCB_pc,r0)   /* 実行再開番地をTCBに保存  60以下ならOK	*/

_dispatch_r:
	/*
	 *	スクラッチレジスタの除くすべてのレジスタを復帰
	 *	　pr,r8〜r14をスタックから復帰
	 */
	movmu.l @r15+,r8
	/*
	 *	タスク例外処理ルーチンの起動
	 *	dispatch_rはdispatcherから呼び出されるため，
	 *	tcbのアドレスはr0に入っている．
	 */
	bld.b  #TCB_enatex_bit, @(TCB_enatex,r0)
	bt     _dispatch_r_check_texptn	/* enatexがtrueならtexptnのチェックへ */
	rts/n		/*  dispatch()の呼び出し元へリターン  */

	/* texptn が 0で無ければ、タスク例外処理ルーチンの呼び出し */
_dispatch_r_check_texptn:
	mov.l  @(TCB_texptn,r0),r1		/* texptnをロード */
	tst    r1,r1
	bf	   _dispatch_r_call_texrtn
	rts/n		/*  dispatch()の呼び出し元へリターン  */

	/*
	 * タスク例外処理ルーチンの呼び出し
	 * 　_call_texrtnからはdispatch()の呼び出し元へリターンするため、
	 * 　jmp命令で分岐する。
	 *　（prレジスタには、dispatch()の呼び出し元の番地が格納されており、
	 *　　ここでは書き換えない。）
	 */
_dispatch_r_call_texrtn:
	mov.l  _dispatch_call_texrtn_dis,r2
	jmp    @r2
	 nop

/*
 * ディスパッチャの動作開始（prc_support.S）
 */
	.globl _start_dispatch
_start_dispatch:
	/*
	 *	このルーチンは，カーネル起動時に，
	 *	　・すべての割込みを禁止した状態（全割込みロック状態と同等）
	 *	　・sp＝非タスクコンテキスト用スタックポインタ
	 *	の状態で呼び出されることを想定している．
	 *
	 *	prc_initializeで，
	 *	　・lock_flag＝true
	 *	　・saved_iipm＝IIPM_ENAALL
	 *	に初期化しているため，カーネル管理外の割込みを許可することで，
	 *	　・CPUロック状態
	 *	　・（モデル上の）割込み優先度マスク全解除状態
	 *	になる．
	 *	また，initialize_taskでdisdspをfalseに初期化しているため，ディ
	 *	スパッチ許可状態になっている．
	 *
	 *	よって、タスクコンテキストに切り替えるだけでよい。
	 */
	mov   #0,r0
	bra   _dispatcher_0
	 mov.l r0, @(PCB_excnest_count,gbr)	/* excnest_count←0  	*/

/*
 *	現在のコンテキストを捨ててディスパッチ
 */
	.globl _exit_and_dispatch
_exit_and_dispatch:
	/* ディスパッチャ本体（dispatcher）へ */

/*
 * ディスパッチャ本体
 */
_dispatcher:
#ifdef LOG_DSP_ENTER
	mov.l _log_dsp_enter_dis, r1	/* ログ出力関数 */
	stc gbr, r8						/* r8←my_p_pcb */
	jsr   @r1
	 mov.l @(PCB_p_runtsk,r8),r4
							/* 引数：r4←my_p_pcb->p_runtsk */
#endif /* LOG_DSP_ENTER */
_dispatcher_0:
   /*
	*  ここにやって来るのは、上記の_dispatcherの他に、
	*  下記の2箇所からも合流してくる。
	*	   ・_start_dispatch
	*	   ・_dispatcher_2（実行すべきタスクがなく割込み待ちの場合）
	*
	*  このルーチンは，
	*	   ・タスクコンテキスト
	*	   ・CPUロック状態
	*	   ・ディスパッチ許可状態
	*	   ・（モデル上の）割込み優先度マスク全解除状態
	*  で呼び出される．
	*
	*  すなわち，
	*	   ・excnest_count=0
	*	   ・lock_flag=true
	*	   ・disdsp=false
	*	   ・dspflg=true
	*	   ・saved_iipm=IIPM_ENAALL
	*　となっている．実行再開番地へもこの状態のまま分岐する．
	*/
	stc gbr, r7						/* r7←my_p_pcb  */
	mov.l @(PCB_p_schedtsk,r7),r8	/* r8 <- my_p_schedtsk 			*/
	tst    r8,r8					/* my_p_schedtsk があるか？		*/
	bt/s  _dispatcher_1				/* my_p_schedtskが無ければジャンプ 	*/
	 mov.l r8, @(PCB_p_runtsk,r7)	/* my_p_runtsk←my_p_schedtsk	*/
	mov.l @(TCB_sp,r8),r15 			/* TCBからタスクスタックを復帰	*/
#ifdef LOG_DSP_LEAVE
	mov.l _log_dsp_leave_dis, r1 	/* ログ出力関数 */
	jsr   @r1
	 mov   r8, r4					/* 引数	  */
#endif /* LOG_DSP_LEAVE */
	mov.l  @(TCB_pc,r8),r1		/* TCBから実行再開番地を復帰		*/
	jmp    @r1
	 mov   r8, r0
		/*
		 *  分岐先（実行再開番地）では、r0にmy_p_schedtskを
		 *  格納された状態を想定している。
		 */

	/* 
	 * 実行すべきタスクがない場合の割込み待ち
	 */
_dispatcher_1:
	/* 
	 * イミディエイト値を毎回読み込むのは効率が悪いので事前に用意する
	 * （要検討：レジスタ割り当ての見直し　ASPのchangeset/1658）
	 * 
	 * r0  : 0x00（割込みを許可するときにはこの値をSRに設定する）
	 * r1  : 0x01
	 * r2  : 割込み禁止のためのSRの値
	 * r8  : my_p_pcb
	 * r12 : 番地_sleep_end
	 */
	mov    #0,r0
	mov    #1,r1
	mov.l  _iipm_disall_dis,r2		/* 割込み禁止時のSRの値 */
	stc    gbr, r8					/* r8←my_p_pcb */
#ifndef OMIT_POWER_CONTROL
	mov.l _sleep_end_dis, r12
#endif /* OMIT_POWER_CONTROL */
	/*
	 *  割込みを許可したら、CPUロック解除状態になるよう準備する
	 *  　　my_p_tacb->lock_flag←false
	 */
	mov.l r0, @(PCB_lock_flag,gbr)
	/*
	 *	非タスクコンテキストに切り替え
	 *	
	 *	ここで非タスクコンテキストに切り換えるのは，ここで発生する割込
	 *	み処理にどのスタックを使うかという問題の解決と，割込みハンドラ
	 *	内でのタスクディスパッチの防止という二つの意味がある．
	 */
	mov.l r1, @(PCB_excnest_count,r8)	/* excnest_count←1  	*/
	mov.l @(PCB_istkpt,r8),r15			/* スタック切替え		*/
_dispatcher_2:
	/*
	 *　割込み待ち
	 *
	 *　ディスパッチャの出口で実行すべきタスクがない(schedtsk==
	 *　NULL）場合は、sleep命令によってプロセッサを省電力モードに切
	 *　り替えて割込み待ちをしている。
	 *　
	 *　割込み許可後にsleep命令を実行しているため、割込み許可命令の
	 *　実行前に割込み要求が入っている場合（あるいは割込み許可直後、
	 *　sleep命令実行前に割込み要求が入った場合）、割込み許可命令の
	 *　実行と共に割込みが受け付けられ、その復帰後にsleepしたままに
	 *　なり、reqflgのチェックに進まない。
	 *　（1msec以内にタイマ割込みが入るため、実際にはsleepしたままと
	 *　いうことはない。）
	 *　
	 *　この問題は、割込みの許可とsleep状態への移行がアトミックに実
	 *　行できないことに起因する。
	 *
	 *	この問題を解決するため，rte命令の「PC,SR更新と遅延スロットの
	 *	命令を、割込みとは不可分に実行する」という仕様を利用し，割込
	 *	みの許可と省電力モードへの移行を不可分に行う．
	 */
#ifdef OMIT_POWER_CONTROL
	ldc    r0,sr				/*  割込み許可 				  */
	 nop
#else
	mov.l r0, @-r15				/*	SRに設定する値をスタックに積む	*/
	mov.l r12, @-r15			/*	戻り番地をスタックに積む  */
	/*
	 *　割込み許可する（スタックから読み出した値をSRに設定する）
	 *　と同時に省電力モードへ移行する。
	 *　
	 *　要検討
	 *　　ソフトウェアスタンバイモードに移行する前に
	 *　　バスステートコントローラのライトバッファから
	 *　　外部RAMへの書き込みが完了したことを確認する。
	 *　　（外部RAMへのダミーリードを行う。）
	 */
	rte
#ifdef JTAG_DEBUG
	/*
	 *　JTAGエミュレータを使用する場合は、スリープしない。
	 */
	 nop						/*	（遅延スロット）  */
#else /* JTAG_DEBUG */
	 sleep						/*	（遅延スロット）  */
#endif /* JTAG_DEBUG */

_sleep_end:
#endif /* OMIT_POWER_CONTROL */
	ldc    r2,sr				/* カーネル管理の割込みを禁止	   */
	/*
	 *　LDC Rm,SR命令の仕様
	 *　　直後の命令実行時に割込み要求があった場合の振る舞い
	 *　
	 *　　・SH-2E以前
	 *　　　　直後の命令が割込み禁止で実行される。
	 *　　　　
	 *　　・SH-2A以降
	 *　　　　LDC Rm,SR命令で設定した割込みマスク（SRのIビット）を反映して
	 *　　　　直後の命令実行時に割込みを受け付けるか否かが決まる。
	 *　
	 *　よって、以下のシーケンスでは、どちらの場合も命令１は
	 *　割込み禁止で実行されるので、nop命令を挿入する必要はない。
	 *　
	 *　  mov  #f0, r0
	 *　  ldc  r0, sr
	 *　　命令１
	 */
	mov.l @(PCB_reqflg,r8),r6	/* r6 <- my_p_pbc->reqflg		   */
	tst   r6,r6					/* reqflg が falseなら			   */
	bt	  _dispatcher_2			/* 　dispatcher_2 へ			   */
	mov.l r0, @(PCB_reqflg,gbr)	/* reqflg←false				   */
	/*
	 *	タスクコンテキストに切り替え
	 *	　スタック切り替えは、分岐先の_dispatcher_0で行う。
	 */
	mov.l r0, @(PCB_excnest_count,gbr)	/* excnest_count をクリア	*/
	
	/*
	 *	CPUロック状態に戻す．割込み待ちの間に実行した割込みハンドラによ
	 *	り，saved_iipmが書き換えられる可能性があるため，元の値に戻す必
	 *	要がある．dispatcherが実行される時は，saved_iipmがIIPM_ENAALL
	 *	となっているため，ここではsaved_iipmをIIPM_ENAALL（＝0）に戻せ
	 *	ばよい．
	 */
	mov.l r1, @(PCB_lock_flag,r8)		/* lock_flag←true */
	bra    _dispatcher_0
	 mov.l r0, @(PCB_saved_iipm,gbr)	/* saved_iipm←0 */


    /*
     *  自タスクのマイグレーション
     *  　パラメータとしてr4にマイグレーション先のプロセッサIDが
     *  　格納されているので、migrate_selfを呼び出すまで、
     *  　破壊してはいけない。
     */
    .text
    .align 2
    .globl _dispatch_and_migrate
_dispatch_and_migrate:
	/*
	 *	スクラッチレジスタを除くすべてのレジスタを保存
	 *	　pr,r8〜r14をスタックに保存
	 *	　r0〜r7は呼び出し元で保存しているため保存する必要が無い
	 */
	movmu.l r8,@-r15
	mov.l @(PCB_p_runtsk,gbr),r0	/*  r0←my_p_pcb->p_runtsk 	*/
	mov.l _dispatch_r_dis,r1		/*  r1←実行再開番地   		*/
	mov.l r15,@(TCB_sp,r0)  /* タスクスタックをTCBに保存 60以下ならOK */
	mov.l r1,@(TCB_pc,r0)   /* 実行再開番地をTCBに保存  60以下ならOK	*/
    mov.l p_migrate_self, r2
	stc   gbr, r8			/* r8←my_p_pcb */
    jsr   @r2		/*  パラメータr4：マイグレーション先のプロセッサID  */
	 mov.l @(PCB_istkpt,r8),r15
							/* 非タスクコンテキスト用スタックへ切り替え */

    .align 2
_iipm_disall_dis:
	.long _iipm_disall

    /*
     *  現在のコンテキストを捨てマイグレーション
     */
    .text
    .align 2
    .globl _exit_and_migrate
_exit_and_migrate:
    mov.l p_exit_and_migrate_self, r2
	stc   gbr, r8	/* r8←my_p_pcb */
    jsr   @r2		/* 自タスクのexitとマイグレーションルーチンへ */
    				/*  パラメータr4：マイグレーション先のプロセッサID  */
	 mov.l @(PCB_istkpt,r8),r15
					/* 非タスクコンテキスト用スタックへ切り替え */

	.align 2
_dispatch_r_dis:
	.long _dispatch_r
_dispatch_call_texrtn_dis:
	.long _dispatch_call_texrtn
#ifndef OMIT_POWER_CONTROL
_sleep_end_dis:
	.long _sleep_end
#endif /* OMIT_POWER_CONTROL */
#ifdef LOG_DSP_ENTER
_log_dsp_enter_dis:
	.long _log_dsp_enter
#endif /* LOG_DSP_ENTER */
#ifdef LOG_DSP_LEAVE
_log_dsp_leave_dis:
	.long _log_dsp_leave
#endif /* LOG_DSP_LEAVE */

p_migrate_self:
    .long _migrate_self
p_exit_and_migrate_self:
    .long _exit_and_migrate_self


/*
 *	カーネルの終了処理の呼出し
 *
 *	スタックを非タスクコンテキスト用に切り替え、
 *	exit_kernelへ分岐する。
 *	この関数は，全割込みロック状態で呼ばれる
 */
	.globl _call_exit_kernel
_call_exit_kernel:
	mov.l _exit_kernel_exit,r0
	stc   gbr, r7					/* r7←my_p_pcb */
	jmp   @r0
	 mov.l @(PCB_istkpt,r7),r15	/* スタック切り替え		   */

	.align 2
_exit_kernel_exit:
	.long _exit_kernel


/*
 *	タスク起動処理
 *
 *	ディスパッチャから、以下の状態でここにくる。
 *  　・タスクコンテキスト
 *  　・CPUロック状態
 *  　・割込み優先度マスク全解除状態
 *  　・ディスパッチ許可状態
 *  　・r0←my_p_runtsk
 */
	.text
	.globl _start_r
_start_r:
	/*
	 *  割込みを許可したら、CPUロック解除状態になるよう準備する
	 *  　　my_p_tacb->lock_flag←false
	 */
	mov    #0,r2
	stc    gbr, r7				 /* r7←my_p_pcb */
	mov.l  r2, @(PCB_lock_flag, r7)
	ldc    r2,sr				 /* 割込み許可 */
	mov.l  @(TCB_p_tinib,r0),r3  /* r3←my_p_runtsk->p_tinib  */
	mov.l  _ext_tsk_start,r5
	mov.l  @(TINIB_task,r3),r1	 /* r1←タスク起動番地	 */
	lds    r5,pr				 /* 戻り番地を設定 */
	jmp    @r1
	 mov.l @(TINIB_exinf,r3),r4  /* exinfを引数レジスタr4に  */

	.align 2
_ext_tsk_start:
	.long _ext_tsk


/*
 *  割込みの際のスタックポインタが指し示す位置からSR,PCのコピーがある
 *  場所までのオフセット
 *　　
 *　　SP_PC_OFFSET_INT：_ret_int_2の時点での、スタックに積まれた
 *　　　　　　　　　　　PCへのオフセット
 *　　SP_SR_OFFSET_INT：_ret_int_3の時点での、スタックに積まれた
 *　　　　　　　　　　　SRへのオフセット
 *　　
 *　　
 * スタック切り替え前のスタック
 * （割込み受付時のコンテキスト用スタック）
 *
 * (1) レジスタバンクを使用しない場合
 *        ----------------------
 *  0    |         PR           |   <-- _ret_int_2 / _ret_int_3 の時点
 *        ----------------------
 *  1    |        MACH          |
 *        ----------------------
 *  2    |        MACL          |
 *        ----------------------
 *  3    |         R0           |
 *        ----------------------
 *  4    |         R1           |
 *        ----------------------
 *  5    |         R2           |
 *        ----------------------
 *  6    |         R3           |
 *        ----------------------
 *  7    |         R4           |
 *        ----------------------
 *  8    |         R5           |
 *        ----------------------
 *  9    |         R6           |
 *        ----------------------
 * 10    |         R7           |
 *        ----------------------
 * 11    |         PC           |
 *        ----------------------
 * 12    |         SR           |
 *        ----------------------
 *
 * (2) レジスタバンクを使用する場合
 *        ----------------------
 *  0    |         PR           |   <-- _ret_int_3 の時点
 *        ----------------------
 *  1    |        MACH          |
 *        ----------------------
 *  2    |        MACL          |
 *        ----------------------
 *  3    |         R0           |
 *        ----------------------
 *  4    |         R1           |
 *        ----------------------
 *  5    |         R2           |
 *        ----------------------
 *  6    |         R3           |
 *        ----------------------
 *  7    |         R4           |
 *        ----------------------
 *  8    |         R5           |
 *        ----------------------
 *  9    |         R6           |
 *        ----------------------
 * 10    |         R7           |
 *        ----------------------
 * 11    |         PC           |   <-- _ret_int_2 の時点
 *        ----------------------
 * 12    |         SR           |
 *        ----------------------
 */
#ifdef USE_BANKED_REG
#define    SP_PC_OFFSET_INT    0
#else /* USE_BANKED_REG  */
#define    SP_PC_OFFSET_INT    (11*4)
#endif /* USE_BANKED_REG */

#define    SP_SR_OFFSET_INT    (12*4)


/*
 *	共通割込みの入口処理
 *　　割込み要因に依らず、共通な部分
 *　　（コンフィギュレータが生成する入口処理（kernel_cfg_asm.S）からの
 *　　　続き）
 *	
 *	割込み要因毎に展開されるルーチンでr4,r5,r6,r7を保存し、
 *　　r5：割込み番号
 *　　r6：割込みハンドラの先頭アドレス
 *　　r7：割込み受付直後のSRのコピー
 *	の状態でここに来る。
 *	（r4に割り当てないのは、CPU例外ハンドラの処理となるべく共通にするため）
 */
	.text
	.globl _int_entry
_int_entry:
#ifndef  USE_BANKED_REG
    movml.l r3, @-r15	/*  残りのスクラッチレジスタをスタックに積む  */
    sts.l  mach,@-r15
    sts.l  macl,@-r15
    sts.l  pr,@-r15
#endif /* USE_BANKED_REG */
							/* 割込み/CPU例外ネストカウンタのチェック */
	stc    gbr, r3				/* r3←my_p_pcb */
	mov.l  @(PCB_excnest_count, r3),r1
	tst    r1,r1				/* 割込み発生時のコンテキストを判定  */
	add    #0x1,r1				/* 割込みネストカウンタをインクリメント */
    bf/s   _int_entry_1         /* 多重割込みならジャンプ  */
	 mov.l  r1, @(PCB_excnest_count, r3)
	mov    r15,r2				/* スタック切り替え　*/
	mov.l  @(PCB_istkpt, r3),r15
	mov.l  r2,@-r15 			 /* タスクスタックの保存 */
_int_entry_1:
	ldc    r7,sr				 /* 割込み許可	*/
	/*
	 * 割込み番号を保存
	 * 　ここで保存した割込み番号には、２つの意味がある。
	 * 　　・default_int_handlerの引数
	 * 　　・log_inh_leave_intの引数
	 */
	mov.l  r5,@-r15

#ifdef LOG_INH_ENTER
	mov.l _log_inh_enter_int, r1 /* ログ出力関数 */
	mov.l  r6,@-r15 	  /* r6（割込みハンドラの先頭アドレス）保存 */
	jsr   @r1
	 mov  r5, r4				 /* 引数		  */
	mov.l @r15+, r6 			 /* r6復帰		  */
#endif /* LOG_INH_ENTER */

	jsr    @r6					 /* 割込みハンドラ呼び出し  */
#ifndef LOG_INH_LEAVE
	  mov.l @r15+,r4			 /* default_int_handler用の引数 */
	  							 /*  （割込み番号）  */
#else  /* LOG_INH_LEAVE */
	  mov.l  @r15, r4			 /* default_int_handler用の引数 */
	  							 /*  （スタックはそのまま）  　 */
	mov.l _log_inh_leave_int, r1 /* ログ出力関数				*/
	jsr  @r1
	 mov.l @r15+,r4 			 /* 割込み番号を復帰			*/
#endif /* LOG_INH_LEAVE */

	/*
	 * 割込みハンドラ出口処理
	 *
	 *	割込み/CPU例外ネストカウンタをデクリメントし，戻り先のコンテキ
	 *	ストを判定してスタックを入れ替える．
	 *	カーネル管理外の割込みをサポートする場合は，両者の間にカーネル
	 *	管理外の割込みが入る場合がある．カーネル管理外の割込みの入口は，
	 *	カーネル管理の割込みと同様であるため，割込み/CPU例外ネストカウ
	 *	ンタを参照して，スタックを入れ替える．そのため，割込み/CPU例外
	 *	ネストカウンタとスタックの入れ替えはカーネル管理外の割込みに対
	 *	しても，不可分に行う必要がある．
	 */
_ret_int:
	movi20 #IIPM_DISALL,r2	/*  全割込み（カーネル管理外も含め）禁止  */
	ldc    r2,sr
    /*
     *　割込み/CPU例外ネストカウンタをデクリメントし、
     *　戻り先が非タスクコンテキストならすぐにリターン
     */
	stc    gbr, r7				 /* r7←my_p_pcb */
	mov.l  @(PCB_excnest_count, r7),r1
	dt     r1
	mov.l r1,@(PCB_excnest_count, r7)
	bf    _ret_int_1			 /* 32ビット命令なので、遅延スロットに配置できない */

	mov.l  @r15,r15 			 /* 戻り先がタスクなのでスタックを戻す	   */
	mov.l  @(PCB_reqflg, r7),r1	 /* reqflgのチェック */
	tst    r1,r1				 /* 	reqflgがfalseならret_int_1へ	   */
	bt	   _ret_int_1
	movi20 #IIPM_LOCK,r0
	/*
	 * 戻り先がタスクでreqflgがセットされている場合の処理へ分岐
	 * 　ret_int_2に分岐してディスパッチしない場合があるのでreqflgはクリアしない
	 */
#ifdef TOPPERS_LDC_IS_SLOT_ILLEGAL_INSTRUCTION
	ldc    r0,sr				 /* カーネル管理外の割込みを許可			  */
	bra   _ret_int_2
	 nop
#else	/* TOPPERS_LDC_IS_SLOT_ILLEGAL_INSTRUCTION */
	bra   _ret_int_2
	 ldc	r0,sr				  /* カーネル管理外の割込みを許可			  */
#endif	/* TOPPERS_LDC_IS_SLOT_ILLEGAL_INSTRUCTION */


	/*
	 *	割込み処理からのリターン
	 */
_ret_int_1:
	/*
	 *	割込み処理からのリターンにより，CPUロック解除状態に移行するよう
	 *	準備する．割込み優先度マスクは，RTE命令により元の値に戻るため，
	 *	lock_flagをfalseにしておけばよい（lock_flagをfalseにするのは，
	 *	CPUロック状態のまま割込みハンドラから戻った時の対策）．
	 */
	mov    #0x0, r1
	stc    gbr, r7						/* r7←my_p_pcb */
	mov.l  r1,@(PCB_lock_flag,r7)

#ifdef USE_BANKED_REG
	resbank
#else	/*  USE_BANKED_REG  */
	lds.l  @r15+,pr
	lds.l  @r15+,macl
	lds.l  @r15+,mach
	movml.l @r15+,r7
#endif	/* USE_BANKED_REG */
	rte
	 nop

	.align 2
#ifdef LOG_INH_ENTER
_log_inh_enter_int:
	.long  _log_inh_enter
#endif /* LOG_INH_ENTER */
#ifdef LOG_INH_LEAVE
_log_inh_leave_int:
	.long  _log_inh_leave
#endif /* LOG_INH_LEAVE */

	/*
	 *	ここへは割込みハンドラの出口処理から分岐してくる
	 *	
	 *	戻り先がタスクでreqflgがセットされている場合のみここにくる。
	 */
_ret_int_2:
	/*
	 *　CPU例外/割込み受付直後に割込み禁止になっていないことへの対策
	 *　（戻り先が割込み／CPU例外の入口処理か否かの判別）
	 *　
	 *　SH2A-DUALではCPU例外/割込み受付直後に割込み禁止になっていないため、
	 *	割込みAの入口処理中に別の割込みBが入る可能性がある。
	 * （これはハードウェアのアーキテクチャ上避けようがない。）
	 *	割込みハンドラB内でタスク切り替えを起こすようなサービスコール
	 *	を呼ぶと割込みBの出口処理で別のタスクにディスパッチしてしまい、
	 *	タスク2から元のタスクに戻ってくるまで、割込みAの処理が遅れてし
	 *	まう。
	 *	これを防ぐため、割込み/例外の入口処理をまとめてvector_entryセ
	 *	クションに置き，戻り先のアドレスがこの範囲内ならディスパッチ
	 *	せずにリターンする．
	 */
	mov.l  @(SP_PC_OFFSET_INT,r15),r0	/* r0 <-スタックに積まれたpc */
	mov.l  _vector_entry_start_ret, r1
	mov.l  _vector_entry_end_ret, r2
	cmp/hs r1, r0
	bf	   _ret_int_2_1
	cmp/hi r0, r2
	bt	   _ret_int_1

	/*
	 *	ディスパッチャもしくはタスク例外処理の呼び出し
	 */
_ret_int_2_1:
	/*
	 *	レジスタバンクを使用する場合はスクラッチレジスタを
	 *	スタックに格納する
	 */
#ifdef USE_BANKED_REG
	resbank
	movml.l  r7, @-r15
	sts.l	 mach,@-r15
	sts.l	 macl,@-r15
	sts.l	 pr,@-r15
#endif /* USE_BANKED_REG */

	/*
	 *	ディスパッチャの呼び出し
	 *
	 *	ここへは，CPU例外ハンドラの出口処理からも分岐してくる．
	 *
	 *	ここでは，戻り先がタスクであり，スタックは，タスクスタックに
	 *	スクラッチレジスタのみが保存された状態になっている．また，
	 *	カーネル管理の割込みを禁止した状態となっている．
	 */
_ret_int_3:
	/*
	 *　reqflgのクリア
	 *　
	 *　ここに到達する前にreqflgをクリアしてしまうと、上記の割込みA
	 *　の出口処理でディスパッチャ呼び出しが必要なケースでもディス
	 *　パッチャが呼び出されない。
	 *　（割込みBでreqflgをセットして、割込みAでreqflgに変化がない場
	 *　　合）
	 */
	mov    #0x0, r0
	stc    gbr, r7					/*  r7←my_p_pcb */
	mov.l  r0, @(PCB_reqflg, r7)	/*  reqflg←false  */

	/*
	 *	CPUロック状態に移行する．カーネル管理の割込みは既に禁止して
	 *	いるので，lock_flagとsaved_iipmを更新する．saved_iipmは，戻り
	 *	先の割込み優先度マスク（の内部表現）に設定する．
	 *
	 *	この時点でCPUロック状態とするのは，dispatcherへ分岐する時と，
	 *	dispatch_call_texrtnを呼び出す時に，CPUロック状態になっている
	 *	必要があるためである．
	 */
	mov    #0x01,r1
	mov.l  r1,@(PCB_lock_flag,r7)	/* lock_flag←true */

	/*
	 * スタック上に保存されている割込み前のIPMを取り出して、saved_iipmに
	 *
	 *　　タスクコンテキストに戻るのに、単にsaved_iipm←0としない
	 *　　（スタック上に保存されているSRからわざわざIPMを取り出す）のは、
	 *　　割込み発生前のタスクコンテキストでchg_ipmを実行し、
	 *　　割込み優先度マスク全解除状態でない状態で、割込み/CPU例外が
	 *　　発生し、ここに到達した場合に対応するためである。
	 */
	mov.l  @(SP_SR_OFFSET_INT,r15),r0
	and    #0xf0,r0		/* and #imm,r0命令はr0のみ指定可能  */
	mov.l  r0,@(PCB_saved_iipm,gbr)
			/* saved_iimp←スタック上に積まれているSRのIPM  */

	/*
	 *	dspflgがfalseである場合と，p_runtskとp_schedtskが同じ場合には，
	 *	ディスパッチを行わない．このチェックが必要なのは，タスク例外処
	 *	理ルーチンの呼出しが必要な場合に，ディスパッチが必要なくても，
	 *	reqflgをtrueにするためである．
	 */
	mov.l  @(PCB_dspflg,r7),r3	  /* r3 <- dspflg					*/
	mov.l  @(PCB_p_runtsk,gbr),r0 /* r0 <- p_runtsk					*/
	tst    r3,r3
	bt	   _ret_int_r_1			/* dspflgがfalseならret_int_r_1へ 	*/
	mov.l  @(PCB_p_schedtsk,r7),r5 /* r5 <- p_schedtsk				*/
	cmp/eq r0,r5				/* runtsk と schedtsk を比較	 	*/
	bt	   _ret_int_r_1 		/* 同じならret_int_r_1へ		 	*/
	/*
	 *	注意
	 *	　r8-r14に加えて、prが重複してスタックに保存されるが
	 *	　_ret_int_r側で読み出すときにも、整合させているので問題ない。
	 */
	movmu.l r8, @-r15			/*  r8-r14,prをスタックに保存  	*/
	mov.l  r15,@(TCB_sp, r0)	/* タスクスタックをTCBに保存	  */
	mov.l  _ret_int_r_ret,r3
	bra    _dispatcher
	 mov.l	r3,@(TCB_pc, r0)	/* 実行再開番地をTCBに保存		  */

_ret_int_r:
	/*
	 *	注意
	 *	　ここに分岐してくるのは_ret_int_3からのみである。
	 *	　_ret_int_3側でr8-r14に加えて、prもスタックに保存している。
	 */
	movmu.l @r15+,r8			/*  r8-r14,prをスタックから復帰  */
_ret_int_r_1:
	/*
	 *	タスク例外処理ルーチンの起動
	 *	ret_int_r は dispatcher から呼び出されるため，tcbのアドレスはr0に
	 *	入っている．
	 *	なお，割込みの出口でタスクスイッチが発生しない場合はディスパッチャ
	 *	を通らずに ret_int から直接 ret_int_1 に飛ぶ
	 */
	/*  enatexのチェック  */
	bld.b  #TCB_enatex_bit, @(TCB_enatex,r0)
	bf     _ret_int_r_return	/* enatexがfalseなら割込み発生元へリターン */

	/* texptnが0で無ければ、タスク例外処理ルーチンの呼び出し */
	mov.l  @(TCB_texptn,r0),r1	/* texptnをロード */
	tst    r1,r1
	bt	   _ret_int_r_return	/* texptnが0なら割込み発生元へリターン  */
	mov.l  _dispatch_call_texrtn_ret,r2
	jsr/n  @r2					/* タスク例外処理ルーチンの呼び出し */

	/*  割込み発生元へリターン  */
_ret_int_r_return:
	mov    #0x0,r0					/* lock_flagをfalseに */
	lds.l  @r15+,pr
	mov.l  r0, @(PCB_lock_flag,gbr)
	lds.l  @r15+,macl
	lds.l  @r15+,mach
	movml.l @r15+,r7
	rte
	 nop

	.align 2
_ret_int_r_ret:
	.long _ret_int_r
_dispatch_call_texrtn_ret:
	.long _dispatch_call_texrtn
_vector_entry_start_ret:
	.long __vector_entry_start
_vector_entry_end_ret:
	.long __vector_entry_end

/*
 *	共通CPU例外入口処理
 *
 *  CPU例外ハンドラは，非タスクコンテキストで実行する．そのため，CPU例
 *  外ハンドラを呼び出す前に非タスクコンテキストに切替え，リターンして
 *  きた後に元のコンテキストに戻す．元のコンテキストに戻すために，ス
 *  タック切替えする前のタスクスタックのスタックポインタを非タスクコン
 *  テキスト用スタック上に保存する．
 *  CPU例外がタスクコンテキストで発生し，reqflg が true になった時に，
 *  _ret_exc_2 へ分岐する．
 *	reqflg をチェックする前に割込みを禁止しないと，reqflg をチェック後
 *	に起動された割込みハンドラ内でディスパッチが要求された場合に，ディ
 *	スパッチされない．
 *	　
 *	一般不当命令の場合は戻り番地を2バイト進める必要があるが
 *	対応していない
 *	　
 *	CPU例外要因毎に展開されるルーチンでr4,r5,r6,r7を保存し、
 *	 r5 ： CPU例外ハンドラ番号
 *	 r6 ： CPU例外ハンドラの先頭アドレス
 *	 r7 ： CPU例外発生時のSRのコピー
 *	の状態でここに来る。
 *	
 * (1) CPU例外発生時のコンテキスト用スタック
 *		  ----------------------
 *	0	 | CPU例外ハンドラ番号	|	<-- p_excinf（CPU例外ハンドラの引数）、
 *		  ----------------------		_ret_exc_non_taskの時点（CPU例外発生時、非タスクコンテキストの場合）
 *	1	 |		   PR			|	<-- _ret_exc_1/_ret_exc_2の時点
 *		  ----------------------
 *	2	 |		  MACH			|
 *		  ----------------------
 *	3	 |		  MACL			|
 *		  ----------------------
 *	4	 |		   R0			|
 *		  ----------------------
 *	5	 |		   R1			|
 *		  ----------------------
 *	6	 |		   R2			|
 *		  ----------------------
 *	7	 |		   R3			|
 *		  ----------------------
 *	8	 |		   R4			|  <-- sp（_exc_entryの時点）
 *		  ----------------------
 *	9	 |		   R5			|
 *		  ----------------------
 * 10	 |		   R6			|
 *		  ----------------------
 * 11	 |		   R7			|
 *		  ----------------------
 * 12	 |		   PC			|	<-- sp（CPU例外受付直後の時点）
 *		  ----------------------
 * 13	 |		   SR			|
 *		  ----------------------
 *	
 * (2) コンテキスト切り替え後の非タスクコンテキスト用スタック
 *		  ----------------------
 *		 |	 CPU例外ハンドラの	|（log_exc_enterを呼び出す場合）
 *		 |	 先頭アドレス(r6)	|
 *		  ----------------------
 *		 |	   p_excinf(r4) 	|（log_exc_enterを呼び出す場合）
 *		  ----------------------
 *		 | CPU例外ハンドラ番号	|（log_exc_leaveを呼び出す場合）
 *		  ----------------------
 *		 |	   saved_iipm		|  <-- sp（_ret_excの時点）
 *		  ----------------------
 *		 |	   lock_flag		|
 *		  ----------------------
 *		 |	   locspinid		|
 *		  ----------------------
 *		 | タスクスタックのsp	|（スタック切替をした場合）
 *		  ----------------------
 */

/*
 *	_ret_exc_2の時点で、スタックポインタの値から
 *  「スタックに積まれているCPU例外からの戻り番地」までのオフセット
 */
#define    SP_PC_OFFSET_EXC     (11 * 4)

	.text
	.globl _exc_entry
_exc_entry:
	movml.l r3,@-r15 	/*	残りのスクラッチレジスタをスタックに積む  */
	sts.l  mach,@-r15
	sts.l  macl,@-r15
	sts.l  pr,@-r15
	mov.l  r5,@-r15 			/* CPU例外ハンドラ番号を保存  */
								/* 割込み/CPU例外ネストカウンタのチェック */
	stc    gbr, r3				/* r3←my_p_pcb */
	mov.l  @(PCB_excnest_count,r3), r1
	mov    r15, r4				 /* CPU例外ハンドラへの引数 */
	tst    r1,r1				 /* 割込み発生時のコンテキストを判定	 */
	add    #0x1,r1				 /* 割込み/CPU例外ネストカウンタをインクリメント */
    bf/s   _exc_entry_1          /* CPU例外発生時に非タスクコンテキストならジャンプ */
	 mov.l r1, @(PCB_excnest_count,r3)
	mov.l @(PCB_istkpt,r3),r15	 /* 非タスクスタックに切り替え			 */
	mov.l  r4,@-r15 			 /* タスクスタックポインタの保存 */
_exc_entry_1:
    /*
     * 元のスピンロック取得状態／CPUロック状態／解除状態をスタックに保存
     */
	mov.l  @(PCB_locspnid, gbr), r0
	mov.l  @(PCB_lock_flag, r3), r1
	mov.l  @(PCB_saved_iipm,r3), r2
	mov.l  r0,@-r15
	mov.l  r1,@-r15
	mov.l  r2,@-r15

#ifdef LOG_EXC_LEAVE
	mov.l  r5,@-r15 			 /* CPU例外ハンドラ番号を保存 */
#endif /* LOG_EXC_LEAVE */

#ifdef LOG_EXC_ENTER
	ldc    r7,sr				 /* 割込み許可	*/
	mov.l _log_exc_enter_exc, r1 /* ログ出力関数  */
	mov.l  r4,@-r15 			 /* r4保存（CPU例外ハンドラの引数） */
	mov    r5, r4				 /* 引数（CPU例外ハンドラ番号）*/
	jsr   @r1
	 mov.l  r6,@-r15 			 /* r6保存（CPU例外ハンドラの先頭アドレス）*/

	mov.l @r15+, r6 			 /* r6復帰		  */
	jsr    @r6					 /* CPU例外ハンドラ呼び出し  */
	 mov.l @r15+, r4			 /* r4復帰		  */
#else /* LOG_EXC_ENTER */

#ifdef TOPPERS_LDC_IS_SLOT_ILLEGAL_INSTRUCTION
	ldc    r7,sr				 /* 割込み許可	*/
	jsr    @r6					 /* CPU例外ハンドラ呼び出し  */
	 nop
#else	/* TOPPERS_LDC_IS_SLOT_ILLEGAL_INSTRUCTION */
	jsr    @r6					 /* CPU例外ハンドラ呼び出し  */
	 ldc	r7,sr				 /* 割込み許可	*/
#endif	/* TOPPERS_LDC_IS_SLOT_ILLEGAL_INSTRUCTION */

#endif /* LOG_EXC_ENTER */

#ifdef LOG_EXC_LEAVE
	mov.l _log_exc_leave_exc, r1 /* ログ出力関数						*/
	jsr  @r1
	 mov.l @r15+, r4             /* 引数：CPU例外ハンドラ番号           */
#endif /* LOG_EXC_LEAVE */

	/*
	 * CPU例外ハンドラ出口処理
	 */
_ret_exc:
	/*
	 * 要検討
	 * 　excnestの更新とスタック切り替えを不可分に行うには、
	 * 　カーネル管理外の割込みも禁止する必要がある。
	 * 　　→カーネル管理外の割込みの有無で処理が変わる。
	 */
	movi20 #IIPM_DISALL, r0				/* 全割込み禁止	 */
	ldc    r0,sr
    /*
     * スピンロック取得状態／CPUロック状態／解除状態を元の状態に戻す
     */
	stc    gbr, r7						/* r7←my_p_pcb */
    mov.l  @r15+,r0						/* saved_iipm */
    mov.l  @r15+,r1						/* lock_flag  */
    mov.l  @r15+,r2						/* locspinid  */
	mov.l  r0, @(PCB_saved_iipm,gbr)
	mov.l  r1, @(PCB_lock_flag, r7)
	tst    r2,r2
	mov.l  r2, @(PCB_locspnid, r7)
	
	/*  locspinid=0ならば、force_unlock_spin(my_p_pdb)を呼び出し  */
	bf     _ret_exc_after_force_unlock_spin
	mov.l  _force_unlock_spin_exc, r3
	jsr    @r3
	 stc   gbr, r4					/*  引数：my_p_pdb  */
_ret_exc_after_force_unlock_spin:

    /*
     * コンテキスト切り替え
	 * 　「スタックに積まれたCPU例外ハンドラ番号を読み飛ばす」処理（ス
	 * 　タックポインタのインクリメント）はスタック切り替え後でないと
	 * 　できないため、戻り先のコンテキスト判定後、2箇所で行っている。
	 * 　（スタック切り替え直後と_ret_exc_non_task）
     */
	/*
	 *	要検討
	 *	　ここで発生したカーネル管理外の割込みで破壊しないように退避
	 */
    /*  　要検討：ここから、不可分  */
	mov    r15,r2
    /*
     *　割込み/CPU例外ネストカウンタをデクリメントし、
     *　戻り先が非タスクコンテキストならすぐにリターン
     */
	stc    gbr, r7					/* r7←my_p_pcb */
	mov.l  @(PCB_excnest_count,r7), r1
    dt     r1
    bf/s   _ret_exc_non_task
	 mov.l r1, @(PCB_excnest_count,r7)

	mov.l  @r2,r15			/* 戻り先がタスクなのでスタックを戻す	  */
    /*  要検討：ここまで、不可分  */

	mov.l  @(PCB_reqflg,r7), r1	/* reqflgのチェック */
	add    #4, r15
		/* スタックに積まれたCPU例外ハンドラ番号を読み飛ばす */
	
	/*
	 *　reqflgがtrueならret_exc_2へ
	 *　　ret_exc_2に分岐してディスパッチしない場合があるのでreqflgは
	 *　　クリアしない
	 */
	tst    r1,r1
	bf	  _ret_exc_2


    /*
	 *　ディスパッチャを呼ばずにリターンする場合
     */
_ret_exc_1:
	lds.l  @r15+,pr
	lds.l  @r15+,macl
	lds.l  @r15+,mach
	movml.l @r15+,r7
	rte
	 nop

	/*
	 *　戻り先が非タスクコンテキスト
	 *　　スタックに積まれているCPU例外ハンドラ番号を読み飛ばし、
	 *　　_ret_exc_1へ分岐
	 */
_ret_exc_non_task:
	bra    _ret_exc_1
	 add   #4, r15
	 	/* スタックに積まれたCPU例外ハンドラ番号を読み飛ばす */

	/*
	 *　戻り先がタスクコンテキストで、reqflg=trueの場合
	 *　（ディスパッチャまたはタスク例外処理ルーチンの呼び出し）
	 *　
	 *　割込みの場合（_ret_int_2）と共通にできないのは、
	 *　レジスタバンクがあり、レジスタ退避のタイミングが異なるため。
	 */
_ret_exc_2:
	/*
	 *　戻り先が割込み／CPU例外の入口処理か否かの判別
	 *	詳しくはret_int_2を参照のこと
	 */
	mov.l  @(SP_PC_OFFSET_EXC,r15),r0	/* r0 <-スタックに積まれたPC */
	mov.l  _vector_entry_start_exc, r1
	mov.l  _vector_entry_end_exc, r2
	cmp/hs r1, r0
	bf	   _ret_exc_2_1			/*  ディスパッチャを呼び出す場合  */
	cmp/hi r0, r2
	bt	   _ret_exc_1			/*  ディスパッチャを呼ばずにリターン  */

_ret_exc_2_1:
	bra    _ret_int_3			/*  割込みの場合と共通の処理  */
	 nop
	
	.align 2
_force_unlock_spin_exc:
	.long  _force_unlock_spin
_vector_entry_start_exc:
	.long __vector_entry_start
_vector_entry_end_exc:
	.long __vector_entry_end
#ifdef LOG_EXC_ENTER
_log_exc_enter_exc:
	.long _log_exc_enter
#endif /* LOG_EXC_ENTER */
#ifdef LOG_EXC_LEAVE
_log_exc_leave_exc:
	.long _log_exc_leave
#endif /* LOG_EXC_LEAVE */

/*
 *	未登録の割込みハンドラの入口処理
 *	　割込み要因毎の入口処理でr7,r6を保存し、
 *	　r6に割込み番号を格納して、ここに来る。
 *　　以下のレジスタ割当で、_int_entryへ分岐する。
 *　　
 *　　レジスタ割当
 *　　　　r7：割込み受付直後のSRのコピー
 *　　　　r6：割込みハンドラの先頭アドレス(_default_int_handler)
 *　　　　r5：割込み番号
 *　　　　r4：_int_entry
 *　　　　　（割込みハンドラが登録されている場合と共通の処理）
 */
	.section .vector_entry,"xa"
	.align 2
	.globl  _default_int_handler_entry
_default_int_handler_entry:
#ifndef USE_BANKED_REG
	mov.l r5,@-r15				   /*  必要最小限のレジスタをスタックに待避  */
	mov.l r4,@-r15
#endif /* USE_BANKED_REG */
	mov   r6,r5						/* 割込み番号 */
	movi20 #IIPM_DISALL,r6
	stc   sr,r7						/* SRの保存 */
	ldc   r6,sr						/* 割込み禁止 */
	mov.l _common_routine_entry,r4	/* 共有割込み入口処理 */
	mov.l _c_routine_entry,r6		/* 割込みハンドラの先頭アドレス */
	/*	（PC相対アドレス形式なので、遅延スロットには入れられない。）  */
	jmp   @r4
	 nop

	.align 2
_c_routine_entry:
	.long _default_int_handler
_common_routine_entry:
	.long  _int_entry

/*
 *	未登録のCPU例外ハンドラの入口処理
 *	　CPU例外要因毎の入口処理でr7,r6を保存し、
 *	　r6にベクタ番号を格納して、ここに来る。
 *　　以下のレジスタ割当で、_exc_entryへ分岐する。
 *　　
 *　　レジスタ割当
 *　　　　r7：CPU例外発生時のSRのコピー
 *　　　　r6：CPU例外ハンドラの先頭アドレス(_default_exc_handler)
 *　　　　r5：ベクタ番号
 *　　　　r4：_exc_entry
 *　　　　　（CPU例外ハンドラが登録されている場合と共通の処理）
 */
	.section .vector_entry,"xa"
	.align 2
	.globl  _default_exc_handler_entry
_default_exc_handler_entry:
		 /*  必要最小限のレジスタをスタックに待避  */
	mov.l r5,@-r15
	mov.l r4,@-r15
	mov   r6,r5							/* ベクタ番号をコピー	*/
	movi20 #IIPM_DISALL,r6
	stc   sr,r7							/* SRの保存   */
	ldc	  r6,sr							/* 全割込み禁止 */
										/* 共有CPU例外入口処理 */ 
	mov.l _common_routine_exc_entry,r4
	mov.l _c_routine_exc_entry,r6	/* CPU例外ハンドラの先頭アドレス */
	/*	（PC相対アドレス形式なので、遅延スロットには入れられない。）  */
	jmp	  @r4
	 nop
		/*
		 *	ldc命令を遅延スロットに配置しないのは、
		 *	空いているレジスタがないため
		 */

	.align 2
_common_routine_exc_entry:		/* CPU例外共通の入口処理 */
	.long  _exc_entry
_c_routine_exc_entry:			/* CPU例外ハンドラの先頭アドレス */
	.long  _default_exc_handler


/*
 *	微少時間待ち
 */
	.text
	.align 2
	.globl _sil_dly_nse
_sil_dly_nse:
	mov.l  _sil_dly_tim1, r1
	add    r1, r4
	cmp/pl r4
	bt	   _sil_dly_nse1
	rts/n
_sil_dly_nse1:
	mov.l  _sil_dly_tim2, r1
	add    r1, r4
	cmp/pl r4
	bt	   _sil_dly_nse1
	rts/n

	.align 2
_sil_dly_tim1:
	.long  -SIL_DLY_TIM1
_sil_dly_tim2:
	.long  -SIL_DLY_TIM2
