/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2010 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: chip_support.S 543 2010-02-26 01:37:54Z ertl-honda $
 */

/*
 *  ターゲット依存モジュール アセンブリ言語部（ARM用）
 */

#define  TOPPERS_MACRO_ONLY
#define UINT_C(val)		(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)	(val)		/* ulong_t型の定数を作るマクロ */
#include "kernel_impl.h"
#include "target_asm.inc"
#include "offset.h"

/*
 *  低レベルのターゲットシステム依存の初期化
 *
 *  スタートアップモジュールの中で，メモリ初期化の前に呼び出される．
 */
    .text
    .align 2
    .global hardware_init_hook
hardware_init_hook:
    bx  lr

/*
 * 割込みハンドラ
 */
    .text
    .align 2
    .global irq_handler
irq_handler:
    /*
     * IRQモードで実行される
     */
    sub   lr, lr, #4 /* 戻り先アドレス  */

    /* 
     * lr(戻り先) spsr(cpsr_svc)をスーパーバイザーモードのスタックへ
     * 保存．
     */
    srsdb #CPSR_SVC! 

    /*
     *  割込み前のモード(スーパーバイザーモード)へ
     *  移行し，コンテキストを保存する
     */
    msr   cpsr, #(CPSR_SVC|CPSR_CPULOCK); /*CPSR_IRQ_BIT)*/
    stmfd sp!, {r0-r3,ip,lr}

    /*
     *  PCBの取得
     */ 
    my_pcb r0, r3

#if TTYPE_KLOCK != G_KLOCK
    /*
     *  プロセッサ・細粒度ロックの場合はロックを取得しているかチェックして
     *  取得していればロックを解放する．
     */ 
