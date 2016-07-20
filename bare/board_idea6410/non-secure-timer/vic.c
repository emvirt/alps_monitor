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
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

/*
 * VIC interrupt controller
 */

#define VIC_MAX_PRIORITY   15
#define VIC_MIN_PRIORITY    0
#define VIC_TMIN_INTNO      0U
#define VIC_TMAX_INTNO     63U
#define VIC_TNUM_INT       64U

#define S3C64XX_VIC0                (0x71200000)
#define S3C64XX_VIC1                (0x71300000)

#define S3C_VIC0REG(x)              ((x) + S3C64XX_VIC0)
#define S3C_VIC1REG(x)              ((x) + S3C64XX_VIC1)

#define S3C64XX_VIC0IRQSTATUS       S3C_VIC0REG(0x000)
#define S3C64XX_VIC1IRQSTATUS       S3C_VIC1REG(0x000)

#define S3C64XX_VIC0FIQSTATUS       S3C_VIC0REG(0x004)
#define S3C64XX_VIC1FIQSTATUS       S3C_VIC1REG(0x004)

#define S3C64XX_VIC0RAWINTR         S3C_VIC0REG(0x008)
#define S3C64XX_VIC1RAWINTR         S3C_VIC1REG(0x008)

#define S3C64XX_VIC0INTSELECT       S3C_VIC0REG(0x00C)
#define S3C64XX_VIC1INTSELECT       S3C_VIC1REG(0x00C)

#define S3C64XX_VIC0INTENABLE       S3C_VIC0REG(0x010)
#define S3C64XX_VIC1INTENABLE       S3C_VIC1REG(0x010)

#define S3C64XX_VIC0INTENCLEAR      S3C_VIC0REG(0x014)
#define S3C64XX_VIC1INTENCLEAR      S3C_VIC1REG(0x014)

#define S3C64XX_VIC0SOFTINT         S3C_VIC0REG(0x018)
#define S3C64XX_VIC1SOFTINT         S3C_VIC1REG(0x018)

#define S3C64XX_VIC0SOFTINTCLEAR    S3C_VIC0REG(0x01C)
#define S3C64XX_VIC1SOFTINTCLEAR    S3C_VIC1REG(0x01C)

#define S3C64XX_VIC0PROTECTION      S3C_VIC0REG(0x020)
#define S3C64XX_VIC1PROTECTION      S3C_VIC1REG(0x020)

#define S3C64XX_VIC0SWPRIORITYMASK  S3C_VIC0REG(0x024)
#define S3C64XX_VIC1SWPRIORITYMASK  S3C_VIC1REG(0x024)

#define S3C64XX_VIC0PRIORITYDAISY   S3C_VIC0REG(0x028)
#define S3C64XX_VIC1PRIORITYDAISY   S3C_VIC1REG(0x028)

#define S3C64XX_VIC0VECTADDR0       S3C_VIC0REG(0x100)
#define S3C64XX_VIC1VECTADDR0       S3C_VIC1REG(0x100)

#define S3C64XX_VIC0VECTPRIORITY0   S3C_VIC0REG(0x200)
#define S3C64XX_VIC1VECTPRIORITY0   S3C_VIC1REG(0x200)

#define S3C64XX_VIC0ADDRESS         S3C_VIC0REG(0xF00)
#define S3C64XX_VIC1ADDRESS         S3C_VIC1REG(0xF00)

void vic_init(uint64_t int_bit_pattern)
{
	uint32_t mask0 = (uint32_t)(int_bit_pattern);
	uint32_t mask1 = (uint32_t)(int_bit_pattern >> 32);

	// disable all interrupts
	*((volatile uint32_t *)S3C64XX_VIC0INTENCLEAR) = (uint32_t)~0;
	*((volatile uint32_t *)S3C64XX_VIC1INTENCLEAR) = (uint32_t)~0;

	// set protected (privileged access only) mode
	*((volatile uint32_t *)S3C64XX_VIC0PROTECTION) = (uint32_t)1;
	*((volatile uint32_t *)S3C64XX_VIC1PROTECTION) = (uint32_t)1;

	// set interrupts to generate IRQ requests (instead of FIQ requests)
	*((volatile uint32_t *)S3C64XX_VIC0INTSELECT) = (uint32_t)0;
	*((volatile uint32_t *)S3C64XX_VIC1INTSELECT) = (uint32_t)0;

	// enable selected interrupts
	*((volatile uint32_t *)S3C64XX_VIC0INTENABLE) = mask0;
	*((volatile uint32_t *)S3C64XX_VIC1INTENABLE) = mask1;
}
