/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: chip_config.c 543 2010-02-26 01:37:54Z ertl-honda $
 */

/*
 * �������åȰ�¸�⥸�塼���MPCORE�ѡ�
 */

#include "kernel_impl.h"
#include <sil.h>
#include "mp.h"

/*
 *  SIL�Υ��ԥ��å����ѿ�
 */
uint32_t TOPPERS_spn_var;

#ifdef USE_IPI_DIS_HANDER_BYPASS
/*
 *  ��λ�������ϥե饰
 */
static bool_t ext_ker_reqflg;
#endif /* USE_IPI_DIS_HANDER_BYPASS */

/*
 *  str_ker() �μ¹����˥ޥ����ץ��å��Τ߼¹Ԥ������������
 */
void
chip_mprc_initialize(void)
{
	/*
	 *  DIC�ν����
	 */
	dic_init();

	/*
	 *  SCU�ν����
	 */
	scu_init();

	/*
	 *  SIL�Υ��ԥ��å����ѿ��ν����
	 */
	TOPPERS_spn_var = 0U;

#ifdef USE_IPI_DIS_HANDER_BYPASS
	/*
	 *  ��λ�������ϥե饰�ν����
	 */
	ext_ker_reqflg = false;
#endif /* USE_IPI_DIS_HANDER_BYPASS */
}


/*
 *  ���åװ�¸�ν����
 */
void
chip_initialize(void)
{
	TPCB *p_tpcb = get_my_p_tpcb();
	uint32_t index = x_prc_index();

#ifdef USE_THREAD_ID_PCB
	/*
	 *  Thread ID �쥸�����ؤ�PCB�ؤΥݥ��󥿤�����
	 */
	CPU15_PONLY_THREAD_ID_WRITE(p_pcb_table[x_prc_index()]);
#endif /* USE_THREAD_ID_PCB */


	p_tpcb = get_my_p_tpcb();


	/*
	 * ARM��¸�ν����
	 */
	core_initialize();



	/*
	 * ����å����̵����
	 */
	mpcore_cache_disable();


	/*
	 * MMU��ͭ����
	 */
	mpcore_mmu_init();


	/*
	 * SMP�⡼�ɤ�
	 */
	mpcore_smp_mode_enable();

	/*
	 * ����å����ͭ����
	 */
	mpcore_cache_enable();

	/*
	 * (�ޥ����ץ��å��ʳ��Ǥ�¹Ԥ���)DIC�ν����
	 * SGI�γ�����ֹ�Υ������ƥ���������ץ��å��ǹԤ�ɬ�פ����롣
	 */
#ifndef NONTRUST
	dic_sgi_sec_init();
#endif

	/*
	 * DIC��CPU���󥿥ե����������
	 */
	dic_cpu_init();

	/*
	 *  ����ߥϥ�ɥ�ơ��֥�ؤΥݥ��󥿤ν����
	 */
	p_tpcb->p_inh_table = p_inh_table[index];
}

/*
 *  ���åװ�¸�ν�λ����
 */
void
chip_exit(void)
{
	/*
	 *  ARM��¸�ν�λ����
	 */
	core_terminate();

	/*
	 *  DIC��CPU���󥿥ե����������
	 */
	dic_cpu_stop();

	/*
	 *  DIC��DIS���󥿥ե����������
	 */
	if (x_sense_mprc()) {
		dic_stop();
	}
}

/*
 *  ������׵�饤���°��������
 *
 *  ASP�����ͥ�Ǥ����Ѥ����ꤷ�ơ��ѥ�᡼�����顼�ϥ����������ǥ�����
 *  �����Ƥ��롥FI4�����ͥ�����Ѥ�����ˤϡ����顼���֤��褦�ˤ��٤�
 *  �Ǥ�����
 */
void
x_config_int(INTNO intno, ATR intatr, PRI intpri, uint_t affinity_mask)
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
		dic_config(INTNO_MASK(intno), DIC_ICR_EDGE);
		x_clear_int(intno);
	}
	else {
		dic_config(INTNO_MASK(intno), DIC_ICR_LEVEL);
	}

	/*
	 * �����ͥ���٥ޥ���������
	 */
	dic_set_priority(INTNO_MASK(intno), INT_IPM(intpri));

	/*
	 * �������å�CPU������ʥ����Х����ߤΤߡ�
	 */
	if (INTNO_MASK(intno) >= TMIN_GLOBAL_INTNO) {
		dic_set_target(INTNO_MASK(intno), (1 << x_prc_index()));
	}

	/*
	 * ����ߤ����
	 */
	if ((intatr & TA_ENAINT) != 0U) {
		(void)x_enable_int(intno);
	}
}

#ifndef OMIT_DEFAULT_INT_HANDLER

/*
 *  ̤����γ���ߤ����ä����ν���
 */
void
default_int_handler(void){
	ID prc_id = ID_PRC(x_prc_index());
	
	syslog_1(LOG_EMERG, "Processor %d : Unregistered Interrupt occurs.", prc_id);
	target_exit();
}

#endif /* OMIT_DEFAULT_INT_HANDLER */

#ifdef USE_IPI_DIS_HANDER_BYPASS

static bool_t ext_ker_reqflg;

/*
 *  �����ͥ뽪λ�����׵�
 */
void
ext_ker_request(void)
{
	ID prcid;

	/* ���Ǥ��׵᤬�ФƤ���Х꥿���� */
	if (ext_ker_reqflg) {
		return;
	}

	ext_ker_reqflg = true;

	for(prcid = 1; prcid <= TNUM_PRCID; prcid++){
		if (prcid != ID_PRC(x_prc_index())) {
			dic_swi((1<<(prcid-1)), DIC_IRQNO_IPI1);
		}
	}
}

/*
 *  �����ͥ뽪λ�����ѤΥץ��å��ֳ���ߥϥ�ɥ�
 */
void
ipi_ext_handler(void)
{
	ext_ker();
}
#endif /* USE_IPI_DIS_HANDER_BYPASS */
