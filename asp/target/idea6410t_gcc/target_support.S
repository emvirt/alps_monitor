/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_support.S 267 2007-07-13 09:19:44Z honda $
 */

/*
 *  ターゲット依存モジュール アセンブリ言語部（ARM用）
 */

#define  TOPPERS_MACRO_ONLY
#define UINT_C(val)		(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)	(val)		/* ulong_t型の定数を作るマクロ */
#include "kernel_impl.h"

#define __ASSEMBLY__
#include "../../../safeg/safeg_measures.h" /* TODO: improve this */

/*
 *  低レベルのターゲットシステム依存の初期化
 *
 *  スタートアップモジュールの中で，メモリ初期化の前に呼び出される．          
 */

    .text
    .align 2
    .global hardware_init_hook
hardware_init_hook:
    mov   pc, lr

    .text
    .align 2
    .global fiq_handler
fiq_handler:
    /* 
     *  割込み前のモード(スーパーバイザーモード)へ
     *  移行し，コンテキストを保存する
     */
    msr   cpsr, #(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET) 
    stmfd sp!, {r0-r1}  /* r0, r1しか潰さない */

    /*
     * spsrと戻り番地を取得するためにFIQモードへ
     */
    msr   cpsr, #(CPSR_FIQ|CPSR_CPULOCK|CPSR_ALWAYS_SET)
	mov   r0, lr
    mrs   r1, spsr

    /*
     *  FIQモードから持ってきた lr_fiq, spsr_fiq を lr_irq, spsr_irq に設定する
     */
    msr   cpsr, #(CPSR_IRQ|CPSR_CPULOCK|CPSR_ALWAYS_SET) 
	mov   lr, r0
	msr   spsr_cxsf, r1

	msr   cpsr, #(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET)
	ldmfd sp!, {r0-r1}


/*
 * 割込みハンドラ
 *
 * FIQ 例外ベクタから呼び出される
 */
    .text
    .align 2
    .global irq_handler
irq_handler:
    /* 
     * IRQモードで実行される
     */
    /* 
     *  割込み前のモード(スーパーバイザーモード)へ
     *  移行し，コンテキストを保存する
     */
    msr   cpsr, #(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET) 
    stmfd sp!, {r0-r3,ip,lr,pc} /* pcはダミー */

#ifdef SUPPORT_OVR
    stmfd sp!, {r0-r3, ip, lr}   /* save scratch registers */
    bl ovrtimer_stop             /* オーバランタイマの停止 */
    ldmfd sp!, {r0-r3, ip, lr}   /* restore scratch registers */
#endif /* SUPPORT_OVR */

#if ASP_MEASURE_IRQ_LAT == 1
    perfmon_start r0
