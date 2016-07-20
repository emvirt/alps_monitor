/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2008-2010 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: prc_support.S 614 2010-05-11 09:57:42Z ertl-honda $
 */

/*
 *  プロセッサ依存モジュール アセンブリ言語部（Nios2用）
 */

#define TOPPERS_MACRO_ONLY
#define UINT_C(val)		(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)	(val)		/* ulong_t型の定数を作るマクロ */
#define CAST(type, val)	(val)		/* 型キャストを行うマクロ */

/*
 *  アセンブラ用のマクロを有効にする
 */
#define TOPPERS_ASM_MACRO
#include "kernel_impl.h"
#include "offset.h"
#include "prc_asm.inc"

/*
 * 標準外の例外を用いる
 */
#define USE_EXTRA_EXCEPTION

/*
 * スタック操作 : プッシュ
 */
 .macro Push source
 addi  sp, sp, -4
 stw   \source, 0(sp)
 .endm	

/*
 * スタック操作 : ポップ
 */
 .macro Pop source
 ldw   \source, 0(sp)
 addi  sp, sp, 4
 .endm

/*
 *  例外/割込みエントリ
 */
	.set noat
	.section .exceptions, "xa"
	.align 2
	.global exception_entry
exception_entry:
	/*
	 *  例外要因の判定
	 */
	rdctl   et, estatus
	andi    et, et, 1
	beq     et, zero, _check_trap
	rdctl   et, ipending
	beq     et, zero, _check_trap

/*
 *  例外要因が割込みの場合
 */
interrupt:
	addi ea, ea, -4      /* 戻り番地をデクリメント */

	/*
	 *  コンテキストの保存
	 */
	addi  sp, sp, -76    
	rdctl et, estatus
	stw   et,   0(sp)
	stw   at,   4(sp)
	stw   r2,   8(sp)
	stw   r3,  12(sp)
	stw   r4,  16(sp)
	stw   r5,  20(sp)
	stw   r6,  24(sp)
	stw   r7,  28(sp)
	stw   r8,  32(sp)
	stw   r9,  36(sp)
	stw   r10, 40(sp)
	stw   r11, 44(sp)
	stw   r12, 48(sp)
	stw   r13, 52(sp)
	stw   r14, 56(sp)
	stw   r15, 60(sp)
	stw   fp,  64(sp)
	stw   ra,  68(sp)
	stw   ea,  72(sp)

	my_pcb r8, r2             /* pcbの読み込み */
#if TTYPE_KLOCK != G_KLOCK
release_lock:
	ldw    r4, PCB_p_firstlock(r8)
	beq    r4, zero, release_lock_end
	rdctl  r2, cpuid
	slli   r2,r2,16
	stw    zero, PCB_p_firstlock(r8)
	ldw    r3, (r4)
	stw    r2, (r3)
	ldw    r4, PCB_p_secondlock(r8)
	beq    r4, zero, release_lock_end
	stw    zero, PCB_p_secondlock(r8)
	ldw    r3, (r4)
	stw    r2, (r3)
release_lock_end:
#endif /* TTYPE_KLOCK != G_KLOCK */

	/*
	 *  割り込み要因の取得 
	 */
#ifdef NIOS2_USE_INT_VEC_INST
	/*
	 *  Interrupt Vector Instruction 命令を持つ場合
	 */
	custom NIOS2_INT_VEC_INST_NO, r4, r0, r0  /* 割込み番号の読み込み(8倍したものが読み込める) */
	srli   r4, r4, 3      /* 8分の1にする */
#else
	/*
	 *  ipending 中の1のビットの内，最も下位（右）のものをサーチ
	 */
int_bitmap_search:
	rdctl r4, ipending
	andi  r2, r4, 65535
	mov   r5, zero
	bne   r2, zero, int_bitmap_search1
	srli  r4, r4, 16
	movi  r5, 16
int_bitmap_search1:
	andi  r2, r4, 255
	bne   r2, zero, int_bitmap_search2
	srli  r4, r4, 8
	addi  r5, r5, 8
int_bitmap_search2:
	andi  r2, r4, 15
	srli  r4, r4, 4
	bne   r2, zero, int_bitmap_search3
	addi  r5, r5, 4
	andi  r2, r4, 15
