/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2009-2010 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: arm.h 2 2008-05-09 18:12:27Z ertl-honda $
 */

#include "kernel_impl.h"
#include "time_event.h"
#include <sil.h>
#include "chip_test.h"

/*
 *  �����ޥ���ȥ���쥸����
 */
static void * const tm_cnt[] = {
	(void *)(MPCORE_PMR_BASE + 0x0708),
	(void *)(MPCORE_PMR_BASE + 0x0808),
	(void *)(MPCORE_PMR_BASE + 0x0908),
	(void *)(MPCORE_PMR_BASE + 0x0a08),
};

/*
 *  �����޳���ߥ��ơ������쥸����
 */
static void * const tm_isr[] = {
	(void *)(MPCORE_PMR_BASE + 0x070C),
	(void *)(MPCORE_PMR_BASE + 0x080C),
	(void *)(MPCORE_PMR_BASE + 0x090C),
	(void *)(MPCORE_PMR_BASE + 0x0a0C),
};

/*
 *  �����޳���ߥ��ɥ쥸����
 */
static void * const tm_lr[] = {
	(void *)(MPCORE_PMR_BASE + 0x0700),
	(void *)(MPCORE_PMR_BASE + 0x0800),
	(void *)(MPCORE_PMR_BASE + 0x0900),
	(void *)(MPCORE_PMR_BASE + 0x0a00),
};

/*
 *  �ƥ��å����������(���ץ��å�)
 */
void
ttsp_target_stop_tick(void)
{
	uint_t i;

	for(i = 1; i <= TNUM_PRCID; i++) {
		ttsp_target_stop_tick_pe(i);
	}
}

/*
 *  �ƥ��å����������(����ץ��å�)
 */
void
ttsp_target_stop_tick_pe(ID prcid)
{
	uint_t prc_index = prcid - 1;
	SIL_PRE_LOC;

	SIL_LOC_SPN();
	sil_wrw_mem(tm_cnt[prc_index], 00);
	SIL_UNL_SPN();
}

/*
 *  �ƥ��å������κƳ������ץ��å���
 */
void
ttsp_target_start_tick(void)
{
	uint_t i;

	for(i = 1; i <= TNUM_PRCID; i++) {
		ttsp_target_start_tick_pe(i);
	}
}

/*
 *  �ƥ��å������κƳ�������ץ��å���
 */
void
ttsp_target_start_tick_pe(ID prcid)
{
	uint_t prc_index = prcid - 1;
	SIL_PRE_LOC;

	SIL_LOC_SPN();
	sil_wrw_mem(tm_cnt[prc_index],
				(MPCORE_TM_PS_1MHZ << MPCORE_TM_CNT_PS_OFFSET)
				| MPCORE_TM_CNT_IEN | MPCORE_TM_CNT_AR | MPCORE_TM_CNT_EN);
	SIL_UNL_SPN();
}

/*
 *  �ƥ��å��ι��������ץ��å���(�ƥ��å���������߾��֤ǸƤӽФ�)
 */
void
ttsp_target_gain_tick(void) {
	uint_t i;

	for(i = 1; i <= TNUM_PRCID; i++) {
		ttsp_target_gain_tick_pe(i);
	}
}

/*
 *  �ƥ��å��ι���������ץ��å���(�ƥ��å���������߾��֤ǸƤӽФ�)
 */
void
ttsp_target_gain_tick_pe(ID prcid) {
	uint_t prc_index = prcid - 1;
	volatile PCB *p_pcb = get_mp_p_pcb(prcid);
	EVTTIM	current_time1, current_time2;
	ID pid;
	SIL_PRE_LOC;

	SIL_LOC_SPN();
	current_time1 = p_pcb->p_tevtcb->current_time;

	/* �����ޡ��Υ������ȡʥ����ȥ���ɤʤ���*/
	sil_wrw_mem(tm_cnt[prc_index],
				(MPCORE_TM_PS_1MHZ << MPCORE_TM_CNT_PS_OFFSET)
				| MPCORE_TM_CNT_IEN | MPCORE_TM_CNT_EN);
	SIL_UNL_SPN();

	/* ��PE���Ф��Ƥϡ����֤��ʤ�ޤ��Ԥ� */
	sil_get_pid(&pid);
	if (pid == prcid) {
		do{
			current_time2 = p_pcb->p_tevtcb->current_time;
		}while(current_time1 == current_time2);
	}
}

/*
 *  �ƥ��å��ѥ����޳�����׵��ȯ���ʼ��ץ��å���(�ƥ��å���������߾��֤ǸƤӽФ�)
 */ 
void
ttsp_target_raise_tick_int(void)
{
	uint_t prc_index = x_prc_index();
	SIL_PRE_LOC;

	SIL_LOC_SPN();

	/* �����ޡ��Υ������ȡʥ����ȥ���ɤʤ���*/
	sil_wrw_mem(tm_cnt[prc_index],
				(MPCORE_TM_PS_1MHZ << MPCORE_TM_CNT_PS_OFFSET)
				| MPCORE_TM_CNT_IEN | MPCORE_TM_CNT_EN);
	SIL_UNL_SPN();

	/* �����ȯ���ޤ��Ԥ� */
	while((sil_rew_mem(tm_isr[prc_index]) & MPCORE_TM_ISR_SCBIT) != MPCORE_TM_ISR_SCBIT);
}
