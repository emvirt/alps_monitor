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
 * �������åȰ�¸�⥸�塼���SOC1176NT�ѡ�
 */

#include "kernel_impl.h"
#include <sil.h>
#include "itron.h"
#include "mmu.h"

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
     *  ARM��¸�ν����
     */
    core_initialize();

    /*
     * MMU�ν����
     */
    pb1176_mmu_init();

    /*
     * CACHE �ν����
     */
    pb1176_cache_init();

    /*
     * GIC�ν����
     */
    gic_init();

    /*
     * GIC��CPU���󥿥ե����������
     */
    gic_cpu_init();

    /*
     *  sys_putc ����ǽ�ˤʤ�褦��UART������
     */
    pb1176nt_init_uart();

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
     *  GIC��CPU���󥿥ե����������
     */
    gic_cpu_stop();

    /*
     *  GIC��DIS���󥿥ե����������
     */
    gic_stop();

    /*
     *  ��ȯ�Ķ���¸�ν�λ����
     */
    pb1176nt_exit();

    while(1);
}

/*
 *  �������åȰ�¸��ʸ������
 */
void
target_fput_log(char_t c)
{
    if (c == '\n') {
        pb1176nt_putc('\r');
    }
    pb1176nt_putc(c);
}

/*
 *  ������׵�饤���°��������
 *
 *  ASP�����ͥ�Ǥ����Ѥ����ꤷ�ơ��ѥ�᡼�����顼�ϥ����������ǥ�����
 *  �����Ƥ��롥FI4�����ͥ�����Ѥ�����ˤϡ����顼���֤��褦�ˤ��٤�
 *  �Ǥ�����
 *
 * VIC�ϡ����ƥ�٥�ȥꥬ�Τ��ᡤ°���ǡ�
 * �ȥꥬ�λ���ϤǤ��ʤ�
 */
void
x_config_int(INTNO intno, ATR intatr, PRI intpri)
{
    assert(VALID_INTNO(intno));
    assert(TMIN_INTPRI <= intpri && intpri <= TMAX_INTPRI);

    /*
     *  ������׵�Υޥ���
     *
     *  ����ߤ�����դ����ޤޡ���٥�ȥꥬ�����å��ȥꥬ������䡤��
     *  ����ͥ���٤������Ԥ��Τϴ��ʤ��ᡤ�����°���ˤ�����餺��
     *  ��ö�ޥ������롥
     */
    x_disable_int(intno);

    /*
     * °��������
     */
    if ((intatr & TA_EDGE) != 0U) {
        gic_config(intno, GIC_ICR_EDGE);
        x_clear_int(intno);
    }
    else {
        gic_config(intno, GIC_ICR_LEVEL);
    }

    /*
     * �����ͥ���٥ޥ���������
     */
    gic_set_priority(intno, INT_IPM(intpri));
    gic_set_target(intno, 1 << 0x00U);

    /*
     * ����ߤ����
     */
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