int_bitmap_search3:
	movhi r3, %hiadj(int_bitmap_search_table-1)
	addi  r3, r3, %lo(int_bitmap_search_table-1)
	add   r3, r2, r3
	ldbu  r2, 0(r3)
	add   r4, r2, r5
#endif

#ifdef USE_IPI_DIS_HANDER_BYPASS
	movi r2, PRC_INT_INT
	beq  r4, r2, ret_int_ipi_dispatch
#endif /* USE_IPI_DIS_HANDER_BYPASS */

	/*
	 *  多重割込みか判定
	 */
	ldw   r3, PCB_except_nest_count(r8)  /* ネスト回数のチェック */
	bltu  zero, r3,  nest_int

	/*
	 * スタックポインタの入れ替え
	 */
	ldw   r2, PCB_istkpt(r8)
	addi  r2, r2, -4
	stw   sp, 0(r2)     /* スタックポインタの保存 */        
	mov   sp, r2        /* スタックポインタの入れ替え */

nest_int:
	/* 
	 *  割込み・例外のネスト回数のインクリメント
	 */
	addi  r3, r3, 1
	stw   r3, PCB_except_nest_count(r8)

	/*
	 *  割込み要因の割込み優先度を求め(モデル上の)割込み優先度マスクをセット
	 *  する．またその際，ハンドラ実行前の(モデル上の)割込み優先度マスクを
	 *  保存する．
	 */
	ldw   r2, PCB_p_inh_iipm_table(r8) /* 受け付けた割込みの割込み優先度マスク */ 
	add   r3, r4, r2                   /* を取得 */
	ldbu  r5, 0(r3)
	ldbu  r2, PCB_iipm(r8)          /* 割込み発生前の割込み優先度マスクを */ 
	Push  r2                        /* スタックに保存                     */
	stb   r5, PCB_iipm(r8)          /* (モデル上の)割込み優先度マスクをセット */

	/*
	 *  ienableを受け付けた割込みの割込み優先度に対応するマスクビットの
	 *  否定値にセットする．
	 */
	slli  r3, r5, 2                 /* 割込み優先度マスクを4倍してオフセットを生成    */
	ldw   r2, PCB_p_iipm_mask_table(r8)   /* 割込み優先度マスクに対応した割込み禁止パターンを取得 */ 
	add   r2, r2, r3
	ldw   r5, 0(r2)
	ldw   r9, PCB_idf(r8)           /* 各割込みの割込み要求禁止フラグの状態を取得     */
	nor   r9, r9, r5                /* 両者のORの否定を生成                           */
	wrctl ienable, r9               /* 指定した割り込みのみ許可                       */

	/*
	 *  割込みハンドラのアドレスを読み込む
	 */
	slli  r3, r4, 2                 /* 割込み番号を4倍してオフセットを生成  */
	ldw   r2, PCB_p_inh_table(r8)
	add   r2, r2, r3
	ldw   r5, 0(r2)

	/*
	 *  割込み許可
	 */
	rdctl r3, status
	ori   r3, r3, STATUS_PIE
	wrctl status, r3

#ifdef LOG_INH_ENTER
	Push  r4           /* 割込み番号を保存 */
	Push  r5           /* ハンドラアドレスを保存 */
	call log_inh_enter 
	Pop   r5           /* ハンドラアドレスを復帰 */
	ldw   r4, 0(sp)    /*  割込み番号を復帰 */
#endif /* LOG_INH_ENTER */

	/* 
	 *  割込みハンドラ呼び出し 
	 */
	callr r5

#ifdef LOG_INH_LEAVE
	Pop  r4           /* 割込み番号を復帰 */     
	call log_inh_leave
#endif /* LOG_INH_LEAVE */

	/*
	 *  割込み禁止
	 */
	rdctl r3, status
	movi  r4, ~STATUS_PIE
	and   r3, r3, r4
	wrctl status, r3

	/*
	 *  スピンロック取得状態なら解除する
	 */
	my_pcb r8, r2              /* 引数 */
	call  force_unlock_spin

