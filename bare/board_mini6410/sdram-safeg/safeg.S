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

#define CPSR_MODE_USER      0b10000
#define CPSR_MODE_FIQ       0b10001
#define CPSR_MODE_IRQ       0b10010
#define CPSR_MODE_SVC       0b10011
#define CPSR_MODE_ABORT     0b10111
#define CPSR_MODE_UNDEF     0b11011
#define CPSR_MODE_SYSTEM    0b11111
#define CPSR_MODE_MONITOR   0b10110

#define CPSR_BIT_7_I        0b00000000000000000000000010000000
#define CPSR_BIT_6_F        0b00000000000000000000000001000000

#define SCR_NS  0b100101
#define SCR_S   0b110000

#define NORMAL_CPSR_INIT_VALUE 0b110010011
#define NSACR_SETTING 0b1110011111111111111

#define BankedOffset    (N_IRQ_SPSR - NtContextBase)

.macro CP15_SCR_WRITE treg
  mcr p15, 0, \treg, c1, c1, 0
.endm

.macro CP15_NSACR_WRITE treg
  mcr p15, 0, \treg, c1, c1, 2
.endm

.macro CP15_MVBAR_WRITE treg
  mcr p15, 0, \treg, c12, c0, 1
.endm

.macro CP15_Flush_BTAC treg
  mov \treg, #0
  mcr p15, 0, \treg, c7, c5, 6
.endm

.macro SaveState
    stmdb   sp, {r0-lr}^
    add     r6, sp, #BankedOffset
    cps     #CPSR_MODE_IRQ
    mrs     r12, spsr
    stmia   r6!, {r12-lr}
    cps     #CPSR_MODE_SVC
    mrs     r12, spsr
    stmia   r6!, {r12-lr}
    cps     #CPSR_MODE_ABORT
    mrs     r12, spsr
    stmia   r6!, {r12-lr}
    cps     #CPSR_MODE_UNDEF
    mrs     r12, spsr
    stmia   r6!, {r12-lr}
    cps     #CPSR_MODE_FIQ
    mrs     r7, spsr
    stmia   r6, {r7-lr}
    cps     #CPSR_MODE_MONITOR
.endm

.macro LoadState
    add     r6, sp, #BankedOffset
    cps     #CPSR_MODE_IRQ
    ldmia   r6!, {r12-lr}
    msr     spsr_fsxc, r12
    cps     #CPSR_MODE_SVC
    ldmia   r6!, {r12-lr}
    msr     spsr_fsxc, r12
    cps     #CPSR_MODE_ABORT
    ldmia   r6!, {r12-lr}
    msr     spsr_fsxc, r12
    cps     #CPSR_MODE_UNDEF
    ldmia   r6!, {r12-lr}
    msr     spsr_fsxc, r12
    cps     #CPSR_MODE_FIQ
    ldmia   r6, {r7-lr}
    msr     spsr_fsxc, r7
    cps     #CPSR_MODE_MONITOR
    ldmdb   sp, {r0-lr}^
.endm

    .section .vector, "a"
    .align 5
    .globl MonitorVectorTableBase
MonitorVectorTableBase:
    ldr pc, safeg_reset_vector
    b   .
    ldr pc, safeg_smc_vector
    b   .
    b   .
    b   .
    b   .
    ldr pc, safeg_fiq_vector

    .global monitor_vector_ref_tbl
monitor_vector_ref_tbl:
safeg_reset_vector:
    .long   safeg_start
safeg_smc_vector:
    .long   safeg_smc_handler
safeg_fiq_vector:
    .long   safeg_fiq_handler

    .text
    .align 5
    .globl safeg_start
safeg_start:
    cps #CPSR_MODE_MONITOR
    mov r0, #SCR_S
    CP15_SCR_WRITE r0
    ldr r0, =MonitorVectorTableBase
    CP15_MVBAR_WRITE r0
    ldr r0, =NSACR_SETTING
    CP15_NSACR_WRITE r0
    ldr  r1, =__bss_start
    ldr  r2, =__bss_end
    cmp  r1,r2
    bhs  nobss
    mov  r0,#0
cleanbss:
    str  r0,[r1],#4
    cmp  r1, r2
    blo  cleanbss
nobss:
    ldr r0, =0x57C00000  /* non_secure_start */
    ldr r1, =secure_start
    ldr r2, =NtContextBase
    ldr r3, =NORMAL_CPSR_INIT_VALUE
    stmea r2, {r0, r3}
    cps #CPSR_MODE_SVC
    mov pc, r1

    .text
    .align  5
    .global safeg_smc_handler
