/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
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
 *  @(#) $Id: chip_asm.inc 394 2009-09-15 07:08:25Z ertl-honda $
 */

#ifndef TOPPERS_CHIP_ASM_INC
#define TOPPERS_CHIP_ASM_INC

.macro my_core_index re_reg
	mrc p15, 0, \re_reg, c0, c0, 5 /* �ץ����å�ID�μ��� */
	and \re_reg, \re_reg, #0x0f    /* ����4bit�Τ�ͭ��   */
.endm

#ifdef USE_THREAD_ID_PCB
.macro my_pcb re_reg sc_reg1
	mrc p15, 0, \re_reg, c13, c0, 4
.endm
#else  /* USE_THREAD_ID_PCB */
.macro my_pcb re_reg sc_reg1
	mrc p15, 0, \re_reg, c0, c0, 5 /* �ץ����å�ID�μ��� */
	and \sc_reg1, \re_reg, #0x0f   /* ����4bit�Τ�ͭ��   */
	ldr \re_reg, =p_pcb_table
	ldr \re_reg, [\re_reg, \sc_reg1, asl #2]
.endm
#endif /* USE_THREAD_ID_PCB */

.macro my_istkpt re_reg sc_reg1
	mrc p15, 0, \re_reg, c0, c0, 5 /* �ץ����å�ID�μ��� */
	and \sc_reg1, \re_reg, #0x0f   /* ����4bit�Τ�ͭ��   */
	ldr \re_reg, =_kernel_istkpt_table
	ldr \re_reg, [\re_reg, \sc_reg1, asl #2]
.endm

.macro toppers_asm_custom_idle
	wfi
	msr   cpsr, #(CPSR_SVC|CPSR_ALWAYS_SET) /* �����ߵ���(�����ƥ�⡼��) */
	msr   cpsr, #(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET) /* CPU���å�(�����ƥ�⡼��) */
.endm

#endif /* TOPPERS_CHIP_ASM_INC */