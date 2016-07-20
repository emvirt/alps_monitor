/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2007 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_config.c 369 2007-07-18 12:48:50Z honda $
 */

/*
 * �������åȰ�¸�⥸�塼���PB1176T�ѡ�
 */

#include "kernel_impl.h"
#include <sil.h>
#include "itron.h"
#include "mmu.h"

/*
 * �Ƴ���ߤγ�����׵�ػߥե饰�ξ���
 */
uint32_t idf;

/*
 * ���ߤγ����ͥ���٥ޥ�������
 */
PRI ipm;

/*
 *  �����°�������ꤵ��Ƥ��뤫��Ƚ�̤��뤿����ѿ�
 */
uint32_t	bitpat_cfgint;

/*
 *  �٥������ơ��֥����Ƭ����
 */
extern void *vector_table;

/*
 *  �������åȰ�¸�ν����
 */
void
target_initialize(void)
{
	/*
	 * Initialize TZPC
	 */
	pb1176t_tzpc_init();
	
	/*
	 *  ARM��¸�ν����
	 */
	core_initialize();

	/*
	 * Initialize the TCM
	 */
	pb1176t_init_tcm();

	/*
	 * MMU�ν����
	 */
	pb1176_mmu_init();

	/*
	 * CACHE �ν����
	 */
	pb1176_cache_init();

	/*
	 * �Ƴ���ߤγ�����׵�ػߥե饰���ػ�
	 */
	idf = ~0U;

	/*
	 * �����ͥ���٥ޥ�����0
	 */
	ipm = 0U;

	/*
	 * ���Ƥγ���ߤ�ޥ���
	 */
	tzic_disable_int(~0U);

	/*
	 *  �����°�������ꤵ��Ƥ��뤫��Ƚ�̤��뤿����ѿ����������롥
	 */
	bitpat_cfgint = 0U;

	/*
	 *  sys_putc ����ǽ�ˤʤ�褦��UART������
	 */
	pb1176t_init_uart();

	/*
	 * ����ߥ٥����ơ��֥�� VECTOR_TABLE_BASE �쥸���������ꤹ��
	 */
	CP15_SET_VBAR((uint32_t) &vector_table);
}

/*
 *  �������åȰ�¸�ν�λ����
 */
void
target_exit(void)
{
    /*
     *  ARM��¸�ν�λ����
     */
    core_terminate();

    /*
     *  ���٤Ƥγ���ߤ�ޥ������롥
     */
    tzic_disable_int(~0U);

    /*
     *  ��ȯ�Ķ���¸�ν�λ����
     */
    pb1176t_exit();

    while(1);
}

/*
 *  �������åȰ�¸��ʸ������
 */
void
target_fput_log(char_t c)
{
    if (c == '\n') {
        pb1176t_putc('\r');
    }
    pb1176t_putc(c);
}

/*
 *  ������׵�饤���°��������
 *
 *  ASP�����ͥ�Ǥ����Ѥ����ꤷ�ơ��ѥ�᡼�����顼�ϥ����������ǥ�����
 *  �����Ƥ��롥FI4�����ͥ�����Ѥ�����ˤϡ����顼���֤��褦�ˤ��٤�
 *  �Ǥ�����
 *
 * TZIC�ϡ����ƥ�٥�ȥꥬ�Τ��ᡤ°���ǡ�
 * �ȥꥬ�λ���ϤǤ��ʤ�
 */
void
x_config_int(INTNO intno, ATR intatr, PRI intpri)
{
    assert(VALID_INTNO(intno));
    assert(TMIN_INTPRI <= intpri && intpri <= TMAX_INTPRI);

	/*
	 *  �����°�������ꤵ��Ƥ��뤫��Ƚ�̤��뤿����ѿ�������
	 */
	bitpat_cfgint |= INTNO_BITPAT(intno);

    /*
     * ���ä������ߤ�ػߤ���
     */
    x_disable_int(intno);

	/*
	 * FIQ��ȯ�����Ȥ����Ѥ���
	 */
	tzic_use_fiq_gen(INTNO_BITPAT(intno));

    if ((intatr & TA_ENAINT) != 0U){
        (void)x_enable_int(intno);
    }
}

#ifndef OMIT_DEFAULT_INT_HANDLER
/*
 * ̤����γ���ߤ����ä����ν���
 */
void
default_int_handler(void){
    syslog_0(LOG_EMERG, "Unregistered Interrupt occurs.");
    target_exit();
}
#endif /* OMIT_DEFAULT_INT_HANDLER */

#ifdef TOPPERS_SUPPORT_GET_UTM
#ifdef OMIT_GET_UTM

#ifndef LOG_GET_UTM_ENTER
#define LOG_GET_UTM_ENTER(p_sysutm)
#endif /* LOG_GET_UTM_ENTER */

#ifndef LOG_GET_UTM_LEAVE
#define LOG_GET_UTM_LEAVE(ercd, sysutm)
#endif /* LOG_GET_UTM_LEAVE */

ER get_utm(SYSUTM *p_sysutm)
{
	LOG_GET_UTM_ENTER(p_sysutm);
	
	*p_sysutm = (*((volatile uint32_t *)0x1000005C) / 24);
	
	LOG_GET_UTM_LEAVE(E_OK, *p_sysutm);
	return(E_OK);
}
#endif /* OMIT_GET_UTM */
#endif /* TOPPERS_SUPPORT_GET_UTM */