/*
 *  割込みハンドラ/CPU例外ハンドラ出口処理
 *
 *  ret_intは，割込みハンドラから戻った直後に実行するルーチンで，
 *  割込みハンドラ終了後，ターゲット依存の処理を実行した後，
 *  カーネル管理の割込みを禁止，スタックを割込み前のスタックにした
 *  状態で呼び出される．
 */
	.global ret_int
	.global ret_exc
ret_int:
ret_exc:
	my_pcb r8, r2              /* pcbの読み込み */

	/*
	 *  (モデル上の)割込み優先度マスクを元に戻す
	 */
	Pop   r3                   /* 割込み前の(モデル上の)割込み優先度マスクを取得 */
	stb   r3, PCB_iipm(r8)     /* (モデル上の)割込み優先度マスクをセット */
	slli  r3, r3, 2            /* 割込み優先度マスクを4倍してオフセットを生成    */
	ldw   r2, PCB_p_iipm_mask_table(r8) /* 割込み優先度マスクに対応した割込み禁止パターンを取得 */ 
	add   r2, r2, r3
	ldw   r5, 0(r2)
	ldw   r9, PCB_idf(r8)      /* 各割込みの割込み要求禁止フラグの状態を取得     */
	nor   r9, r9, r5           /* 両者のORの否定を生成                           */
	wrctl ienable, r9          /* 指定した割り込みのみ許可                       */

	/* 
	 *  割込み・例外のネスト回数のデクリメント
	 */
	ldw   r3, PCB_except_nest_count(r8)
	addi  r3, r3, -1
	stw   r3, PCB_except_nest_count(r8)

	bltu  zero, r3, ret_int_1    /* ネスト回数が1以上なら戻る */

	ldw   r2, 0(sp)              /* スタックポインタを戻す    */
	mov   sp, r2
	my_pcb r8, r2
	ldw   r3, PCB_reqflg(r8)     /* reqflgのチェック          */
	beq   r3, zero, ret_int_1    /* false なら タスクに戻る   */    
	br    ret_int_2              /* true なら ret_int_2 に    */

ret_int_1:
	ldw   et,   0(sp)  /* レジスタの復帰 */
	wrctl estatus, et
	ldw   at,   4(sp)
	ldw   r2,   8(sp)
	ldw   r3,  12(sp)
	ldw   r4,  16(sp)
	ldw   r5,  20(sp)
	ldw   r6,  24(sp)
	ldw   r7,  28(sp)
	ldw   r8,  32(sp)
	ldw   r9,  36(sp)
	ldw   r10, 40(sp)
	ldw   r11, 44(sp)
	ldw   r12, 48(sp)
	ldw   r13, 52(sp)
	ldw   r14, 56(sp)
	ldw   r15, 60(sp)
	ldw   fp,  64(sp)
	ldw   ra,  68(sp)
	ldw   ea,  72(sp)
	addi  sp, sp, 76        
	eret    

	/*
	 * 例外かトラップかを判断
	 */
_check_trap:
	/*
	 *  Trapか判定
	 */
//	ldw   et, -4(ea)               /* 例外を出した命令を取得 */
//	xorhi et, et, 0x003b           /*  上位16bit             */
//	xori  et, et, 0x683a           /*  下位16bit             */
//	beq   et, zero, trap_handler

	/*
	 *  例外エントリ
	 */
exception:
	/*
	 * コンテキストの保存
	 */
	addi  sp, sp, -76
	rdctl et,   estatus
	stw   et,    0(sp)
	stw   at,    4(sp)
	stw   r2,    8(sp)
	stw   r3,   12(sp)
	stw   r4,   16(sp)
	stw   r5,   20(sp)
	stw   r6,   24(sp)
	stw   r7,   28(sp)
	stw   r8,   32(sp)
	stw   r9,   36(sp)
	stw   r10,  40(sp)
	stw   r11,  44(sp)
	stw   r12,  48(sp)
	stw   r13,  52(sp)
	stw   r14,  56(sp)
	stw   r15,  60(sp)
	stw   fp,   64(sp)
	stw   ra,   68(sp)
	stw   ea,   72(sp)
	mov   r4,   sp     /* ハンドラの引数 */

	/*
	 * 多重割込みか判定
	 */
	my_pcb r8, r2
	ldw   r3, PCB_except_nest_count(r8)  /* ネスト回数のチェック */
	bltu  zero, r3,  nest_int_exc

	/*
	 * スタックポインタの入れ替え
	 */
	ldw   r2, PCB_istkpt(r8)
	addi  r2, r2, -4
	stw   sp, 0(r2)     /* スタックポインタの保存 */        
	mov   sp, r2        /* スタックポインタの入れ替え */

