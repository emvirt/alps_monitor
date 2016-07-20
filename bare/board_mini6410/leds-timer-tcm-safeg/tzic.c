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
#define TZIC0_BASE 0x71000000
#define TZIC1_BASE 0x71100000

#define TZIC0_STATUS      (TZIC0_BASE + 0x00)
#define TZIC0_RAWSTATUS   (TZIC0_BASE + 0x04)
#define TZIC0_INTSELECT   (TZIC0_BASE + 0x08)
#define TZIC0_ENABLE      (TZIC0_BASE + 0x0C)
#define TZIC0_CLEAR       (TZIC0_BASE + 0x10)
#define TZIC0_PROTECTION  (TZIC0_BASE + 0x18)
#define TZIC0_LOCK        (TZIC0_BASE + 0x1C)
#define TZIC0_LOCK_STATUS (TZIC0_BASE + 0x20)

#define TZIC1_STATUS      (TZIC1_BASE + 0x00)
#define TZIC1_RAWSTATUS   (TZIC1_BASE + 0x04)
#define TZIC1_INTSELECT   (TZIC1_BASE + 0x08)
#define TZIC1_ENABLE      (TZIC1_BASE + 0x0C)
#define TZIC1_CLEAR       (TZIC1_BASE + 0x10)
#define TZIC1_PROTECTION  (TZIC1_BASE + 0x18)
#define TZIC1_LOCK        (TZIC1_BASE + 0x1C)
#define TZIC1_LOCK_STATUS (TZIC1_BASE + 0x20)

#define TZIC_UNLOCK_CODE 0x0ACCE550
#define TZIC_LOCK_CODE   0x00000000

typedef unsigned int uint32_t;
typedef unsigned long long	uint64_t;

void tzic_init(uint64_t int_bit_pattern)
{
	uint32_t mask0 = (uint32_t)(int_bit_pattern);
	uint32_t mask1 = (uint32_t)(int_bit_pattern >> 32);

	// unlock tzic
	*((volatile uint32_t *) TZIC0_LOCK) = TZIC_UNLOCK_CODE;
	*((volatile uint32_t *) TZIC1_LOCK) = TZIC_UNLOCK_CODE;

	// disable all interrupts
	*((volatile uint32_t *) TZIC0_CLEAR) = ~0U;
	*((volatile uint32_t *) TZIC1_CLEAR) = ~0U;

	// fiq generation
	*((volatile uint32_t *) TZIC0_INTSELECT) = mask0;
	*((volatile uint32_t *) TZIC1_INTSELECT) = mask1;

	// enable selected interrupts
	*((volatile uint32_t *) TZIC0_ENABLE) = mask0;
	*((volatile uint32_t *) TZIC1_ENABLE) = mask1;

	// lock tzic
	*((volatile uint32_t *) TZIC0_LOCK) = TZIC_LOCK_CODE;
	*((volatile uint32_t *) TZIC1_LOCK) = TZIC_LOCK_CODE;
}
