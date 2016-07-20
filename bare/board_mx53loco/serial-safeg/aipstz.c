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

#define AIPSTZ_BASE           0x53F00000
#define AISPTZ_MPROT0         (AIPSTZ_BASE + 0x0000)
#define AISPTZ_MPROT1         (AIPSTZ_BASE + 0x0004)
#define AISPTZ_OPACR0         (AIPSTZ_BASE + 0x0040)
#define AISPTZ_OPACR1         (AIPSTZ_BASE + 0x0044)
#define AISPTZ_OPACR2         (AIPSTZ_BASE + 0x0048)
#define AISPTZ_OPACR3         (AIPSTZ_BASE + 0x004C)
#define AISPTZ_OPACR4         (AIPSTZ_BASE + 0x0050)

void aipstz_init(void)
{
	unsigned int csu_reg;

	// Initialize the control registers (u-boot already does this job)
	__REG(AISPTZ_MPROT0) = 0x77777777;
	__REG(AISPTZ_MPROT1) = 0x77777777;
	__REG(AISPTZ_OPACR0) = 0x44444444;
	__REG(AISPTZ_OPACR1) = 0x44444444;
	__REG(AISPTZ_OPACR2) = 0x44444444;
	__REG(AISPTZ_OPACR3) = 0x44444444;
	__REG(AISPTZ_OPACR4) = 0x44000000;

	// set everything to non-secure experimentally
	// CSU values (use x S:0x63F9C000 in ds-5):
	// - 0x63F9C000 ~ 0x63F9C07C: 32 registers 0x00330033
	//   33 is probably the sec_config_X (for each X peripheral, so we can
	//   set 64 peripherals.
	//
	// - To discover the UART1 one, we use "x N:0x53FBC080" until it does not
	//   generate a precise abort (we set FF instead of 33)
	for(csu_reg = 0x63F9C000; csu_reg < 0x63F9C080; csu_reg = csu_reg + 4)
		__REG(csu_reg) = 0x00FF00FF;
}
