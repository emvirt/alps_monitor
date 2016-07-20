/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2008 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: naviengine.c 447 2009-10-16 08:51:47Z ertl-honda $
 */

#include "kernel_impl.h"
#include "naviengine.h"

#ifdef G_SYSLOG

#define my_prc_uart_base	UART0_BASE

#else

/*
 *  低レベル出力でプロセッサ毎に使用するUARTのアドレス
 */
const uint32_t prc_uart_base_tbl[TNUM_PRCID] = {
	UART0_BASE,
#if TNUM_PRCID >= 2
	UART1_BASE,
#endif /* TNUM_PRCID >= 2 */
#if TNUM_PRCID >= 3
	UART2_BASE,  
#endif /* TNUM_PRCID >= 3 */
#if TNUM_PRCID >= 4
	UART3_BASE,
#endif /* TNUM_PRCID >= 4 */
};

/*
 *  自プロセッサが使用するUARTのアドレス
 */
#define my_prc_uart_base	(prc_uart_base_tbl[x_prc_index()])

#endif /* G_SYSLOG */


/*
 *  UARTからのポーリング出力
 */
void
naviengine_uart_putc(char_t c)
{
	/* バッファが空くのを待つ */
	while((sil_reb_mem((void *)(my_prc_uart_base + UART_LSR_REG)) & 0x20U)
          == 0x00U);
	/* 書き込み */
	sil_wrb_mem((void *)(my_prc_uart_base + UART_THR_REG), c);
	/* バッファが空くのを待つ */
	while((sil_reb_mem((void *)(my_prc_uart_base + UART_LSR_REG)) & 0x20U)
		  == 0x00U);
}


/*
 * ポーリング出力のための初期化
 */
void
naviengine_uart_init(void)
{
	sil_wrb_mem((void *)(my_prc_uart_base + UART_LCR_REG), 0x83);
	sil_wrb_mem((void *)(my_prc_uart_base + UART_DLL_REG), 0xD8);
	sil_wrb_mem((void *)(my_prc_uart_base + UART_DLM_REG), 0x00);
	sil_wrb_mem((void *)(my_prc_uart_base + UART_LCR_REG), 0x03);
	sil_wrb_mem((void *)(my_prc_uart_base + UART_IER_REG), 0x00);
	sil_wrb_mem((void *)(my_prc_uart_base + UART_FCR_REG), 0x00);
}