nest_int_exc:
	/* 
	 *  割込み・例外のネスト回数のインクリメント
	 */
	addi  r4, r3, 1
	stw   r4, PCB_except_nest_count(r8)

	/*
	 *  CPU例外ハンドラ番号の取得(4倍されている)
	 */
#ifdef USE_EXTRA_EXCEPTION
	rdctl r5, exception
#else 
	mov   r5, zero
#endif /* USE_EXTRA_EXCEPTION */
	srli  r6, r5, 2

	/*
	 *  割込み発生前の割込み優先度マスクをスタックに保存
	 */
	ldbu  r2, PCB_iipm(r8)
	Push  r2

	/*
	 *  locspnidを保存
	 */
	my_pcb r7, r4
	ldw    r4, PCB_locspnid(r7)
	Push   r4

	/*
	 *  例外フレームの作成            オフセット
	 *    例外番号                        0
	 *    ESTATUS(et)                     4
	 *    割込み・例外ネストカウント(r3)  8
	 *    戻りアドレス                   12  
	 *    割込み優先度マスク(iipm)       16
	 */
	addi  sp,  sp, -20
	stw   r6,  0(sp)
	stw   et,  4(sp)
	stw   r3,  8(sp)
	stw   ea, 12(sp)
	stw   r2, 16(sp)

	/*
	 *  例外フレームの先頭番地をCPU例外ハンドラの引数に設定（r4）
	 */
	mov   r4, sp

	/*
	 *  例外ハンドラアドレスの取得
	 */
	ldw   r2, PCB_p_exch_table(r8)
	add   r2, r2, r5
	ldw   r5, 0(r2)

	/*
	 *  例外発生時CPUロック状態でなければCPUロック解除状態とする
	 */
	rdctl r3, estatus
	andi  r3, r3, STATUS_PIE
	beq   r3, zero, call_exc_handler
	rdctl r3, status
	ori   r3, r3, STATUS_PIE
	wrctl status, r3

call_exc_handler:
#ifdef LOG_EXC_ENTER
	Push  r6         /* CPU例外ハンドラ番号保存       */
	Push  r5         /* CPU例外ハンドラアドレス保存   */
	Push  r4         /* CPU例外フレームの先頭番地保存 */
	mov   r4, r6     /* CPU例外ハンドラ番号を引数に   */
	call  log_exc_enter
	Pop   r4         /* CPU例外フレームの先頭番地復帰 */
	Pop   r5         /* CPU例外ハンドラアドレス復帰   */
#endif /* LOG_EXC_ENTER */

	/* 
	 *  CPU例外ハンドラ呼び出し 
	 */
	callr r5

#ifdef LOG_EXC_LEAVE
	Pop   r4         /* CPU例外ハンドラ番号復帰        */
	call  log_exc_leave
#endif /* LOG_EXC_LEAVE */

	/*
	 *  割込み禁止
	 */
	rdctl r3, status
	movi  r4, ~STATUS_PIE
	and   r3, r3, r4
	wrctl status, r3

	/*
	 *  例外フレームを捨てる
	 */
	addi  sp,  sp, 20

	/*
	 *  CPU例外発生時にスピンロックを取得していれば(!=0)解除は試みない
	 */
	Pop   r4
	bltu  zero, r4, ret_exc

	/*
	 *  CPU例外発生時にスピンロックを取得していない場合
	 */
	my_pcb r8, r2           /* 引数 */
	call  force_unlock_spin /* スピンロックを取得していれば解除する */
	br    ret_exc

	/*
	 *  Trapハンドラ
	 *  特権モードとしてリターンする
	 */
