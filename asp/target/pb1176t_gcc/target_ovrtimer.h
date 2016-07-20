/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005,2006 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_ovrtimer.h 264 2009-11-17 08:23:10Z dsl $
 */

/*
 *		�����Х�󥿥��ޥɥ饤�С�PB1176�ѡ�
 */

#ifndef TOPPERS_TARGET_OVRTIMER_H
#define TOPPERS_TARGET_OVRTIMER_H

#include <sil.h>
#include "pb1176t.h"
#include "target_config.h"

/*
 *  �����Х�󥿥��޳���ߥϥ�ɥ���Ͽ�Τ�������
 */
#define INHNO_OVRTIMER	IRQNO_OVRTIMER		/* ����ߥϥ�ɥ��ֹ� */
#define INTNO_OVRTIMER	IRQNO_OVRTIMER		/* ������ֹ� */
#define INTPRI_OVRTIMER	-5					/* �����ͥ���� */
#define INTATR_OVRTIMER	0					/* �����°�� */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  �������ͤ�����ɽ���η�
 */
typedef ulong_t		OVRCLOCK;

/*
 *  �������ͤ�����ɽ���ȥץ��å����֤Ȥ��Ѵ�
 *
 *  PB1176�Ǥϡ������ޤ�1������˥�����ȥ��åפ��롥
 */
#define TO_OVRCLOCK(ovrtim)		((ulong_t)(ovrtim))
#define TO_OVRTIM(clk)			((OVRTIM)(clk))

/*
 *  �����Х�󥿥��ޤν��������
 *
 *  �����Х�󥿥��ޤν����������Ԥ��������ޤ�ư��ϤϹԤ�ʤ�����
 *  ���ߥϥ�ɥ����Ͽ�ϡ�target_ovrtimer.cfg�򥳥�ե�����졼���˽�������
 *  �뤳�ȤǹԤ���
 */
Inline void
target_ovrtimer_initialize(void)
{
	sil_wrw_mem((void *)(OVRTIMER_BASE_REG + TIMER_SP804_1CONTROL),
		OVRTIMER_SP804_CONTROL_BITS);

	sil_wrw_mem((void *)(OVRTIMER_BASE_REG + TIMER_SP804_1CLEAR), 0x01);
}

/*
 *  �����Х�󥿥��ޤ�ư���
 *
 *  �����Х�󥿥��ޤˡ��Ĥ����ovrtim�����ꤷ��ư��Ϥ��롥ovrtim��
 *  ñ�̤Ϧ��äȤ��롥
 */
Inline void
target_ovrtimer_start(OVRTIM ovrtim)
{
	assert(ovrtim > 0u);
	assert(ovrtim <= TMAX_OVRTIM);

	sil_wrw_mem((void *)(OVRTIMER_BASE_REG + TIMER_SP804_1LOAD),
		TO_OVRCLOCK(ovrtim));

	sil_wrw_mem((void *)(OVRTIMER_BASE_REG + TIMER_SP804_1CONTROL),
		OVRTIMER_SP804_CONTROL_BITS | OVRTIMER_SP804_CONTROL_ENA);
}

/*
 *  �����Х�󥿥��ޤλĤ���֤μ���
 *
 *  �����Х�󥿥��ޤλĤ���֤��ɤ߽Ф���
 */
Inline OVRTIM
target_ovrtimer_get_current(void)
{
	return(TO_OVRTIM(
		sil_rew_mem((void *)(OVRTIMER_BASE_REG + TIMER_SP804_1VALUE))));
}

/*
 *  �����Х�󥿥��ޤ����
 *
 *  �����Х�󥿥��ޤ���ߤ��������ޤλĤ���֤��ɤ߽Ф����⤷�Ĥ����
 *  ��0�ʤޤ��Ϥ���̤���ˤˤʤäƤ������ˤϡ�1���֤����ޤ��������Х�
 *  �󥿥��ޤ���γ���ߤ򥯥ꥢ���롥
 */
Inline OVRTIM
target_ovrtimer_stop(void)
{
	OVRCLOCK cyc;

	sil_wrw_mem((void *)(OVRTIMER_BASE_REG + TIMER_SP804_1CONTROL),
		OVRTIMER_SP804_CONTROL_BITS);

	cyc = sil_rew_mem((void *)(OVRTIMER_BASE_REG + TIMER_SP804_1VALUE));

	return TO_OVRTIM(cyc);
}

/*
 *  �����Х�󥿥��޳���ߥϥ�ɥ�
 */
extern void	target_overtimer_handler(void);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_TARGET_OVRTIMER_H */
