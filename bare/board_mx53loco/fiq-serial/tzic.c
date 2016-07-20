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
#define __REG(x)	(*((volatile unsigned int *)(x)))

#define TZIC_NUM_IRQS           128
#define TZIC_BASE               0x0FFFC000
#define TZIC_INTCNTL            (TZIC_BASE + 0x0000)	/* control register */
#define TZIC_INTTYPE            (TZIC_BASE + 0x0004)	/* Controller type register */
#define TZIC_IMPID              (TZIC_BASE + 0x0008)	/* Distributor Implementer Identification Register */
#define TZIC_PRIOMASK           (TZIC_BASE + 0x000C)	/* Priority Mask Reg */
#define TZIC_SYNCCTRL           (TZIC_BASE + 0x0010)	/* Synchronizer Control register */
#define TZIC_DSMINT             (TZIC_BASE + 0x0014)	/* DSM interrupt Holdoffregister */
#define TZIC_INTSEC0            (TZIC_BASE + 0x0080)	/* interrupt security register 0 */
#define TZIC_ENSET0             (TZIC_BASE + 0x0100)	/* Enable Set Register 0 */
#define TZIC_ENCLEAR0           (TZIC_BASE + 0x0180)	/* Enable Clear Register 0 */
#define TZIC_SRCSET0            (TZIC_BASE + 0x0200)	/* Source Set Register 0 */
#define TZIC_SRCCLAR0           (TZIC_BASE + 0x0280)	/* Source Clear Register 0 */
#define TZIC_PRIORITY0          (TZIC_BASE + 0x0400)	/* Priority Register 0 */
#define TZIC_PND0               (TZIC_BASE + 0x0D00)	/* Pending Register 0 */
#define TZIC_HIPND0             (TZIC_BASE + 0x0D80)	/* High Priority Pending Register */
#define TZIC_WAKEUP0            (TZIC_BASE + 0x0E00)	/* Wakeup Config Register */
#define TZIC_SWINT              (TZIC_BASE + 0x0F00)	/* Software Interrupt Rigger Register */
#define TZIC_ID0                (TZIC_BASE + 0x0FD0)	/* Indentification Register 0 */

#define MX51_INT_UART1	31

void tzic_init(void)
{
	int i;

	// Enable Secure and Non-Secure interrupts
	i = __REG(TZIC_INTCNTL);
	__REG(TZIC_INTCNTL) = 0x80010001;
	i = __REG(TZIC_INTCNTL);

	// Set priority mask to the minimum (inverse order)
	__REG(TZIC_PRIOMASK) = 0xFF;
	i = __REG(TZIC_PRIOMASK);

	// Set for low latency
	__REG(TZIC_SYNCCTRL) = 0;
	i = __REG(TZIC_SYNCCTRL);

	// Set all interrupts as Secure
	for (i = 0; i < 4; i++) {
		__REG(TZIC_INTSEC0 + i * 4) = 0;
	}

	// Set all interrupts to the highest priority (inverse order)
	for (i = 0; i < 4; i++) {
		__REG(TZIC_PRIORITY0 + i * 4) = 0;
	}

	/* Disable all interrupts */
	for (i = 0; i < 32; i++) {
		__REG(TZIC_ENCLEAR0 + i * 4) = 0xFFFFFFFF;
	}

	/* Enable the UART interrupt */
	__REG(TZIC_ENSET0) = (1 << MX51_INT_UART1);
	i = __REG(TZIC_ENSET0);
}
