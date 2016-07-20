/*
 *	TOPPERS/FMP Kernel
 *		Toyohashi Open Platform for Embedded Real-Time Systems/
 *		Advanced Standard Profile Kernel
 *	
 *	Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *				Graduate School of Information Science, Nagoya Univ., JAPAN
 *	Copyright (C) 2009-2010 by Industrial Technology Institute,
 *								Miyagi Prefectural Government, JAPAN
 *	
 *	�嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *	�����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *	�ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *	(1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *		��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *		����������˴ޤޤ�Ƥ��뤳�ȡ�
 *	(2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *		�ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *		�ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *		��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *	(3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *		�ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *		�ȡ�
 *	  (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *		  �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *	  (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ������Ȥ�
 *		  ��𤹤뤳�ȡ�
 *	(4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *		������⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ����դ��뤳�ȡ�
 *		�ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *		ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ�
 *		���դ��뤳�ȡ�
 *	
 *	�ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *	���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *	���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *	�������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *	����Ǥ�����ʤ���
 * 
 *  @(#) $Id$
 */

/*
 *	�������åȰ�¸�⥸�塼���APSH2AD�ѡ�
 */

#include "kernel_impl.h"
#include <sil.h>
#include "pdic/sh/sh_scif.h"
#include "apsh2ad.h"
#include "target_syssvc.h"		/*  TNUM_PORT  */

/*
 *  str_ker() �μ¹����˥ޥ����ץ��å��Τ߼¹Ԥ������������
 */
void
target_mprc_initialize(void)
{
	prc_mprc_initialize();

	/*
	 *	��������ϥ⡼�ɤ�����
	 */
		/*
		 *	CPU0��sleep̿��μ¹Ԥǡ�CPU0�����꡼�ץ⡼�ɤ�����
		 *	�������꡼�ץ��顼�ػ�
		 *	���������ǥ����ѿ徽ȯ���Ҥ�̵��
		 */
	sil_wrb_mem((void *)STBCR1, STBCR1_AXTALE);

		/*
		 *	����H-UDI��ư��
		 *	����FPU0,FPU1��ư��
		 *	����UBC0,UBC1��ư��
		 *	��
		 *	������
		 *	����JTAG���ߥ�졼������Ѥ�����ϡ�
		 *	����H-UDI��FPU��UBC����ߤ��ƤϤ����ʤ���
		 */
	sil_wrb_mem((void *)STBCR2, 0x00U);

		/*
		 *	����ATAPI�����
		 *	����MTU2�����
		 *	����ADC�����
		 *	����DAC�����
		 *	����RTC�����
		 */
	sil_wrb_mem((void *)STBCR3, 0xffU);

		/*
		 *	����SCFI0��ư��
		 *	����SCFI1��ư��
		 *	����SCFI2�����
		 *	����SCFI3�����
		 *	����SCFI4�����
		 *	����SCFI5�����
		 */
	sil_wrb_mem((void *)STBCR4, (uint8_t)~(STBCR4_SCIF0 | STBCR4_SCIF1));

		/*
		 *	����IIC3_0�����
		 *	����IIC3_1�����
		 *	����IIC3_2�����
		 *	����IIC3_3�����
		 *	����RCAN0�����
		 *	����RCAN1�����
		 */
	sil_wrb_mem((void *)STBCR5, 0xffU);

		/*
		 *	����SSIF0�����
		 *	����SSIF1�����
		 *	����SSIF2�����
		 *	����SSIF3�����
		 *	����SSIF4�����
		 *	����SSIF5�����
		 */
	sil_wrb_mem((void *)STBCR6, 0xffU);

		/*
		 *	����CMT01��ư��
		 *	����CMT23�����
		 *	����FLCTL�����
		 *	����SSU0�����
		 *	����SSU1�����
		 *	����2DG�����
		 *	����USB�����
		 */
	sil_wrb_mem((void *)STBCR7, (uint8_t)~STBCR7_CMT01);

	/*
	 *	SCIF��I/O�ݡ��Ȥ�����	 �׸�Ƥ:PE4/SCK0ü�Ҥ������ɬ�ס�
	 */
	sil_wrh_mem((void *)PECRL1, 0x1111U);/* SCIF0:RTS0 CTS0 TxD0 RxD0 */
#if TNUM_PORT > 1
	sil_wrh_mem((void *)PECRL2, 0x0110U);/* SCIF1:TxD1 RxD1 */
#endif /* TNUM_PORT > 1 */
	
	/*
	 * �С��ʡ������ѤΥ��ꥢ�륳��ȥ���ν����
	 */
	sh_scif_init(1);
#if TNUM_PORT > 1
	sh_scif_init(2);
#endif /* TNUM_PORT > 1 */
}

/*
 *	�������åȰ�¸�ν����
 */
void
target_initialize(void)
{
	/*
	 *	�ץ��å���¸�ν����
	 */
	prc_initialize();
}

/*
 *	�������åȰ�¸�ν�λ����
 */
void
target_exit(void)
{
	/*
	 *	�ץ��å���¸�ν�λ����
	 */
	prc_exit();

	/*
	 *	��ȯ�Ķ���¸�ν�λ����
	 */
	apsh2ad_exit();

	/*
	 * �����ˤ���ʤ�
	 */
	while(1);
}

/*
 *	�����ƥ�������٥���ϤΤ����ʸ������
 */
void
target_fput_log(char_t c)
{
#if TNUM_PORT == 1
	ID siopid = TARGET_PUTC_PORTID;
#else	/* TNUM_PORT == 1 */
	ID siopid = sil_get_pindex() + 1;
#endif	/* TNUM_PORT == 1 */

	if (c == '\n') {
		sh_scif_pol_putc('\r', siopid);
	}
	sh_scif_pol_putc(c, siopid);
}


/*
 *	�����������μ��Ի��μ¹����ǽ�����T.B.D��
 */
void
TOPPERS_assert_abort(void)
{
	uint_t index = sil_get_pindex();
	
	syslog(LOG_EMERG, "prcindex = %d" ,index);
	while(1);
}

