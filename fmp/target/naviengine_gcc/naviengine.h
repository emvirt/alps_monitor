/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel  
 * 
 *  Copyright (C) 2006-2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: naviengine.h 91 2009-01-25 16:50:43Z ertl-honda $
 */

/*
 *  NaviEngine用ドライバ定義
 */

#ifndef TOPPERS_NAVIENGINE_H
#define TOPPERS_NAVIENGINE_H

#include <sil.h>

/*
 * コアクロック（400MHz）
 */
#define NAVIENGINE_CORE_CLK  400

/*
 *  MPCore Private Memory Regin Base Address
 */
#define MPCORE_PMR_BASE  0xc0000000

/*
 *  MPCore Status
 */
#define NAVIENGINE_MPCORE_STATUS  0x18037D08

/*
 *  MPCore内蔵のタイマを1MHzで動作させるためのプリスケーラーの設定値
 */
#define MPCORE_TM_PS_1MHZ    199   /* コアクロック399Mhz */

/*
 *  UARTのレジスタ
 */
#define UART0_BASE		0x18034000
#define UART1_BASE		0x18034400
#define UART2_BASE		0x18034800
#define UART3_BASE		0x18034C80
#define UART4_BASE		0x180350c0

#define	UART_RBR_REG		(0x0 )
#define	UART_THR_REG		(0x0 )
#define	UART_DLL_REG		(0x00)
#define	UART_IER_REG		(0x04)
#define	UART_DLM_REG		(0x04)
#define	UART_IIR_REG		(0x08)
#define	UART_LCR_REG		(0x0c)
#define	UART_MCR_REG		(0x10)
#define	UART_LSR_REG		(0x14)
#define	UART_MSR_REG		(0x18)
#define	UART_SCR_REG		(0x1c)
#define	UART_FCR_REG		(0x20)

/*
 * 割込み番号
 */
#define DIC_IRQNO_UART0   86
#define DIC_IRQNO_UART1   87
#define DIC_IRQNO_UART2   88
#define DIC_IRQNO_UART3   89

/*
 *  設定内容
 */
#define UART0_LCR_VAL 0x03
#define UART0_DLM_VAL 0x00
#define UART0_DLL_VAL 0xd8

#define UART1_LCR_VAL 0x03
#define UART1_DLM_VAL 0x00
#define UART1_DLL_VAL 0xd8

#define UART2_LCR_VAL 0x03
#define UART2_DLM_VAL 0x00
#define UART2_DLL_VAL 0xd8

#define UART3_LCR_VAL 0x03
#define UART3_DLM_VAL 0x00
#define UART3_DLL_VAL 0xd8

#ifndef TOPPERS_MACRO_ONLY
/*
 *  UARTからのポーリング出力
 */
extern void naviengine_uart_putc(char_t c);

/*
 * ポーリング出力のための初期化
 */
extern void naviengine_uart_init(void);
#endif /* TOPPERS_MACRO_ONLY */

#define GIC_TMIN_INTNO    0U
#define GIC_TMAX_INTNO  127U
#define GIC_TNUM_INT    128U

#ifndef TOPPERS_MACRO_ONLY

/*
 *  開発環境依存の処理(T.B.D)
 */
Inline void
naviengine_exit(void)
{
	while(1);
}

/*
 *  トレースログに関する定義
 *  SCUを用いる． 
 */
#define TRACE_HW_INIT()   scu_counter_init()
#define TRACE_GET_TIM()   ((scu_counter_get_current() * 1000) / NAVIENGINE_CORE_CLK)

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_NAVIENGINE_H */
