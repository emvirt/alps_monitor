/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: nios2_system.h 451 2009-10-19 08:50:43Z ertl-honda $
 */

/*
 *  NIOS2システムの定義
 *
 *  ペリフェラルのアドレスや割込み番号を定義する
 */

#ifndef TOPPERS_NIOS2_SYSTEM_H
#define TOPPERS_NIOS2_SYSTEM_H

#ifdef TOPPERS_NIOS2_DEV_1S40

/*
 *  プロセッサ数
 */
#ifndef TNUM_PRCID
#define TNUM_PRCID  2
#endif  /* TNUM_PRCID */

/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */
#define TIMER_CLOCK     50000U

/*
 *  微少時間待ちのための定義（本来はSILのターゲット依存部）
 */
#define SIL_DLY_TIM1    230
#define SIL_DLY_TIM2     70

/*
 *  非タスクコンテキスト用のスタックサイズの定義
 */
#define DEFAULT_PRC1_ISTKSZ    (0x1000U)  /* 4Kbyte */
#define DEFAULT_PRC2_ISTKSZ    (0x1000U)  /* 4Kbyte */
#define DEFAULT_PRC3_ISTKSZ    (0x1000U)  /* 4Kbyte */
#define DEFAULT_PRC4_ISTKSZ    (0x1000U)  /* 4Kbyte */

/*
 *  ネイティブスピン方式の場合のスピンロックの最大数
 */
#define TMAX_NATIVE_SPN   4

/*
 *  データセクションの初期化を行なわない
 */
#define NIOS2_OMIT_DATA_INIT

/*
 *  Interrupt Vector Instruction 命令を持つ
 */
#define NIOS2_USE_INT_VEC_INST       /* Interrupt Vector Instruction を使う */
#define NIOS2_INT_VEC_INST_NO    0   /* 命令番号                            */

/*
 *  キャッシュサイズ
 */
#define NIOS2_ICACHE_SIZE       0x1000  /* 4kbyte */
#define NIOS2_ICACHE_LINE_SIZE  0x0010  /* 32byte */
#define NIOS2_DCACHE_SIZE       0x0000
#define NIOS2_DCACHE_LINE_SIZE 

/*
 *  ペリフェラルのベースアドレス
 */
#define CPU_1_PERI_BASEADDR  0x01000000
#define CPU_2_PERI_BASEADDR  0x02000000
#define CPU_3_PERI_BASEADDR  0x03000000
#define CPU_4_PERI_BASEADDR  0x04000000

/*
 *  Interval Timer
 */
#define SYS_CLK_TIMER_1_BASE (0x00000840 + CPU_1_PERI_BASEADDR)
#define SYS_CLK_TIMER_2_BASE (0x00000840 + CPU_2_PERI_BASEADDR)
#define SYS_CLK_TIMER_3_BASE (0x00000840 + CPU_3_PERI_BASEADDR)
#define SYS_CLK_TIMER_4_BASE (0x00000840 + CPU_4_PERI_BASEADDR)

#define SYS_CLK_TIMER_1_INT   0U
#define SYS_CLK_TIMER_2_INT   0U
#define SYS_CLK_TIMER_3_INT   0U
#define SYS_CLK_TIMER_4_INT   0U

/*
 *  JTAG UART
 */
#define JTAG_UART_1_BASE (0x00000860 + CPU_1_PERI_BASEADDR)
#define JTAG_UART_2_BASE (0x00000860 + CPU_2_PERI_BASEADDR)
#define JTAG_UART_3_BASE (0x00000860 + CPU_3_PERI_BASEADDR)
#define JTAG_UART_4_BASE (0x00000860 + CPU_4_PERI_BASEADDR)

#define JTAG_UART_1_INT   1U
#define JTAG_UART_2_INT   1U
#define JTAG_UART_3_INT   1U
#define JTAG_UART_4_INT   1U