release_lock:
    ldr   r3, [r0, #PCB_p_firstlock]  /* p_firstlock を取得 */
    cmp   r3, #0                      /* NULLなら終了       */
    beq   release_lock_end
    mov   r2, #0
    mcr   p15, 0, r2, c7, c10, 5      /* data_memory_barrier */
    str   r2, [r3]
    mcr   p15, 0, r2, c7, c10, 4      /* data_sync_barrier */
    sev
    str   r2, [r0, #PCB_p_firstlock]  /* p_firstlock を NULL へ */
    ldr   r3, [r0, #PCB_p_secondlock] /* p_secondlock を取得 */
    cmp   r3, #0                      /* NULLなら終了       */    
    mcr   p15, 0, r2, c7, c10, 5      /* data_memory_barrier */
    str   r2, [r3]
    mcr   p15, 0, r2, c7, c10, 4      /* data_sync_barrier */
    sev    
    str   r2, [r0, #PCB_p_firstlock]  /* p_secondlock を NULL へ */
release_lock_end:
#endif /* TTYPE_KLOCK != G_KLOCK */

    mov   lr, sp         /* この時点のスタックを復帰のため取得 */

    /*
     *  割り込み要因の判定．
     */
    ldr   r2, =(DIC_CPUIF_IAR)
    ldr   r1, [r2]
    mov   r1, r1, asl #22  /* 下位10ビット以外をクリア */
    mov   r1, r1, lsr #22

#ifdef USE_IPI_DIS_HANDER_BYPASS

#ifdef NONTRUST /* ノントラスト側の時 */
    cmp     r1, #DIC_IRQNO_IPI0
#else /* トラスト側の時 */
    cmp     r1, #DIC_IRQNO_IPI2
#endif /* NONTRUST */

    ldreq   r2,=DIC_CPUIF_EOI
    streq   r1,[r2]                /* EOIレジスタへの書き込み */
    beq     ret_int_ipi_dispatch
#endif /* USE_IPI_DIS_HANDER_BYPASS */

    /*
     *  多重割込みか判定
     */
    ldr   r3, [r0, #PCB_excpt_nest_count]  /* 例外・割込みネスト回数を取得 */
    add   r2, r3, #1                       /* 例外・割込みネスト回数を更新 */
    str   r2, [r0, #PCB_excpt_nest_count]
    cmp   r3, #0    
    bne   irq_handler_1

    /* 
     * 一段目の例外（割込み）ならスタックを変更する
     */
    ldr   sp, [r0, #PCB_istkpt]

irq_handler_1:
    stmfd sp!, {lr}     /* 復帰用のスタックポインタの保存 */

    /*
     *  割込みハンドラのアドレスを取得
     */
    ldr   r3, [r0, #PCB_p_inh_table] /* 割込みハンドラテーブルの読み込み */
    ldr   r0, [r3, r1, asl #2]       /* r0<-割込みハンドラ */

    /*
     *  割込み要因の割込み優先度を求め(モデル上の)割込み優先度マスクをセット
     *  する．またその際，ハンドラ実行前の(モデル上の)割込み優先度マスクを
     *  保存する．
     */
    ldr   lr, =(DIC_CPUIF_RII)
    ldr   r2, [lr]
    ldr   r3, =(DIC_CPUIF_PMR) /* 割込み発生前の割込み優先度マスクを取得 */
    ldr   lr, [r3]   
    str   r2, [r3]   /* 新しい割込み優先度マスクをセットする */
    stmfd sp!, {lr}  /* 割込み発生前の割込み優先度マスクをスタックに保存 */

    stmfd sp!,{r1}       /* inhno を保存  */

    /* 
     * 割り込み許可 
     */         
    msr   cpsr, #(CPSR_SVC|CPSR_ALWAYS_SET)

#ifdef LOG_INH_ENTER
    stmfd sp!,{r0}
    mov   r0, r1         /* inhno をパラメータに    */
    bl    log_inh_enter  /* log_inh_enterを呼び出す */
    ldmfd sp!,{r0}
#endif /* LOG_INH_ENTER */

    /* 
     * 割込みハンドラの呼び出し
     */        
    mov   lr,  pc
    bx    r0

target_ret_int:
    ldmfd sp!,{r0}       /* inhno を復帰  */

    ldr   r1,=DIC_CPUIF_EOI
    str   r0,[r1]                /* EOIレジスタへの書き込み */

#ifdef LOG_INH_LEAVE
    bl    log_inh_leave  /* log_inh_leaveを呼び出す */
#endif /* LOG_INH_LEAVE */

    /*
     * カーネル管理の割込みを禁止する
     */
    msr   cpsr, #(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET)

    /*
     * 割込み優先度マスクを元に戻す
     */
    ldmfd sp!,{r1}                       /* 元の割込み優先度マスクを取得 */
    ldr   r0, =(DIC_CPUIF_PMR)           /* DICにセット */
    str   r1, [r0]

    /*
     * スタックポインタの復帰
     */
    ldmfd sp!,{r2}              /* 元のスタックポインタを取得 */
    mov   sp, r2
    
    /*
     * 後の処理はARM依存部で実行
     */
    b     ret_int

/*
 * ターゲット依存の例外入口処理
 */
    .text
    .global target_exc_handler
target_exc_handler:
    /*
     *  例外フレームの作成
     */
    mov   lr, sp         /* この時点のスタックを復帰のため取得 */

    ldr   r0, =(DIC_CPUIF_PMR) /* 例外発生前の割込み優先度マスクを取得 */
    ldr   r1, [r0]
    asr   r1, r1, #DIC_CPUIF_PMR_OFFSET /* 外部表現に変更 */
    sub   r1, r1, #(IPM_BIT_MASK)
    stmfd sp!, {r1}  /* 例外発生前の割込み優先度マスクをスタックに保存 */

    /* 
     *  コンテキスト判定のため，excpt_nest_count をスタックに保存．
     *  スタックに保存せず，現在のexcpt_nest_countを-1すると取得できるが，
     *  スタックに積んでおいた方がデバッグ等が行いやすいので，スタックに
     *  保存する．
     */
    my_pcb r0, r1
    ldr   r1, [r0, #PCB_excpt_nest_count]
    stmfd sp!, {r1}

    mov   r3,  sp         /* 例外フレーム番地を保存 */

    /*
     *  多重割り込みか判定
     */
    add   r1, r1, #1
    str   r1, [r0, #PCB_excpt_nest_count]
    cmp   r1, #1
    bne   target_exc_handler_1

    /* 
     *  一段目の実行ならスタックを変更する
     */
    ldr   sp, [r0, #PCB_istkpt]

target_exc_handler_1:
    stmfd sp!, {lr}     /* 復帰用のスタックポインタの保存 */

    /*
     *  locspnidを保存
     */
    my_pcb r0, r1
    ldr    r1, [r0,#PCB_locspnid]
    stmfd  sp!,{r1}

    /* 
     * 割込み発生前の割込み優先度マスクをスタックに保存 
     * 割込みハンドラと出口ルーチンを共有するために保存
     */
    ldr   ip, =(DIC_CPUIF_PMR) /* 例外発生前の割込み優先度マスクを取得 */
    ldr   r1, [ip]
    stmfd sp!, {r1}  /* 例外発生前の割込み優先度マスクをスタックに保存 */

    /*
     *  CPU例外ハンドラの起動番地を取得
     */
    ldr   ip, [r0, #PCB_p_exch_table] /* 割込みハンドラテーブルの読み出し */
    ldr   r1, [ip, r2, lsl #2]        /* r1<-例外ハンドラ                 */

    stmfd sp!,{r2}      /* excno を保存 */

    /* 
     * 割り込み許可 
     * CPUロック状態，割込みロック状態は継承する
     */         
    ldr   r0, [lr, #(7*4)]
    and   r0, r0, #(CPSR_CPULOCK|CPSR_INTLOCK)
    orr   r0, r0, #(CPSR_SVC|CPSR_ALWAYS_SET)
    msr   cpsr,r0

#ifdef LOG_EXC_ENTER
    stmfd sp!,{r1, r3}
    mov   r0, r2         /* excno をパラメータに    */
    bl    log_exc_enter  /* log_exc_enterを呼び出す */
    ldmfd sp!,{r1, r3}
#endif /* LOG_EXC_ENTER */

    /* 
     * CPU例外ハンドラの呼び出し
     *
     * 例外フレームの先頭を引数として渡す
     */        
    mov   r0,  r3
    mov   lr,  pc
    bx    r1

target_ret_exc:
    ldmfd sp!,{r0}       /* excno を復帰  */
#ifdef LOG_EXC_LEAVE
    bl    log_exc_leave  /* log_exc_leaveを呼び出す */
#endif /* LOG_EXC_LEAVE */

    /*
     * カーネル管理の割込みを禁止する
     */
    msr   cpsr, #(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET)

    /*
     * 割込み優先度マスクを元に戻す
     */
    ldmfd sp!,{r1}                       /* 元の割込み優先度マスクを取得 */
    ldr   r0, =(DIC_CPUIF_PMR)           /* DICにセット */
    str   r1, [r0]

    /*
     * CPU例外発生時のlocspnidをr0へ
     */
    ldmfd sp!,{r0} 

    /*
     * スタックポインタの復帰
     */
    ldmfd sp!,{r2}              /* 元のスタックポインタを取得 */
    mov   sp, r2
    
    /*
     * 後の処理はARM依存部で実行
     */
    b     ret_exc
