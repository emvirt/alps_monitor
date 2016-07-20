/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2006 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ�
 *      ���դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 * 
 *  @(#) $Id: target_timer.c 264 2007-07-12 08:23:10Z hiro $
 */

/*
 *  �����ޥɥ饤�С�PB1176T�ѡ�
 */

#include "kernel_impl.h"
#include "time_event.h"
#include <sil.h>
#include "target_timer.h"

#if ASP_MEASURE_TIMER_LAT == 1
unsigned long int safeg_latency[MAX_COUNT];
unsigned int latency_counter = 0;
#endif /* MEASURE_ASP_IRQ_LATENCY */

/*
 *  �����ޤε�ư����
 */
void
target_timer_initialize(intptr_t exinf)
{
	CLOCK    cyc = TO_CLOCK(TIC_NUME, TIC_DENO);
	assert(cyc <= MAX_CLOCK);

#if ASP_MEASURE_TIMER_LAT == 1
	/*
	 * Load the performance monitor cycle counter with the value
	 * 210000 (as the board is 210MHz, this is 1ms period). Note that
	 * the counter goes up, so we substract the value from 0xFFFFFFFF.
	 * Then enable the counter and configure it to cause interrupts.
	 */
	Asm("mcr p15, 0, %0, c15, c12, 1\n"
	    "mcr p15, 0, %1, c15, c12, 0\n"
	    ::"r"(0xffffffff - cyc), "r"(0x41));
#else
	/*
	 *  �����޼��������ꤷ�������ޤ�ư��򳫻Ϥ��롥
	 */
	sil_wrw_mem((void *)(TIMER_BASE_REG + TIMER_SP804_1LOAD), 922); /* �����ͥ��å� */
	sil_wrw_mem((void *)(TIMER_BASE_REG + TIMER_SP804_1CONTROL), 0x000000E2); /* �����ޡ��������� */
#endif
}

/*
 *  �����ޤ���߽���
 */
void
target_timer_terminate(intptr_t exinf)
{

}

/*
 *  �����޳���ߥϥ�ɥ�
 */
void
target_timer_handler(void)
{
#if ASP_MEASURE_TIMER_LAT == 1
	CLOCK cyc = TO_CLOCK(TIC_NUME, TIC_DENO);
	assert(cyc <= MAX_CLOCK);
	static uint32_t result = 0;
	/*
	 * Read the performance counter in result. Disable interrupts.
	 * Clear Instruction and Data cache. Enable interrupts again.
	 * Save the result in the array safeg_latency which can be read
	 * from realview to obtain the data.
	 */
	Asm("mrc p15, 0, %0, c15, c12, 1\n"
	    "mrs r0, cpsr\n"
	    "cpsid ifa\n"
	    "mcr p15, 0, %1, c7, c5, 0\n"
	    "mcr p15, 0, %1, c7, c5, 0\n"
	    "mcr p15, 0, %1, c7, c5, 0\n"
	    "mcr p15, 0, %1, c7, c5, 0\n"
	    "mcr p15, 0, %1, c7, c14, 0\n"
	    "msr cpsr, r0\n"
	    :"=r"(result)
	    :"r"(0)
	    :"r0");

	safeg_latency[latency_counter++] = result;
	latency_counter %= MAX_COUNT;

	/* Stop counter, load 1ms, clean overflow bit, restart it*/
	Asm("mcr p15, 0, %2, c15, c12, 0\n"
	    "mcr p15, 0, %0, c15, c12, 1\n"
	    "mcr p15, 0, %1, c15, c12, 0\n"
	    ::"r"(0xffffffff - cyc), "r"(0xFF00441), "r"(0x0));
#else
	/* �����ޥ��ꥢ */
	sil_wrw_mem((void *)(TIMER_BASE_REG + TIMER_SP804_1CLEAR), 0x01);
#endif
	i_begin_int(INTNO_TIMER);
	signal_time();                    /* ������ƥ��å��ζ��� */
	i_end_int(INTNO_TIMER);
}
