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
typedef struct {
	volatile unsigned long	ULCON;
	volatile unsigned long	UCON;
	volatile unsigned long	UFCON;
	volatile unsigned long	UMCON;
	volatile unsigned long	UTRSTAT;
	volatile unsigned long	UERSTAT;
	volatile unsigned long	UFSTAT;
	volatile unsigned long	UMSTAT;
	volatile unsigned char	UTXH;
	volatile unsigned char	res1[3];
	volatile unsigned char	URXH;
	volatile unsigned char	res2[3];
	volatile unsigned long	UBRDIV;
	volatile unsigned long	UDIVSLOT;
} S3C64XX_UART;

void uart_init(void)
{
	volatile S3C64XX_UART *const uart =  (S3C64XX_UART *)0x7F005000;

	uart->UFCON = 0;   // FIFO disabled
	uart->UMCON = 0;   // RTS AFC disabled
	uart->ULCON = 0x3; // 8-N-1
	uart->UCON = 0xe45;  // polling mode - external clock 
	uart->UBRDIV = 34; // integer part of DIV_VAL => (66MHz / (115200 x 16)) ‐ 1 = 34.81
	uart->UDIVSLOT = 0x1FFF; // 0.81 * 16 = number of 1s in UDIVSLOT = 13
}

void uart_putc(char c)
{
	volatile S3C64XX_UART *const uart =  (S3C64XX_UART *)0x7F005000;

	while (!(uart->UTRSTAT & 0x2));
	uart->UTXH = c;
}
