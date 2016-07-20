/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2007-2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_test.h 502 2010-01-25 01:55:04Z ertl-honda $
 */

/*
 *		�ƥ��ȥץ����Υ��åװ�¸�����AT91SKYEYE�ѡ�
 */

#ifndef TOPPERS_TARGET_TEST_H
#define TOPPERS_TARGET_TEST_H

/*
 *  ��ǽɾ���ץ����Τ�������
 */

/*
 *  �����ͥ����������Ƥ���ؿ��γ������
 */
extern void cycle_counter_get(uint32_t *p_time);

/*
 *  ��ǽɾ���ѤΥޥ������
 */
#define HISTTIM  uint32_t
#define HIST_GET_TIM(p_time)  cycle_counter_get(p_time)

/*
 *  ���åפǶ��̤����
 */
#define CPUEXC1				0x10002		/* SWI�㳰 */
#define CPUEXC2				0x20002		/* SWI�㳰 */
#define CPUEXC3				0x40002		/* SWI�㳰 */
#define CPUEXC4				0x80002		/* SWI�㳰 */
#define RAISE_CPU_EXCEPTION Asm("svc #0")
#define TASK_LOOP           300000
#define ALM_TIME             100
#define CYC_TIME             100

/*
 *  TTSP�Ѥ����
 */

/*
 *  ����������
 */
#define TTSP_INVALID_ADDRESS  0x123456

/*  
 *  ������ֹ�(������)
 */
#define TTSP_INTNO_HIGHPRI_INT  9
#define TTSP_INTNO_MIDPRI_INT  10
#define TTSP_INTNO_LOWPRI_INT  11

/*
 *  ������ֹ�(�۾���)
 */
#define TTSP_INVALID_INTNO  0

/*
 *  ����ߥϥ�ɥ��ֹ�(������)
 */
#define TTSP_INHNO_HIGHPRI_INT  9
#define TTSP_INHNO_MIDPRI_INT  10
#define TTSP_INHNO_LOWPRI_INT  11

/*
 *  ����ߥϥ�ɥ��ֹ�(�۾���)
 */
#define TTSP_INVALID_INHNO  0

/*
 *  CPU�㳰�ϥ�ɥ��ֹ�(������)
 */
#define TTSP_EXCNO 2

/*
 *  CPU�㳰�ϥ�ɥ��ֹ�(�۾���)
 */
#define TTSP_INVALID_EXCNO  0

/*
 *  �ƥ����Ѥδؿ�
 */

/*
 *  �ƥ��å���������ߡ����ץ��å���
 */ 
extern void ttsp_target_stop_tick(void);

/*
 *  �ƥ��å���������ߡ�����ץ��å���
 */ 
extern void ttsp_target_stop_tick_pe(ID prcid);

/*
 *  �ƥ��å������κƳ������ץ��å���
 */ 
extern void ttsp_target_start_tick(void);

/*
 *  �ƥ��å������κƳ�������ץ��å���
 */ 
extern void ttsp_target_start_tick_pe(ID prcid);

/*
 *  �ƥ��å��ι��������ץ��å���
 */
extern void ttsp_target_gain_tick(void);

/*
 *  �ƥ��å��ι���������ץ��å���
 */
extern void ttsp_target_gain_tick_pe(ID prcid);

/*
 *  �ƥ��å��ѥ����޳�����׵��ȯ���ʼ��ץ��å���
 */ 
extern void ttsp_target_raise_tick_int(void);

/*
 *  ����ߤ�ȯ��
 */
extern void ttsp_highpri_int_raise(void);
extern void ttsp_midpri_int_raise(void);
extern void ttsp_lowpri_int_raise(void);

/*
 *  ����ߥϥ�ɥ�
 */
extern void target_highpri_int_handle(void);
extern void target_midpri_int_handle(void);
extern void target_lowpri_int_handle(void);

/*
 *  CPU�㳰��ȯ��
 */
extern void ttsp_cpuexc_raise(void);

/*
 *  �����ޥϥ�ɥ��Ѵؿ�
 */
extern bool_t ttsp_timer_handler_begin_hook(void);
extern void ttsp_timer_handler_end_hook(void);
#endif /* TOPPERS_TARGET_TEST_H */
