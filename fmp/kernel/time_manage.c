/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2009 by Embedded and Real-Time Systems Laboratory
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
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ��������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ�
 *      ���դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ��������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 * 
 *  @(#) $Id: time_manage.c 378 2009-09-11 16:21:24Z ertl-honda $
 */

/*
 *		�����ƥ���������ǽ
 */

#include "kernel_impl.h"
#include "check.h"
#include "time_event.h"
#include "task.h"
#include "mp.h"

/*
 *  �ȥ졼�������ޥ����Υǥե�������
 */
#ifndef LOG_GET_TIM_ENTER
#define LOG_GET_TIM_ENTER(p_systim)
#endif /* LOG_GET_TIM_ENTER */

#ifndef LOG_GET_TIM_LEAVE
#define LOG_GET_TIM_LEAVE(ercd, systim)
#endif /* LOG_GET_TIM_LEAVE */

#ifndef LOG_GET_UTM_ENTER
#define LOG_GET_UTM_ENTER(p_sysutm)
#endif /* LOG_GET_UTM_ENTER */

#ifndef LOG_GET_UTM_LEAVE
#define LOG_GET_UTM_LEAVE(ercd, sysutm)
#endif /* LOG_GET_UTM_LEAVE */

/*
 *  �����ƥ����λ���
 */
#ifdef TOPPERS_get_tim

ER
get_tim(SYSTIM *p_systim)
{
	ER		ercd;
	PCB		*my_p_pcb;

	LOG_GET_TIM_ENTER(p_systim);
	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	my_p_pcb = t_acquire_tsk_lock_self();
	*p_systim = my_p_pcb->p_tevtcb->current_time;
	ercd = E_OK;
	release_tsk_lock(my_p_pcb);
	t_unlock_cpu();

  error_exit:
	LOG_GET_TIM_LEAVE(ercd, *p_systim);
	return(ercd);
}

#endif /* TOPPERS_get_tim */

/*
 *  ��ǽɾ���ѥ����ƥ����λ���
 */
#ifdef TOPPERS_get_utm
#ifdef TOPPERS_SUPPORT_GET_UTM
#ifndef OMIT_GET_UTM
#ifdef TOPPERS_SYSTIM_GLOBAL
#error get_utm is not supported with Global timer.
#else /* TOPPERS_SYSTIM_LOCAL */
#include "target_timer.h"

ER
get_utm(SYSUTM *p_sysutm)
{
	SYSUTM	utime;
	SYSTIM	time;
#if TIC_DENO != 1
	uint_t	subtime;
#endif /* TIC_DENO != 1 */
	CLOCK	clock1, clock2;
	bool_t	ireq;
	PCB		*my_p_pcb; 
	SIL_PRE_LOC;

	LOG_GET_UTM_ENTER(p_sysutm);

	SIL_LOC_INT();
	my_p_pcb = acquire_tsk_lock_without_preemption_self();
	time = my_p_pcb->p_tevtcb->next_time;
#if TIC_DENO != 1
	subtime = next_subtime;
#endif /* TIC_DENO != 1 */
	clock1 = target_timer_get_current();
	ireq = target_timer_probe_int();
	clock2 = target_timer_get_current();
	release_tsk_lock(my_p_pcb);
	SIL_UNL_INT();

	utime = ((SYSUTM) time) * 1000U;
#if TIC_DENO != 1
	utime += subtime * 1000U / TIC_DENO;
#endif /* TIC_DENO != 1 */

	if (!(ireq || my_p_pcb->p_tevtcb->pend_update) || clock1 > clock2) {
		utime -= TIC_NUME * 1000U / TIC_DENO;
	}
	utime += TO_USEC(clock1);
	*p_sysutm = utime;

	LOG_GET_UTM_LEAVE(E_OK, *p_sysutm);
	return(E_OK);
}

#endif /* TOPPERS_SYSTIM_LOCAL */
#endif /* OMIT_GET_UTM */
#endif /* TOPPERS_SUPPORT_GET_UTM */
#endif /* TOPPERS_get_utm */