/*
 *  Mutex
 */ 
#define TSK_MUTEX_1_BASE   (0x00000810 + CPU_1_PERI_BASEADDR)
#define OBJ_MUTEX_1_BASE   (0x00000820 + CPU_1_PERI_BASEADDR)
#define TSK_MUTEX_2_BASE   (0x00000810 + CPU_2_PERI_BASEADDR)
#define OBJ_MUTEX_2_BASE   (0x00000820 + CPU_2_PERI_BASEADDR)
#define TSK_MUTEX_3_BASE   (0x00000810 + CPU_3_PERI_BASEADDR)
#define OBJ_MUTEX_3_BASE   (0x00000820 + CPU_3_PERI_BASEADDR)
#define TSK_MUTEX_4_BASE   (0x00000810 + CPU_4_PERI_BASEADDR)
#define OBJ_MUTEX_4_BASE   (0x00000820 + CPU_4_PERI_BASEADDR)

/*
 * プロセッサ間割り込みHW
 */
#define PRC_INT_1_BASE  (0x00000800 + CPU_1_PERI_BASEADDR)
#define PRC_INT_2_BASE  (0x00000800 + CPU_2_PERI_BASEADDR)
#define PRC_INT_3_BASE  (0x00000800 + CPU_3_PERI_BASEADDR)
#define PRC_INT_4_BASE  (0x00000800 + CPU_4_PERI_BASEADDR)

#define PRC_INT_INT     2U

/*
 * SYSVER
 */
#define SYSVER_REG1 0x07110200
#define SYSVER_REG2 0x07110204
#define SYSVER_REG3 0x07110208
#define SYSVER_REG4 0x0711020C
#define SYSVER_REG5 0x07110210
#define SYSVER_REG6 0x07110214
#define SYSVER_REG7 0x07110218
#define SYSVER_REG8 0x0711021C

/*
 *  Performance Couter
 */
#define PERF_COUNTER_BASE 0x07110100

/*
 *  Performance Counter のクロック
 */
#define PERF_COUNTER_CLOCK     50U

#elif defined(TOPPERS_NIOS2_DEV_3C25)

/*
 *  プロセッサ数
 */
#ifndef TNUM_PRCID
#define TNUM_PRCID  2
#endif /* TNUM_PRCID */

/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */
#define TIMER_CLOCK     50000U

/*
 *  微少時間待ちのための定義（本来はSILのターゲット依存部）
 */
#define SIL_DLY_TIM1    230
#define SIL_DLY_TIM2     70

/*
 *  ネイティブスピン方式の場合のスピンロックの最大数
 */
#define TMAX_NATIVE_SPN   4

/*
 *  非タスクコンテキスト用のスタックサイズの定義
 */
#define DEFAULT_PRC1_ISTKSZ    (0x1000U)  /* 4Kbyte */
#define DEFAULT_PRC2_ISTKSZ    (0x1000U)  /* 4Kbyte */
#define DEFAULT_PRC3_ISTKSZ    (0x1000U)  /* 4Kbyte */
#define DEFAULT_PRC4_ISTKSZ    (0x1000U)  /* 4Kbyte */

/*
 *  データセクションの初期化を行なわない
 */
#define NIOS2_OMIT_DATA_INIT

/*
 *  Interrupt Vector Instruction 命令を持つ
 */
#define NIOS2_USE_INT_VEC_INST       /* Interrupt Vector Instruction を使う */
#define NIOS2_INT_VEC_INST_NO    0   /* 命令番号                            */

/*
 *  キャッシュサイズ
 */
#define NIOS2_ICACHE_SIZE       0x1000  /* 4kbyte */
#define NIOS2_ICACHE_LINE_SIZE  0x0010  /* 32byte */
#define NIOS2_DCACHE_SIZE       0x0000
#define NIOS2_DCACHE_LINE_SIZE 

/*
 *  ペリフェラルのベースアドレス
 */
