/*
 *  TOPPERS/SafeG Dual-OS monitor
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Safety Gate Dual-OS monitor
 *
 *  Copyright (C) 2009-2011 by Embedded and Real-Time Systems Laboratory
 *     Graduate School of Information Science, Nagoya University, JAPAN
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
 */
#define __REG(x)	(*((volatile unsigned int *)(x)))

#define TZIC_NUM_IRQS           128
#define TZIC_BASE               0x0FFFC000
#define TZIC_INTCNTL            (TZIC_BASE + 0x0000)	/* control register */
#define TZIC_INTTYPE            (TZIC_BASE + 0x0004)	/* Controller type register */
#define TZIC_IMPID              (TZIC_BASE + 0x0008)	/* Distributor Implementer Identification Register */
#define TZIC_PRIOMASK           (TZIC_BASE + 0x000C)	/* Priority Mask Reg */
#define TZIC_SYNCCTRL           (TZIC_BASE + 0x0010)	/* Synchronizer Control register */
#define TZIC_DSMINT             (TZIC_BASE + 0x0014)	/* DSM interrupt Holdoffregister */
#define TZIC_INTSEC0            (TZIC_BASE + 0x0080)	/* interrupt security register 0 */
#define TZIC_ENSET0             (TZIC_BASE + 0x0100)	/* Enable Set Register 0 */
#define TZIC_ENCLEAR0           (TZIC_BASE + 0x0180)	/* Enable Clear Register 0 */
#define TZIC_SRCSET0            (TZIC_BASE + 0x0200)	/* Source Set Register 0 */
#define TZIC_SRCCLAR0           (TZIC_BASE + 0x0280)	/* Source Clear Register 0 */
#define TZIC_PRIORITY0          (TZIC_BASE + 0x0400)	/* Priority Register 0 */
#define TZIC_PND0               (TZIC_BASE + 0x0D00)	/* Pending Register 0 */
#define TZIC_HIPND0             (TZIC_BASE + 0x0D80)	/* High Priority Pending Register */
#define TZIC_WAKEUP0            (TZIC_BASE + 0x0E00)	/* Wakeup Config Register */
#define TZIC_SWINT              (TZIC_BASE + 0x0F00)	/* Software Interrupt Rigger Register */
#define TZIC_ID0                (TZIC_BASE + 0x0FD0)	/* Indentification Register 0 */

#define MX51_INT_UART1	31

void tzic_init(void)
{
	int i;

	// Enable Secure and Non-Secure interrupts
	i = __REG(TZIC_INTCNTL);
	__REG(TZIC_INTCNTL) = 0x80010001;
	i = __REG(TZIC_INTCNTL);

	// Set priority mask to the minimum (inverse order)
	__REG(TZIC_PRIOMASK) = 0xFF;
	i = __REG(TZIC_PRIOMASK);

	// Set for low latency
	__REG(TZIC_SYNCCTRL) = 0;
	i = __REG(TZIC_SYNCCTRL);

	// Set all interrupts as Secure
	for (i = 0; i < 4; i++) {
		__REG(TZIC_INTSEC0 + i * 4) = 0;
	}

	// Set all interrupts to the highest priority (inverse order)
	for (i = 0; i < 4; i++) {
		__REG(TZIC_PRIORITY0 + i * 4) = 0;
	}

	/* Disable all interrupts */
	for (i = 0; i < 32; i++) {
		__REG(TZIC_ENCLEAR0 + i * 4) = 0xFFFFFFFF;
	}

	/* Enable the UART interrupt */
	__REG(TZIC_ENSET0) = (1 << MX51_INT_UART1);
	i = __REG(TZIC_ENSET0);
}
