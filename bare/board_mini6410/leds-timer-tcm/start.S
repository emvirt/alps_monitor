/*
 *  TOPPERS/SafeG Dual-OS monitor
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Safety Gate Dual-OS monitor
 *
 *  Copyright (C) 2009-2011 by Embedded and Real-Time Systems Laboratory
 *     Graduate School of Information Science, Nagoya University, JAPAN
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
 */
#define CPSR_IRQ_BIT  (0x80)
#define CPSR_FIQ_BIT  (0x40)
#define CPSR_SVC      (0x13)
#define CPSR_FIQ      (0x11)

    .section .initialization,"x"
    .align 5
    .global initialization
initialization:
    msr  cpsr, #(CPSR_SVC|CPSR_FIQ_BIT|CPSR_IRQ_BIT)
    ldr  sp, =__init_stack_end
    mov  fp, #0
    mov  r7, #0
    b    tcm_init
    b    .

    .section .vector,"a"
    .align 5
    .global vector_table
vector_table:
    ldr pc, reset_vector
    ldr pc, exc_vector
    ldr pc, exc_vector
    ldr pc, exc_vector
    ldr pc, exc_vector
    ldr pc, exc_vector
    ldr pc, exc_vector
    ldr pc, fiq_vector

reset_vector:
    .long   secure_start
fiq_vector:
    .long   fiq_handler
exc_vector:
    .long   exc_handler

    .text
    .align  5
    .global secure_start
secure_start:
    msr  cpsr, #(CPSR_SVC|CPSR_FIQ_BIT|CPSR_IRQ_BIT)
    ldr  sp, =__stack_end
    mov  fp, #0
    mov  r7, #0
    bl   main

    .text
    .align 5
    .global fiq_handler
fiq_handler:
    msr   cpsr, #(CPSR_SVC|CPSR_FIQ_BIT|CPSR_IRQ_BIT)
    stmfd sp!, {r0-r3,ip,lr,pc}

    msr   cpsr, #(CPSR_FIQ|CPSR_FIQ_BIT|CPSR_IRQ_BIT)
    sub   r0, lr, #4
    mrs   r1, spsr

    msr   cpsr, #(CPSR_SVC|CPSR_FIQ_BIT|CPSR_IRQ_BIT)
    str   r0, [sp,#0x18]
    stmfd sp!,{r1}

    bl fiq_c_handler

    ldmfd sp!,{r1}
    msr   spsr, r1
    ldmfd sp!,{r0-r3,ip,lr,pc}^

    .text
    .align 5
    .global exc_handler
exc_handler:
    msr   cpsr, #(CPSR_SVC|CPSR_FIQ_BIT|CPSR_IRQ_BIT)
    bl    exc_c_handler
    b     .