trap_handler:
	rdctl   et, estatus
	andi    et, et, 0
	ori     et, et, 1
	wrctl   estatus, et
	eret  

	/*
	 *  割込みハンドラ/CPU例外ハンドラ出口処理1
	 * 
	 *  ここでは，戻り先がタスクであり，スタックは，タスクスタックの上
	 *  にスクラッチレジスタのみが保存された状態になっている．また，
	 *  プロセッサは，スーパーバイザーモード・カーネル管理の割込みを禁止
	 *  した状態となっている．
	 *  PCBへのポインタがr8に入っている．
	 */  
ret_int_2:
	stw zero, PCB_reqflg(r8)       /* reqflg を false に */

	/*
	 *  CPUロック状態に移行する．
	 *
	 *  この時点でCPUロック状態とするのは，dispatcherへ分岐する時と，
	 *  call_texrtnを呼び出す時に，CPUロック状態になっている必要がある
	 *  ためである．
	 *
	 *  Nios2では，カーネル管理外の割込みを設けないため，ここでは，
	 *  特に何も行わない．
	 */

	/*
	 *  dspflgがfalseである場合と，p_runtskとp_schedtskが同じ場合には，
	 *  ディスパッチを行わない．このチェックが必要なのは，タスク例外処
	 *  理ルーチンの呼出しが必要な場合に，ディスパッチが必要なくても，
	 *  reqflgをtrueにするためである．
	 */
	ldw   r4,   PCB_p_runtsk(r8)       /* r4 <- runtsk      */
	ldw   r6,   PCB_dspflg(r8)         /* r6 <- dspflg      */
	beq   r6,   zero, ret_int_3        /* dspflg が false なら ret_int_3 へ */
	ldw   r5,   PCB_p_schedtsk(r8)     /* r5 <- p_schedtsk  */
	beq   r4,   r5, ret_int_3          /* runtsk と schedtskが同じならret_int_3へ */
	addi  sp,   sp, -32                /* 残りのレジスタを保存 */    
	stw   r16,  0(sp)
	stw   r17,  4(sp)
	stw   r18,  8(sp)
	stw   r19, 12(sp)
	stw   r20, 16(sp)
	stw   r21, 20(sp)
	stw   r22, 24(sp)
	stw   r23, 28(sp)
	stw   sp,  TCB_sp(r4)         /* タスクスタックをTCBに保存 */
	movhi r2,  %hiadj(ret_int_r)  /* 実行開始番地を保存        */
	addi  r2,  r2, %lo(ret_int_r)
	stw   r2,  TCB_pc(r4)         /* 実行再開番地をTCBに保存   */
	br    dispatcher

	/*
	 *  割込み・例外でコンテキスト保存した場合の復帰ルーチン
	 */
ret_int_r:
	ldw   r16,  0(sp)       /* レジスタを復帰 */
	ldw   r17,  4(sp)
	ldw   r18,  8(sp)
	ldw   r19, 12(sp)
	ldw   r20, 16(sp)
	ldw   r21, 20(sp)
	ldw   r22, 24(sp)
	ldw   r23, 28(sp)
	addi  sp, sp, 32    

ret_int_3:
	/*
	 * タスク例外ルーチンの起動
	 * ret_int_r は dispatcher から呼び出されるため，
	 * tcb のアドレスは r4 に入っている
	 */
	ldb  r5, TCB_enatex(r4)       /* r5 <- enatex */
#if TCB_enatex_mask > 0xffff
	andhi r6, r5, %hi(TCB_enatex_mask)
#else
	andi r6, r5, %lo(TCB_enatex_mask)
#endif /* TCB_enatex_mask > 0xffff */
	beq  r6, zero, ret_int_4      /* enatex が false ならリターン */
	ldw  r7, TCB_texptn(r4)       /* r7 <- texptn, texptnが0でなければ */
	beq  zero, r7, ret_int_4      
	call call_texrtn              /* タスク例外ルーチンの呼び出し */

