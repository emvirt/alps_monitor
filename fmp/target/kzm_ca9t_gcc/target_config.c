/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2007-2010 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_config.c 599 2010-05-07 08:00:38Z ertl-honda $
 */

/*
 * �������åȰ�¸�⥸�塼���KZM_CA9�ѡ�
 */
#include "kernel_impl.h"
#include <sil.h>
#include "itron.h"
#include "kzm_ca9.h"

extern void *vector_table;

#define CP15_SET_VBAR(x)    Asm("mcr p15, 0, %0, c12, c0, 0"::"r"(x))

/*
 *  str_ker() �����ǥޥ����ץ��å��ǹԤ������
 */
void
target_mprc_initialize(void)
{
	/* MPCore��¸���Ǥν���� */
	chip_mprc_initialize();
}

/*
 * ����Υ١������ɥ쥹�ȥ�����
 */
#define DDR_ADDR 0x00000000
#define DDR_SIZE 0x04000000 /* 64MB */


#define SIZE_1M    0x00100000


/*
 * ����ޥåפν������chip��¸������ƤӽФ�����
 * Ʊ���˥���å����ͭ���ˤ���
 */
void
target_mmu_init(void)
{
	MEMORY_ATTRIBUTE m_attribute;

	/*
	 *  DDR������(����å���ͭ����)
	 */
	m_attribute.pa   = DDR_ADDR;
	m_attribute.va   = m_attribute.pa;
	m_attribute.size = DDR_SIZE;
	m_attribute.s    = 1;		  /* ��ͭ		 */
	m_attribute.ap   = 3;		  /* �ե륢������ */
	m_attribute.tex  = 1;		  /* Outer and Inner */
	m_attribute.c    = 1;		  /* Inner Write-Back, Write Allocate */ 
	m_attribute.b    = 1;

	mmu_map_memory(&m_attribute);
	
	
	
//	m_attribute.pa   = 0x01000000;
//    m_attribute.va   = m_attribute.pa;
//    m_attribute.size = 0x01000000;
//    m_attribute.s    = 0;    /* not-shared */
//    m_attribute.ap   = 3;    /* �ե륢������ */
//    m_attribute.tex  = 0;    /* 0: Outer and Inner Write-Back */
//    m_attribute.c    = 1;    /* cb:11 No Allocate on Write */
//    m_attribute.b    = 1;

//    mmu_map_memory (&m_attribute);
    
    
	/*
	 *  �٥������ơ��֥��0x0���֤������
	 *  �ƥ����ȥ�������󳫻ϥ��ɥ쥹���ۥ��ɥ쥹��0x0�ˤ���
     */
//  	m_attribute.pa   = 0x01000000;
//	m_attribute.va   = 0x00000000;
//	m_attribute.size = 0x01000000; /* 16MB */
//	m_attribute.s    = 1;		   /* ��ͭ		 */
//	m_attribute.ap   = 3;		   /* �ե륢������ */
//	m_attribute.tex  = 1;		   /* Outer and Inner */
//	m_attribute.c    = 1;		   /* Inner Write-Back, Write Allocate */ 
//	m_attribute.b    = 1;
//	
//	mmu_map_memory(&m_attribute);
	
	
	
	
}


/*
 *  �������åȰ�¸�ν����
 */
void
target_initialize(void)
{
	/*
	 * ���åװ�¸�ν����
	 */
	chip_initialize();

	/*
	 *  UART������
	 */
#ifdef G_SYSLOG
	if (x_sense_mprc()) {
		kzm_ca9_uart_init();
	}
#else
	naviengine_uart_init();
#endif /* G_SYSLOG */

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
	 *  ���åװ�¸�ν�λ����
	 */
	chip_exit();

	/*
	 *  ��ȯ�Ķ���¸�ν�λ����
	 */
	kzm_ca9_exit();

	while(1);
}

/*
 *  �����ƥ�������٥���ϤΤ����ʸ������
 */
void
target_fput_log(char_t c)
{
	if (c == '\n') {
		kzm_ca9_putc('\r');
	}
	kzm_ca9_putc(c);
}

/*
 *  �¹Ի���ʬ�۽��ץ⥸�塼����Ѥ���¬���ѥ����ޤν�����ؿ�
 */
//void
//perf_timer_initialize(intptr_t exinf)
//{
//	timer_sp804_init();
//}

/*
 *  �¹Ի���ʬ�۽��ץ⥸�塼���ѤΥ����󥿤ˤ���¬
 */
//void
//perf_timer_get(uint32_t *p_time)
//{
//	*p_time = timer_sp804_get_current();
//}

/*
 *  �¹Ի���ʬ�۽��ץ⥸�塼���ѤΥ����󥿤ˤ��¬���̤�ñ���Ѵ�
 *  nsec���Ѵ� 
 */
uint32_t
perf_timer_conv_tim(uint32_t time)
{
	/* 100Mhz�ǥ������ */
	return (time * 10);
}