#define CPU_1_PERI_BASEADDR  0x02000000
#define CPU_2_PERI_BASEADDR  0x03000000

/*
 *  Interval Timer
 */
#define SYS_CLK_TIMER_1_BASE (0x00000800 + CPU_1_PERI_BASEADDR)
#define SYS_CLK_TIMER_2_BASE (0x00000800 + CPU_2_PERI_BASEADDR)

#define SYS_CLK_TIMER_1_INT   0U
#define SYS_CLK_TIMER_2_INT   0U

/*
 *  JTAG UART
 */
#define JTAG_UART_1_BASE (0x00000820 + CPU_1_PERI_BASEADDR)
#define JTAG_UART_2_BASE (0x00000820 + CPU_2_PERI_BASEADDR)

#define JTAG_UART_1_INT   1U
#define JTAG_UART_2_INT   1U

/*
 *  Mutex
 */
#define TSK_MUTEX_1_BASE   (0x00000830 + CPU_1_PERI_BASEADDR)
#define OBJ_MUTEX_1_BASE   (0x00000840 + CPU_1_PERI_BASEADDR)
#define TSK_MUTEX_2_BASE   (0x00000830 + CPU_2_PERI_BASEADDR)
#define OBJ_MUTEX_2_BASE   (0x00000840 + CPU_2_PERI_BASEADDR)

/*
 * プロセッサ間割り込みHW
 */
#define PRC_INT_1_BASE  (0x00000850 + CPU_1_PERI_BASEADDR)
#define PRC_INT_2_BASE  (0x00000850 + CPU_2_PERI_BASEADDR)

#define PRC_INT_1_INT   2U
#define PRC_INT_2_INT   2U

/*
 * SYSVER
 */
#define SYSVER_REG1 0x08000200
#define SYSVER_REG2 0x08000204
#define SYSVER_REG3 0x08000208
#define SYSVER_REG4 0x0800020C
#define SYSVER_REG5 0x08000210
#define SYSVER_REG6 0x08000214
#define SYSVER_REG7 0x08000218
#define SYSVER_REG8 0x0800021C

#elif defined(TOPPERS_NIOS2_DEV_3C120)

/*
 *  プロセッサ数
 */
#ifndef TNUM_PRCID
#define TNUM_PRCID  2
#endif /* TNUM_PRCID */

/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */
#define TIMER_CLOCK     50000U

/*
 *  微少時間待ちのための定義（本来はSILのターゲット依存部）
 */
#define SIL_DLY_TIM1    230
#define SIL_DLY_TIM2     70

/*
 *  非タスクコンテキスト用のスタックサイズの定義
 */
#define DEFAULT_PRC1_ISTKSZ    (0x1000U)  /* 4Kbyte */
#define DEFAULT_PRC2_ISTKSZ    (0x1000U)  /* 4Kbyte */
#define DEFAULT_PRC3_ISTKSZ    (0x1000U)  /* 4Kbyte */
#define DEFAULT_PRC4_ISTKSZ    (0x1000U)  /* 4Kbyte */

/*
 *  ネイティブスピン方式の場合のスピンロックの最大数
 */
#define TMAX_NATIVE_SPN   4

/*
 *  データセクションの初期化を行なわない
 */
#define NIOS2_OMIT_DATA_INIT

/*
 *  Interrupt Vector Instruction 命令を持つ
 */
#define NIOS2_USE_INT_VEC_INST       /* Interrupt Vector Instruction を使う */
#define NIOS2_INT_VEC_INST_NO    0   /* 命令番号                            */

/*
 *  キャッシュサイズ
 */
#define NIOS2_ICACHE_SIZE       0x1000  /* 4kbyte */
#define NIOS2_ICACHE_LINE_SIZE  0x0010  /* 32byte */
#define NIOS2_DCACHE_SIZE       0x0000
#define NIOS2_DCACHE_LINE_SIZE 