ret_int_4:
	ldw   et,   0(sp)    /* レジスタを復帰 */
	wrctl estatus, et
	ldw   at,   4(sp)
	ldw   r2,   8(sp)
	ldw   r3,  12(sp)
	ldw   r4,  16(sp)
	ldw   r5,  20(sp)
	ldw   r6,  24(sp)
	ldw   r7,  28(sp)
	ldw   r8,  32(sp)
	ldw   r9,  36(sp)
	ldw   r10, 40(sp)
	ldw   r11, 44(sp)
	ldw   r12, 48(sp)
	ldw   r13, 52(sp)
	ldw   r14, 56(sp)
	ldw   r15, 60(sp)
	ldw   fp,  64(sp)
	ldw   ra,  68(sp)
	ldw   ea,  72(sp)
	addi  sp, sp, 76        
	eret 

/*
 *  ディスパッチ要求用のプロセッサ間割込みのバイパス処理
 *
 *  共通部で用意されている ipi_handler を呼び出さずにディスパッチャを
 *  呼び出す．最も外側のルーチンでない場合は，reqflgをセットする．
 *  自PCBのアドレスがr8に入っていることを想定している．
 */    
#ifdef USE_IPI_DIS_HANDER_BYPASS
ret_int_ipi_dispatch:
    ldw   r4, PCB_prc_int_base(r8)
    stw   zero, 0(r4)
    movi  r5, 1
    ldw   r3, %gprel(ext_ker_reqflg)(gp)
    beq   r5, r3, call_ext_ker
    ldw   r2, PCB_except_nest_count(r8)   /* 戻り先がタスクコンテキストなら   */
    beq   zero, r2, ret_int_2            /* ret_int_2 へ                     */
    stw   r5, PCB_reqflg(r8)             /* reqflg を true へ                */
    br    ret_int_1                      /* 戻り先が非タスクコンテキストなら */
call_ext_ker:
    call  ext_ker
#endif /* USE_IPI_DIS_HANDER_BYPASS */

/*
 * タスクディスパッチャ
 */
	.global dispatch
	.align  2
dispatch:
	/*
	 *  このルーチンは，タスクコンテキスト・CPUロック状態・ディスパッチ
	 *  許可状態・（モデル上の）割込み優先度マスク全解除状態で呼び出さ
	 *  れる．
	 */
	addi  sp, sp, -40                /* レジスタを保存 */    
	stw   r16,  0(sp)
	stw   r17,  4(sp)
	stw   r18,  8(sp)
	stw   r19, 12(sp)
	stw   r20, 16(sp)
	stw   r21, 20(sp)
	stw   r22, 24(sp)
	stw   r23, 28(sp)
	stw   fp,  32(sp)
	stw   ra,  36(sp)
	my_pcb r8, r2
	ldw   r4, PCB_p_runtsk(r8)      /* r4 <- runtsk              */
	stw   sp,  TCB_sp(r4)           /* タスクスタックをTCBに保存 */
	movhi r5,  %hiadj(dispatch_r)   /* 実行開始番地を保存        */
	addi  r5,  r5, %lo(dispatch_r)
	stw   r5,  TCB_pc(r4)           /* 実行再開番地をTCBに保存   */
	br    dispatcher

dispatch_r:
	ldw   r16,  0(sp)               /* レジスタを復帰 */
	ldw   r17,  4(sp)
	ldw   r18,  8(sp)
	ldw   r19, 12(sp)
	ldw   r20, 16(sp)
	ldw   r21, 20(sp)
	ldw   r22, 24(sp)
	ldw   r23, 28(sp)
	ldw   fp,  32(sp)     

	/*
	 *  タスク例外処理ルーチンの起動
	 *  dispatch_r は dispatcher から呼び出されるため，
	 *  tcb のアドレスは r4 に入っている
	 */
	ldb  r5, TCB_enatex(r4)       /* r5 <- enatex */
#if TCB_enatex_mask > 0xffff
	andhi r6, r5, %hi(TCB_enatex_mask)
#else
	andi r6, r5, %lo(TCB_enatex_mask)
#endif /* TCB_enatex_mask > 0xffff */
	beq  r6, zero, dispatch_r_1   /* enatex が false ならリターン */
	ldw  r7, TCB_texptn(r4)       /* r7 <- texptn, texptnが0でなければ */
	beq  r7, zero, dispatch_r_1    
	call dispatch_call_texrtn     /* タスク例外ルーチンの呼び出し */