safeg_smc_handler:
    ldr sp, =TContextBase
    srsia   #CPSR_MODE_MONITOR
    SaveState
    ldr sp, =NtContextBase
    LoadState
    mov lr, #SCR_NS
    CP15_SCR_WRITE lr
    CP15_Flush_BTAC lr
    rfeia   sp

    .text
    .align 5
    .global safeg_fiq_handler
safeg_fiq_handler:
    mov sp, #SCR_S
    CP15_SCR_WRITE sp
    sub lr, lr, #4
    ldr sp, =NtContextBase
    srsia #CPSR_MODE_MONITOR
    SaveState
    ldr sp, =TContextBase
    LoadState
    rfeia sp

    .bss
    .align 5
    .global NtContextSavedArea
    .global NtContextSavedAreaEnd
    .global TContextSavedArea
    .global TContextSavedAreaEnd
    .global NtContextBase
    .global TContextBase
NtContextSavedArea:
N_USR_R0       : .long 0x00
N_USR_R1       : .long 0x00
N_USR_R2       : .long 0x00
N_USR_R3       : .long 0x00
N_USR_R4       : .long 0x00
N_USR_R5       : .long 0x00
N_USR_R6       : .long 0x00
N_USR_R7       : .long 0x00
N_USR_R8       : .long 0x00
N_USR_R9       : .long 0x00
N_USR_R10      : .long 0x00
N_USR_R11      : .long 0x00
N_USR_R12      : .long 0x00
N_USR_SP       : .long 0x00
N_USR_LR       : .long 0x00

NtContextBase:
N_PC           : .long 0x00
N_CPSR         : .long 0x00
N_SCRATCH      : .long 0x00
N_IRQ_SPSR     : .long 0x00
N_IRQ_SP       : .long 0x00
N_IRQ_LR       : .long 0x00
N_SVC_SPSR     : .long 0x00
N_SVC_SP       : .long 0x00
N_SVC_LR       : .long 0x00
N_ABT_SPSR     : .long 0x00
N_ABT_SP       : .long 0x00
N_ABT_LR       : .long 0x00
N_UND_SPSR     : .long 0x00
N_UND_SP       : .long 0x00
N_UND_LR       : .long 0x00
N_FIQ_SPSR     : .long 0x00
N_FIQ_R8       : .long 0x00
N_FIQ_R9       : .long 0x00
N_FIQ_R10      : .long 0x00
N_FIQ_R11      : .long 0x00
N_FIQ_R12      : .long 0x00
N_FIQ_SP       : .long 0x00
N_FIQ_LR       : .long 0x00
NtContextSavedAreaEnd:

TContextSavedArea:
S_USR_R0       : .long 0x00
S_USR_R1       : .long 0x00
S_USR_R2       : .long 0x00
S_USR_R3       : .long 0x00
S_USR_R4       : .long 0x00
S_USR_R5       : .long 0x00
S_USR_R6       : .long 0x00
S_USR_R7       : .long 0x00
S_USR_R8       : .long 0x00
S_USR_R9       : .long 0x00
S_USR_R10      : .long 0x00
S_USR_R11      : .long 0x00
S_USR_R12      : .long 0x00
S_USR_SP       : .long 0x00
S_USR_LR       : .long 0x00

TContextBase:
S_PC           : .long 0x00
S_CPSR         : .long 0x00
S_SCRATCH      : .long 0x00
S_IRQ_SPSR     : .long 0x00
S_IRQ_SP       : .long 0x00
S_IRQ_LR       : .long 0x00
S_SVC_SPSR     : .long 0x00
S_SVC_SP       : .long 0x00
S_SVC_LR       : .long 0x00
S_ABT_SPSR     : .long 0x00
S_ABT_SP       : .long 0x00
S_ABT_LR       : .long 0x00
S_UND_SPSR     : .long 0x00
S_UND_SP       : .long 0x00
S_UND_LR       : .long 0x00
S_FIQ_SPSR     : .long 0x00
S_FIQ_R8       : .long 0x00
S_FIQ_R9       : .long 0x00
S_FIQ_R10      : .long 0x00
S_FIQ_R11      : .long 0x00
S_FIQ_R12      : .long 0x00
S_FIQ_SP       : .long 0x00
S_FIQ_LR       : .long 0x00
TContextSavedAreaEnd:
