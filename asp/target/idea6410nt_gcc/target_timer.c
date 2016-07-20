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

/*
 *  �����ޤε�ư����
 */
void
target_timer_initialize(intptr_t exinf)
{
	uint32_t bits;
	
	// clear and disable the timer interrupt
	bits = sil_rew_mem((void *)S3C_TINT_CSTAT);
	bits &= ~(0x1F << 5);
	bits |= TARGET_TIMER_INT_CLEAR;
	bits &= ~TARGET_TIMER_INT_ENABLE;
	sil_wrw_mem((void *)(S3C_TINT_CSTAT), bits);
	
	// set preescaler
	bits = sil_rew_mem((void *)S3C_TCFG0);
	bits &= ~TARGET_TIMER_PREESCALER_MASK;
	bits |= TARGET_PREESCALER_VALUE;
	sil_wrw_mem((void *)(S3C_TCFG0), bits);
	
	// set the divider
	bits = sil_rew_mem((void *)S3C_TCFG1);
	bits &= ~TARGET_TIMER_DIVIDER_MASK;
	bits |= TARGET_TIMER_DIVIDER_VALUE;
	sil_wrw_mem((void *)(S3C_TCFG1), bits);
	
	// set the load for 1 ms
	sil_wrw_mem((void *)(TARGET_TIMER_TCNTB), TO_CLOCK(TIC_NUME, TIC_DENO));
	
	// set periodic and update value
	bits = sil_rew_mem((void *)S3C_TCON);
	bits &= ~TARGET_TIMER_CONTROL_MASK;
	bits |= TARGET_TIMER_CONTROL_UPDATE;
	sil_wrw_mem((void *)(S3C_TCON), bits);
	
	// enable the timer interrupt
	bits = sil_rew_mem((void *)S3C_TINT_CSTAT);
	bits &= ~(0x1F << 5);
	bits |= TARGET_TIMER_INT_ENABLE;
	sil_wrw_mem((void *)(S3C_TINT_CSTAT), bits);
	
	// start the timer
	bits = sil_rew_mem((void *)S3C_TCON);
	bits &= ~TARGET_TIMER_CONTROL_MASK;
	bits |= TARGET_TIMER_CONTROL_START;
	sil_wrw_mem((void *)(S3C_TCON), bits);
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
	uint32_t bits;
	
	// clear the interrupt bit
	bits = sil_rew_mem((void *)S3C_TINT_CSTAT);
	bits &= ~(0x1F << 5);
	bits |= TARGET_TIMER_INT_CLEAR;
	sil_wrw_mem((void *)(S3C_TINT_CSTAT), bits);
	
	i_begin_int(INTNO_TIMER);
	signal_time();                    /* ������ƥ��å��ζ��� */
	i_end_int(INTNO_TIMER);
}