#endif

    /*
     * spsrと戻り番地を取得するためにIRQモードへ
     */
    msr   cpsr, #(CPSR_IRQ|CPSR_CPULOCK|CPSR_ALWAYS_SET)
    sub   r0, lr, #4
    mrs   r1, spsr

    /*
     *  割込みハンドラ実行時のモード（スーパーバイザーモード）に
     */
    msr   cpsr, #(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET) 
    str   r0, [sp,#0x18] /* 戻り番地をスタックに */
    stmfd sp!,{r1}       /* spsrをスタックに保存 */
    mov   lr, sp         /* この時点のスタックを復帰のため取得 */

    /*
     *  多重割込みか判定
     */
    ldr   r2, =excpt_nest_count /* 例外・割込みネスト回数を取得 */
    ldr   r3, [r2]
    add   r0,r3,#1              /* 例外・割込みネスト回数を更新 */
    str   r0, [r2]
    cmp   r3, #0    
    bne   irq_handler_1

    /* 
     * 一段目の例外（割込み）ならスタックを変更する
     */
    ldr  r0, =_kernel_istkpt
    ldr  sp, [r0]

irq_handler_1:
    stmfd sp!, {lr}     /* 復帰用のスタックポインタの保存 */

    /*
     *  割り込み要因の判定
     */
    ldr   r1, =TZIC0_STATUS
    ldr   r2, [r1]
	mov   r0, #31
	clz   r3, r2
	sub   r3, r0, r3

    /*
     *  割込み要因の割込み優先度を求め(モデル上の)割込み優先度マスクをセット
     *  する．またその際，ハンドラ実行前の(モデル上の)割込み優先度マスクを
     *  保存する．
     */
    ldr   r0, =inh_ipm_tbl      /* 割込み優先度を取得 */
    ldr   r1, [r0, r3, lsl #2]  /* r1<-割込み優先度   */
    ldr   r0, =ipm              /* 割込み発生前の割込み優先度マスクをスタックに保存 */
    ldr   r2, [r0]
    stmfd sp!,{r2}
    str   r1, [r0]              /* 新たな割込み優先度マスクを設定 */
    ldr   r0, =ipm_mask_tbl     /* (モデル上)の割込み優先度のを実現するための */
    rsb   r1, r1, #0
    ldr   r2, [r0, r1, lsl #3]  /* 割込み要求禁止フラグを取得 */
    ldr   r0, =idf              /* 各割込みの割込み要求禁止フラグの状態を取得 */
    ldr   r1, [r0]
    /*
     * 各割込みの割込み要求禁止フラグの状態と(モデル上)の割込み優先度のを実現する
     * ための割込み要求禁止フラグの状態のORをとり，それの否定を求めることにより，
     * IRCの割込み許可レジスタへの設定値を生成し設定する．
     */
    ldr   r0, =TZIC0_LOCK        /* ロック解除 */
    ldr   lr, =TZIC_UNLOCK_CODE
    str   lr, [r0]
    ldr   r0, =TZIC0_CLEAR       /* いったん全割込みの割込み要求をマスクする */
    mvn   lr, #0
    str   lr, [r0]
    ldr   r0, =TZIC0_ENABLE      /* マスク指定されていない割込みの許可 */
    orr   r1, r1, r2            /* 設定値を生成 */
    mvn   r1, r1
    str   r1, [r0]
    ldr   r0, =TZIC0_LOCK        /* ロック */
    ldr   lr, =TZIC_LOCK_CODE
    str   lr, [r0]

    /*
     *  割込みハンドラの起動番地を取得
     */
    ldr   r0, =inh_tbl         /* 割込みハンドラテーブルの読み出し  */
    ldr   r0, [r0,r3,lsl #2]   /* r0<-割込みハンドラ            */

    /* 
     * 割り込み許可 
     */         
    msr   cpsr, #(CPSR_SVC|CPSR_ALWAYS_SET)

#if SAFEG_MEASURE_FIQ_FIQ == 1
#if SAFEG_MEASURE_GPIO_ENA == 1
    gpio_clear lr, r1, GPIO_0_DATA_MASK_0  /* clear port 0 */
#endif /* SAFEG_MEASURE_GPIO_ENA */
#if SAFEG_MEASURE_PERF_ENA == 1
    perfmon_end lr, r1, r2
#endif /* SAFEG_MEASURE_PERF_ENA */
#endif /* SAFEG_MEASURE_FIQ_FIQ */

#if SAFEG_MEASURE_FIQ_IRQ == 1
#if SAFEG_MEASURE_GPIO_ENA == 1
    cmp r3, #18 /* TODO: do this general for other irqs */
    bne not_serial_irq
    gpio_set lr, r1, GPIO_0_DATA_MASK_1  /* set port 1 */
not_serial_irq:
#endif /* SAFEG_MEASURE_GPIO_ENA */
#endif /* SAFEG_MEASURE_FIQ_IRQ */

#if ASP_MEASURE_IRQ_LAT == 1
    perfmon_end lr, r1, r2
#endif

    /* 
     * 割込みハンドラの呼び出し
     */        
    ldr   lr,  =target_ret_int
    mov   pc,  r0


/*
 * ターゲット依存の例外入口処理
 */
    .text
    .global target_exc_handler
target_exc_handler:    
    /*
     *  例外実行のモード（スーパーバイザーモード）に
     *  割込みロック状態，CPUロック状態はCPU例外発生時の
     *  状態を継承する．
     */
    msr   cpsr, #(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET) 
    str   r0, [sp,#0x18] /* 戻り番地をスタックに */
    stmfd sp!,{r1}       /* spsrをスタックに保存 */    
    mov   lr, sp         /* この時点のスタックを復帰のため取得 */

#ifdef SUPPORT_OVR
    stmfd sp!, {r0-r3, ip, lr}   /* save scratch registers */
    bl ovrtimer_stop             /* オーバランタイマの停止 */
    ldmfd sp!, {r0-r3, ip, lr}   /* restore scratch registers */
#endif /* SUPPORT_OVR */

    ldr   r0, =ipm       /* 割込み発生前の割込み優先度マスクをスタックに保存 */
    ldr   r1, [r0]
    stmfd sp!,{r1}

    /* 
     * コンテキスト判定のため，excpt_nest_count をスタックに保存．
     * スタックに保存せず，現在のexcpt_nest_countを-1すると取得できるが，
     * スタックに積んでおいた方がデバッグ等が行いやすいので，スタックに
     * 保存する．
     */
    ldr   r0, =excpt_nest_count
    ldr   r1, [r0]
    stmfd sp!,{r1}

    mov   r3,  sp         /* 例外フレーム番地を保存 */

    /*
     *  多重割り込みか判定
     */
    ldr   r0, =excpt_nest_count
    ldr   r1, [r0]
    add   r1,r1,#1
    str   r1, [r0]
    cmp   r1, #1
    bne   target_exc_handler_1    

    /* 
     * 一段目の実行ならスタックを変更する
     */
    ldr  r0, =_kernel_istkpt
    ldr  sp, [r0]

target_exc_handler_1:    
    stmfd sp!, {lr}     /* 復帰用のスタックポインタの保存 */

    /* 
     * 割込み発生前の割込み優先度マスクをスタックに保存 
     * 割込みハンドラと出口ルーチンを共有するために保存
     */
    ldr   r0, =ipm
    ldr   r1, [r0]
    stmfd sp!,{r1}

    /*
     *  CPU例外ハンドラの起動番地を取得
     */
    ldr   r0, =exch_tbl        /* 割込みハンドラテーブルの読み出し */
    ldr   r1, [r0,r2,lsl #2]   /* r1<-例外ハンドラ                 */

    /* 
     * 割り込み許可 
     * CPUロック状態，割込みロック状態は継承する
     */         
    ldr   r0, [lr]
    and   r0, r0, #(CPSR_CPULOCK)
    orr   r0, r0, #(CPSR_SVC|CPSR_ALWAYS_SET)
    msr   cpsr,r0

    /* 
     * CPU例外ハンドラの呼び出し
     *
     * 例外フレームの先頭を引数として渡す
     */        
    mov   r0,  r3
    ldr   lr,  =target_exc_int
    mov   pc,  r1



/*
 * ターゲット依存の例外・割込みの出口処理
 *
 */
    .global target_ret_int        
    .global target_exc_int        
target_ret_int:        
target_exc_int:
    /*
     * カーネル管理の割込みを禁止する
     */
    msr   cpsr, #(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET)  

    /*
     * 割込み優先度マスクを元に戻す
     */
    ldmfd sp!,{r1}              /* 元の割込み優先度マスクを取得 */
    ldr   r0, =ipm              /* 割込み優先度マスクを復帰     */
    str   r1, [r0]
    ldr   r0, =ipm_mask_tbl     /* (モデル上)の割込み優先度のを実現するための */
    rsb   r1, r1, #0
    ldr   r2, [r0, r1, lsl #2]  /* 割込み要求禁止フラグを取得 */    
    ldr   r0, =idf              /* 各割込みの割込み要求禁止フラグの状態を取得 */
    ldr   r1, [r0]
    /*
     * 各割込みの割込み要求禁止フラグの状態と(モデル上)の割込み優先度のを実現する
     * ための割込み要求禁止フラグの状態のORをとり，それの否定を求めることにより，
     * IRCの割込み許可レジスタへの設定値を生成し設定する．
     */
    ldr   r0, =TZIC0_LOCK        /* ロック解除 */
    ldr   lr, =TZIC_UNLOCK_CODE 
    str   lr, [r0]
    ldr   r0, =TZIC0_CLEAR       /* いったん全割込みの割込み要求をマスクする */
    mvn   lr, #0                
    str   lr, [r0]                                
    ldr   r0, =TZIC0_ENABLE      /* マスク指定されていない割込みの許可 */
    orr   r1, r1, r2            /* 設定値を生成 */    
    mvn   r1, r1                
    str   r1, [r0]
    ldr   r0, =TZIC0_LOCK        /* ロック */
    ldr   lr, =TZIC_LOCK_CODE 
    str   lr, [r0]

    /*
     * スタックポインタの復帰
     */
    ldmfd sp!,{r2}              /* 元のスタックポインタを取得 */
    mov   sp, r2
    
    /*
     * 後の処理はARM依存部で実行
     */            
    b     ret_int