dispatch_r_1:
	ldw   ra, 36(sp)              /* 残りのレジスタを復帰 */    
	addi  sp, sp, 40                
	ret


/*
 *  ディスパッチャの動作開始
 */
    .globl start_dispatch
start_dispatch:
	/*
	 *  このルーチンは，カーネル起動時に，すべての割込みを禁止した状態
	 * （割込みロック状態と同等）で呼び出される．また，割込みモード（非
	 *  タスクコンテキストと同等）で呼び出されることを想定している．
	 *
	 *  dispatcherは，CPUロック状態，（モデル上の）割込み優先度マスク全
	 *  解除状態，例外（割込み/CPU例外）のネスト回数0で呼び出す．     
	 *  target_initializeでは，（モデル上の）割込み優先度マスク全解除
	 *  とし，カーネル管理外の割込みであるFIQを許可することで，
	 *  CPUロック状態・（モデル上の）割込み優先度マスク全解除状態になる．
	 *  また，task_initializeでdisdspをfalseに初期化しているため，ディ
	 *  スパッチ許可状態になっている．
	 */ 
	/* 例外（割込み/CPU例外）のネスト回数をクリア */
	my_pcb  r8, r2
	stw   zero, PCB_except_nest_count(r8)

/*
 *  現在のコンテキストを捨ててディスパッチ
 */
    .globl exit_and_dispatch
exit_and_dispatch:
	/* ディスパッチャ本体（dispatcher）へ */  
	my_pcb  r8, r2    /* my_pcb を読み込む */

/*
 *  ディスパッチャ本体
 */
dispatcher:
	/*
	 *  このルーチンは，タスクコンテキスト・CPUロック状態・ディスパッチ
	 *  許可状態・（モデル上の）割込み優先度マスク全解除状態で呼び出さ
	 *  れる．実行再開番地へもこの状態のまま分岐する．
	 *  さらに，my_pcb のポインタをr8に入れた状態で呼び出す．
	 */
#ifdef LOG_DSP_ENTER
	ldw  r4, PCB_p_runtsk(r8)
	Push r8
	call log_dsp_enter
	Pop  r8
#endif /* LOG_DSP_ENTER */

dispatcher_0:
	ldw  r4, PCB_p_schedtsk(r8)  /* r4 <- schedtsk              */
	stw  r4, PCB_p_runtsk(r8)    /* schedtsk を runtskに        */
	beq  r4, zero, dispatcher_1  /* schedtskがあるか            */
	ldw  sp, TCB_sp(r4)          /* TCBからタスクスタックを復帰 */

#ifdef LOG_DSP_LEAVE
	Push r4
	call log_dsp_leave
	Pop  r4
#endif /* LOG_DSP_LEAVE */

	ldw   r5,   TCB_pc(r4)           /* TCBから実行再開番地を復帰   */
	jmp   r5

dispatcher_1:
	/*
	 * CPUロック状態の解除と，非タスクコンテキスト実行状態への
	 * 準備をする
	 */
	my_pcb r2, r3
	ldw   sp, PCB_istkpt(r2)
	movi  r5, 1
	stw   r5, PCB_except_nest_count(r2) /* except_nest_count を1に    */ 
dispatcher_2:
	/*
	 *  割込みを許可し，非タスクコンテキスト実行状態とし割込みを待つ．
	 *
	 *  ここで非タスクコンテキスト実行状態に切り換えるのは，ここで発生
	 *  する割込み処理にどのスタックを使うかという問題の解決と，割込み
	 *  ハンドラ内でのタスクディスパッチの防止という2つの意味がある．
	 *
	 *  プロセッサを割込み待ちに移行させる処理と，割込み許可とは，不可
	 *  分に行なう必要がある．
	 *  これを不可分に行なわない場合，割込みを許可した直後に割込
	 *  みが入り，その中でタスクが実行可能状態になると，実行すべきタス
	 *  クがあるにもかかわらずプロセッサが割込み待ちになってしまう．
	 *
	 *  割込み待ちの間は，p_runtskをNULL（＝0）に設定しなければならな
	 *  い．このように設定しないと，割込みハンドラからiget_tidを呼び出
	 *  した際の動作が仕様に合致しなくなる．
	 *
	 */
	rdctl r3, status          /* 割込み許可 */
	ori   r3, r3, STATUS_PIE
	wrctl status, r3
	nop
	nop
	nop
	nop
	rdctl r3, status          /* 割込み禁止 */
	movi  r4, ~STATUS_PIE
	and   r3, r3, r4
	wrctl status, r3
	ldw   r6, PCB_reqflg(r8)                  /* r6 <- reqflg             */
	beq   r6, zero, dispatcher_2              /* reqflg が FALSE なら     */
	stw   zero, PCB_except_nest_count(r2)     /* except_nest_countをクリア */ 
	stw   zero, PCB_reqflg(r8)                /* reqflg を FALSE に       */ 
	br    dispatcher_0

