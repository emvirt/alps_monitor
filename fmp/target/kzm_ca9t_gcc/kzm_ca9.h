/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
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
 *  @(#) $Id: kzm_ca9.h 599 2010-05-07 08:00:38Z ertl-honda $
 */

/*
 * KZM_CA9用ドライバ定義
 */

#ifndef TOPPERS_KZM_CA9_H
#define TOPPERS_KZM_CA9_H

#include <sil.h>

/*
 * コアクロック（400MHz）
 */
#define KZM_CA9_CORE_CLK  400
        
/*
 *  MPCore Private Memory Regin Base Address
 */
#define MPCORE_PMR_BASE  0x1e000000

/*
 *  MPCore内蔵のタイマを1MHzで動作させるためのプリスケーラーの設定値
 */
#define MPCORE_TM_PS_1MHZ    199   /* コアクロック400Mhz */

/*
 * ベースアドレス
 */
#define SYS_BASE   0x10000000

/*
 *  割込み番号
 */
#define DIC_IRQNO_UART2   64
#define DIC_IRQNO_TM_SP804_0 80
#define DIC_IRQNO_TM_SP804_1 81
#define DIC_IRQNO_UART0   84

/*
 * TestChip内蔵タイマ関連(SP804)
 */

/* 
 * ベースアドレス
 */
#define TIMER_BASE_REG  (SYS_BASE + 0xe4000)


#ifndef TOPPERS_MACRO_ONLY

/*
 *  TestChip内蔵タイマの初期化
 */
extern void timer_sp804_init(void);

/*
 *  TestChip内蔵タイマの現在値の読出し
 */
Inline uint32_t
timer_sp804_get_current(void)
{
	return (0xffffffff - sil_rew_mem((void *)(TIMER_BASE_REG + 0x004))); // 0x004 == TIMER_SP804_1VALUE
}

#endif /* TOPPERS_MACRO_ONLY */

/*
 * UART関連の定義
 * レジスタの定義は
 * 	・arm_primecell_uart.h(UART0)
 * 	・tl16c554a.h(UART2)
 * で行っている
 */
 
/* 
 * ベースアドレス
 */
#define UART0_BASE  (SYS_BASE + 0xe3000)
#define SMC0_CS2_BASE	0x48000000
#define UART2_CSA_BASE (SMC0_CS2_BASE + 0x00000000) 
#define UART2_CSB_BASE (SMC0_CS2_BASE + 0x00800000) 
#define UART2_CSC_BASE (SMC0_CS2_BASE + 0x01000000) 
#define UART2_CSD_BASE (SMC0_CS2_BASE + 0x01800000) 


/*
 * コプロへのアクセスマクロ
 */
//#define CP15_SET_VBAR(x)    Asm("mcr p15, 0, %0, c12, c0, 0"::"r"(x))




/*
 *  BPS設定(38400bps)
 */ 
#define UART_IBRD_38400   0x0c
#define UART_FBRD_38400   0x00

#ifndef TOPPERS_MACRO_ONLY
/*
 *  UARTからのポーリング出力
 */
extern void kzm_ca9_putc(char_t c);

/*
 * ポーリング出力のための初期化
 */
extern void kzm_ca9_uart_init(void);

/*
 *  開発環境依存の処理(T.B.D)
 */
Inline void
kzm_ca9_exit(void){
	while(1);
}

/*
 *  トレースログに関する定義
 *  SCUを用いる．
 */
#define TRACE_HW_INIT()   scu_counter_init()
#define TRACE_GET_TIM()   ((scu_counter_get_current() * 1000) / CORE_CLK)

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_KZM_CA9_H */
