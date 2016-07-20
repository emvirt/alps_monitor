/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2010 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: kzm_ca9.c 599 2010-05-07 08:00:38Z ertl-honda $
 */

#include "kernel_impl.h"
//#include "pdic/arm_primecell_uart/arm_primecell_uart.h"
#include "tl16c554a.h"
#include "target_timer.h"
#include "kzm_ca9.h"
#ifdef G_SYSLOG

//#define my_prc_uart_base	UART0_BASE
#define my_prc_uart_base	UART2_CSA_BASE

#else

/*
 *  ���٥���Ϥǥץ����å���˻��Ѥ���UART�Υ��ɥ쥹
 */
static const uint32_t prc_uart_base_table[TNUM_PRCID] = {
	UART0_BASE,
#if TNUM_PRCID >= 2
	UART1_BASE,
#endif /* TNUM_PRCID >= 2 */
#if TNUM_PRCID >= 3
	UART2_BASE,  
#endif /* TNUM_PRCID >= 3 */
#if TNUM_PRCID >= 4
	UART3_BASE,
#endif /* TNUM_PRCID >= 4 */
};

/*
 *  ���ץ����å������Ѥ���UART�Υ��ɥ쥹
 */
#define my_prc_uart_base	(prc_uart_base_tbl[x_prc_index()])

#endif /* G_SYSLOG */



/*
 *  UART����Υݡ���󥰽���
 */
void
kzm_ca9_putc(char_t c){
	/* �Хåե��������Τ��Ԥ� */
	while ((sil_rew_mem((void *)(my_prc_uart_base + TL16C554A_LSR)) & LSR_THRE) == 0);
	/* �񤭹��� */
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_THR), c);

	/* �Хåե��������Τ��Ԥ� */
	while ((sil_rew_mem((void *)(my_prc_uart_base + TL16C554A_LSR)) & LSR_THRE) == 0);


}

/*
 * �ݡ���󥰽��ϤΤ���ν����
 */
void
kzm_ca9_uart_init(void){

	/* �����߶ػ� */
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_IER), 0x00);

	/* Access baud rate */
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_LCR), LCR_BKSE);
    
	/* �ܡ��졼�Ȥ����� */
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_DLL), 12 & 0xff);
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_DLM), (12 >> 8) & 0xff);

    	/* 8 data, 1 stop, no parity */
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_LCR), LCR_8N1);

	/* no RTS/DTR */
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_MCR), MCR_VAL);

	/* Clear & enable FIFOs */
	sil_wrw_mem((void *)(my_prc_uart_base + TL16C554A_FCR), FCR_VAL);

}

/*
 *  TestChip��¢�����ޤν����
 */
void
timer_sp804_init(void)
{
#if 0
	/* ��������� */
	sil_wrw_mem((void *)(TIMER_BASE_REG + TIMER_SP804_1CONTROL), 0x00);
	
	/* ��������ͤ򥻥å� */
	sil_wrw_mem((void *)(TIMER_BASE_REG + TIMER_SP804_1LOAD), 0xffffffff);

	/* �����ޥ������ȡ��ե꡼���˥󥰡�ʬ���ʤ���32bit������ */
	sil_wrw_mem((void *)(TIMER_BASE_REG + TIMER_SP804_1CONTROL),
				(TIMER_SP804_CTRL_TIMER_EN|TIMER_SP804_CTRL_TIMESIZE_32));
#endif
}