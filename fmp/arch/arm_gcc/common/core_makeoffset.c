/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2008 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: core_makeoffset.c 383 2009-09-14 11:11:04Z ertl-honda $
 */

#include "kernel_impl.h"
#include "task.h"

#define OFFSET_DEF(TYPE, FIELD)							\
	Asm("OFFSET_DEF " #TYPE "_" #FIELD " = %0"			\
	  : /* no output */									\
	  : "g"(offsetof(TYPE, FIELD)))

#define OFFSET_DEF2(TYPE, FIELD, FIELDNAME)				\
	Asm("OFFSET_DEF " #TYPE "_" #FIELDNAME " = %0"		\
	  : /* no output */									\
	  : "g"(offsetof(TYPE, FIELD)))

void
makeoffset(void)
{
	OFFSET_DEF(TCB, p_tinib);
	OFFSET_DEF(TCB, texptn);
	OFFSET_DEF2(TCB, tskctxb.sp, sp);
	OFFSET_DEF2(TCB, tskctxb.pc, pc);

	OFFSET_DEF(TINIB, exinf);
	OFFSET_DEF(TINIB, task);

	OFFSET_DEF(PCB, p_runtsk);
	OFFSET_DEF(PCB, p_schedtsk);
	OFFSET_DEF(PCB, reqflg);
	OFFSET_DEF(PCB, disdsp);
	OFFSET_DEF(PCB, dspflg);
	OFFSET_DEF(PCB, locspnid);
#if TTYPE_KLOCK != G_KLOCK
	OFFSET_DEF(PCB, p_firstlock);
	OFFSET_DEF(PCB, p_secondlock);
#endif /* TTYPE_KLOCK != G_KLOCK */
}

uint32_t	BIT_REF_4 = 0x12345678;
uint16_t	BIT_REF_2 = 0x1234;
uint8_t		BIT_REF_1 = 0x12;

TCB	BIT_BB_TCB_enatex = {
	{ NULL, NULL }, NULL, 0U, 0U,
	false, false, true,
	0U, NULL, { NULL, NULL }, 0U,
#if TTYPE_KLOCK != G_KLOCK
	false, /* pend_relwai */
	false, /* pend_chgpri */
	0U,    /* pend_newpri */
#endif /* TTYPE_KLOCK != G_KLOCK */
	0U, {0U, NULL, NULL}, NULL, {{0U, 0U, 0U}}
};