/*
 *  カーネルの終了処理の呼出し
 *
 *  モードとスタックを非タスクコンテキスト用に切り替え．
 */
    .globl call_exit_kernel
call_exit_kernel:
	my_pcb r8, r2             /* pcbの読み込み */
	ldw    sp, PCB_istkpt(r8)
	call  exit_kernel         

/*
 *  自タスクのマイグレーション
 *  第一引数(r4)はマイグレーション先のプロセッサのIDが入っているため，
 *  破壊しないようにする．
 */
	.text
	.global dispatch_and_migrate
	.align 2
dispatch_and_migrate:
	addi  sp, sp, -40                /* レジスタを保存 */    
	stw   r16,  0(sp)
	stw   r17,  4(sp)
	stw   r18,  8(sp)
	stw   r19, 12(sp)
	stw   r20, 16(sp)
	stw   r21, 20(sp)
	stw   r22, 24(sp)
	stw   r23, 28(sp)
	stw   fp,  32(sp)
	stw   ra,  36(sp)
	my_pcb r8, r2                  /* r8 <- pcb                 */
	ldw   r6, PCB_p_runtsk(r8)     /* r6 <- runtsk              */
	stw   sp,  TCB_sp(r6)          /* タスクスタックをTCBに保存 */
	movhi r5,  %hiadj(dispatch_r)  /* 実行開始番地を保存        */
	addi  r5,  r5, %lo(dispatch_r)
	stw   r5,  TCB_pc(r6)          /* 実行再開番地をTCBに保存   */
	my_istkpt  sp, r2              /* 非タスクコンテキスト用スタックへ切り替え */
	jmpi  migrate_self

/*
 *  現在のコンテキストを捨てマイグレーション
 *  第一引数(r4)はマイグレーション先のプロセッサのIDが入っているため，
 *  破壊しないようにする．
 */
	.text
	.global exit_and_migrate
	.align 2
exit_and_migrate:
	my_istkpt  sp, r2             /* 非タスクコンテキスト用スタックへ切り替え */
	jmpi  exit_and_migrate_self   /* 自タスクのexitとマイグレーションルチンへ */

/*
 *  タスク開始時処理
 *
 *  dispatcherから呼び出されるため，TCBのアドレスは r4 に入っている
 */
	.text
	.global start_r
	.align 2
start_r:
	rdctl r3, status              /* 割込み許可 */
	ori   r3, r3, STATUS_PIE
	wrctl status, r3
	ldw   r3, TCB_p_tinib(r4)     /* r3  <- p_runtsk->p_tinib     */
	ldw   r4, TINIB_exinf(r3)     /* r4  <- 引数（exinf）         */
	ldw   r2, TINIB_task(r3)      /* r2 <- タスクの実行番地       */
	movhi ra, %hiadj(ext_tsk)
	addi  ra, ra, %lo(ext_tsk)
	jmp   r2              /* タスクの実行開始 */


/*
 * 微少時間待ち
 */
	.global sil_dly_nse
sil_dly_nse:
	addi   r4, r4, -SIL_DLY_TIM1
	bgt    r4, zero, sil_dly_nse_1
	ret
sil_dly_nse_1:
	addi   r4, r4, -SIL_DLY_TIM2
	bgt    r4, zero, sil_dly_nse_1
	ret
