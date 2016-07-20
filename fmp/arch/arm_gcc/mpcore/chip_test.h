/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel  
 * 
 *  Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: chip_test.h 498 2010-01-20 09:55:11Z ertl-honda $
 */

/*
 *		�ƥ��ȥץ����Υ��åװ�¸�����MPCore�ѡ�
 */

#ifndef TOPPERS_CHIP_TEST_H
#define TOPPERS_CHIP_TEST_H

/*
 *  ��ǽɾ���ץ����Τ�������
 */

/*
 *  �����ޤγ�����ֹ�
 */
#define _INTNO_TIMER  29

/*
 *  ��ǽɾ������ν���
 */
#ifndef CPU1_PERF_PRE_HOOK
#define CPU1_PERF_PRE_HOOK    dis_int(_INTNO_TIMER);
#endif  /* CPU1_PERF_PRE_HOOK */

#ifndef CPU1_PERF_POST_HOOK
#define CPU1_PERF_POST_HOOK   ena_int(_INTNO_TIMER);
#endif  /* CPU1_PERF_POST_HOOK */

#ifdef TOPPERS_SYSTIM_LOCAL

#ifndef CPU2_PERF_PRE_HOOK
#define CPU2_PERF_PRE_HOOK    dis_int(_INTNO_TIMER);
#endif  /* CPU2_PERF_PRE_HOOK */
#ifndef CPU2_PERF_POST_HOOK
#define CPU2_PERF_POST_HOOK   ena_int(_INTNO_TIMER);
#endif  /* CPU2_PERF_POST_HOOK */

#endif /* TOPPERS_SYSTIM_LOCAL */

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

#endif /* TOPPERS_CHIP_TEST_H */
