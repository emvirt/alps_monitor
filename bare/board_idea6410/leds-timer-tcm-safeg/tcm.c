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
extern char __itcm_start, __itcm_end, __text_load_start;

#define __section(S) __attribute__ ((__section__(#S)))
#define __init __attribute__((long_call)) __section(.initialization)

static void __init copy(char *dest, char *src, unsigned int count) {
	char *dst8 = dest;
	char *src8 = src;

	while (count--) {
		*dst8++ = *src8++;
	}
}

static void __init itcm_copy(void)
{
	char *start;
	char *end;
	char *ram;
	
	start = &__itcm_start;
	end   = &__itcm_end;
	ram   = &__text_load_start;
	
	copy(start, ram, (unsigned int)(end-start));
}

#define CP15_TCM_SEL_WRITE(x)   __asm__ volatile ("mcr p15, 0, %0, c9, c2, 0"::"r"(x))
#define CP15_ITCM_NS_WRITE(x)   __asm__ volatile ("mcr p15, 0, %0, c9, c1, 3"::"r"(x))
#define CP15_ITCM_REG_WRITE(x)  __asm__ volatile ("mcr p15, 0, %0, c9, c1, 1"::"r"(x))

void __init tcm_init(void)
{
	CP15_TCM_SEL_WRITE(0);
	CP15_ITCM_NS_WRITE(0);
	CP15_ITCM_REG_WRITE(0x80000000 | 1);
	itcm_copy();
	__asm__ volatile("b 0x80000000");
}
