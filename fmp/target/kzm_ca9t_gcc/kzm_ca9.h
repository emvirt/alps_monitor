/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2006-2010 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: kzm_ca9.h 599 2010-05-07 08:00:38Z ertl-honda $
 */

/*
 * KZM_CA9�ѥɥ饤�����
 */

#ifndef TOPPERS_KZM_CA9_H
#define TOPPERS_KZM_CA9_H

#include <sil.h>

/*
 * ��������å���400MHz��
 */
#define KZM_CA9_CORE_CLK  400
        
/*
 *  MPCore Private Memory Regin Base Address
 */
#define MPCORE_PMR_BASE  0x1e000000

/*
 *  MPCore��¢�Υ����ޤ�1MHz��ư����뤿��Υץꥹ�����顼��������
 */
#define MPCORE_TM_PS_1MHZ    199   /* ��������å�400Mhz */

/*
 * �١������ɥ쥹
 */
#define SYS_BASE   0x10000000

/*
 *  ������ֹ�
 */
#define DIC_IRQNO_UART2   64
#define DIC_IRQNO_TM_SP804_0 80
#define DIC_IRQNO_TM_SP804_1 81
#define DIC_IRQNO_UART0   84

/*
 * TestChip��¢�����޴�Ϣ(SP804)
 */

/* 
 * �١������ɥ쥹
 */
#define TIMER_BASE_REG  (SYS_BASE + 0xe4000)


#ifndef TOPPERS_MACRO_ONLY

/*
 *  TestChip��¢�����ޤν����
 */
extern void timer_sp804_init(void);

/*
 *  TestChip��¢�����ޤθ����ͤ��ɽФ�
 */
Inline uint32_t
timer_sp804_get_current(void)
{
	return (0xffffffff - sil_rew_mem((void *)(TIMER_BASE_REG + 0x004))); // 0x004 == TIMER_SP804_1VALUE
}

#endif /* TOPPERS_MACRO_ONLY */

/*
 * UART��Ϣ�����
 * �쥸�����������
 * 	��arm_primecell_uart.h(UART0)
 * 	��tl16c554a.h(UART2)
 * �ǹԤäƤ���
 */
 
/* 
 * �١������ɥ쥹
 */
#define UART0_BASE  (SYS_BASE + 0xe3000)
#define SMC0_CS2_BASE	0x48000000
#define UART2_CSA_BASE (SMC0_CS2_BASE + 0x00000000) 
#define UART2_CSB_BASE (SMC0_CS2_BASE + 0x00800000) 
#define UART2_CSC_BASE (SMC0_CS2_BASE + 0x01000000) 
#define UART2_CSD_BASE (SMC0_CS2_BASE + 0x01800000) 


/*
 * ���ץ�ؤΥ��������ޥ���
 */
//#define CP15_SET_VBAR(x)    Asm("mcr p15, 0, %0, c12, c0, 0"::"r"(x))




/*
 *  BPS����(38400bps)
 */ 
#define UART_IBRD_38400   0x0c
#define UART_FBRD_38400   0x00

#ifndef TOPPERS_MACRO_ONLY
/*
 *  UART����Υݡ���󥰽���
 */
extern void kzm_ca9_putc(char_t c);

/*
 * �ݡ���󥰽��ϤΤ���ν����
 */
extern void kzm_ca9_uart_init(void);

/*
 *  ��ȯ�Ķ���¸�ν���(T.B.D)
 */
Inline void
kzm_ca9_exit(void){
	while(1);
}

/*
 *  �ȥ졼�����˴ؤ������
 *  SCU���Ѥ��롥
 */
#define TRACE_HW_INIT()   scu_counter_init()
#define TRACE_GET_TIM()   ((scu_counter_get_current() * 1000) / CORE_CLK)

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_KZM_CA9_H */