/*
 *  ペリフェラルのベースアドレス
 */
#define CPU_1_PERI_BASEADDR  0x01000000
#define CPU_2_PERI_BASEADDR  0x02000000
#define CPU_3_PERI_BASEADDR  0x03000000
#define CPU_4_PERI_BASEADDR  0x04000000

/*
 *  Interval Timer
 */
#define SYS_CLK_TIMER_1_BASE (0x00000800 + CPU_1_PERI_BASEADDR)
#define SYS_CLK_TIMER_2_BASE (0x00000800 + CPU_2_PERI_BASEADDR)
#define SYS_CLK_TIMER_3_BASE (0x00000800 + CPU_3_PERI_BASEADDR)
#define SYS_CLK_TIMER_4_BASE (0x00000800 + CPU_4_PERI_BASEADDR)

#define SYS_CLK_TIMER_1_INT   0U
#define SYS_CLK_TIMER_2_INT   0U
#define SYS_CLK_TIMER_3_INT   0U
#define SYS_CLK_TIMER_4_INT   0U

/*
 *  JTAG UART
 */
#define JTAG_UART_1_BASE (0x00000900 + CPU_1_PERI_BASEADDR)
#define JTAG_UART_2_BASE (0x00000900 + CPU_2_PERI_BASEADDR)
#define JTAG_UART_3_BASE (0x00000900 + CPU_3_PERI_BASEADDR)
#define JTAG_UART_4_BASE (0x00000900 + CPU_4_PERI_BASEADDR)

#define JTAG_UART_1_INT   1U
#define JTAG_UART_2_INT   1U
#define JTAG_UART_3_INT   1U
#define JTAG_UART_4_INT   1U

/*
 *  Mutex
 */
#define TSK_MUTEX_1_BASE   (0x00000a00 + CPU_1_PERI_BASEADDR)
#define OBJ_MUTEX_1_BASE   (0x00000b00 + CPU_1_PERI_BASEADDR)
#define TSK_MUTEX_2_BASE   (0x00000a00 + CPU_2_PERI_BASEADDR)
#define OBJ_MUTEX_2_BASE   (0x00000b00 + CPU_2_PERI_BASEADDR)
#define TSK_MUTEX_3_BASE   (0x00000a00 + CPU_3_PERI_BASEADDR)
#define OBJ_MUTEX_3_BASE   (0x00000b00 + CPU_3_PERI_BASEADDR)
#define TSK_MUTEX_4_BASE   (0x00000a00 + CPU_4_PERI_BASEADDR)
#define OBJ_MUTEX_4_BASE   (0x00000b00 + CPU_4_PERI_BASEADDR)

/*
 * プロセッサ間割り込みHW
 */
#define PRC_INT_1_BASE  (0x00000c00 + CPU_1_PERI_BASEADDR)
#define PRC_INT_2_BASE  (0x00000c00 + CPU_2_PERI_BASEADDR)
#define PRC_INT_3_BASE  (0x00000c00 + CPU_3_PERI_BASEADDR)
#define PRC_INT_4_BASE  (0x00000c00 + CPU_4_PERI_BASEADDR)

#define PRC_INT_1_INT   2U
#define PRC_INT_2_INT   2U
#define PRC_INT_3_INT   2U
#define PRC_INT_4_INT   2U

/*
 * SYSVER
 */
#define SYSVER_REG1 0x0c021100
#define SYSVER_REG2 0x0c021104
#define SYSVER_REG3 0x0c021108
#define SYSVER_REG4 0x0c02110c
#define SYSVER_REG5 0x0c021110
#define SYSVER_REG6 0x0c021114
#define SYSVER_REG7 0x0c021118
#define SYSVER_REG8 0x0c02111c

#endif /* TOPPERS_NIOS2_DEV_3C120 */

#endif /* TOPPERS_NIOS2_SYSTEM_H */
