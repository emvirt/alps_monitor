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
#define TZIC0_BASE 0x71000000
#define TZIC1_BASE 0x71100000

#define TZIC0_STATUS      (TZIC0_BASE + 0x00)
#define TZIC0_RAWSTATUS   (TZIC0_BASE + 0x04)
#define TZIC0_INTSELECT   (TZIC0_BASE + 0x08)
#define TZIC0_ENABLE      (TZIC0_BASE + 0x0C)
#define TZIC0_CLEAR       (TZIC0_BASE + 0x10)
#define TZIC0_PROTECTION  (TZIC0_BASE + 0x18)
#define TZIC0_LOCK        (TZIC0_BASE + 0x1C)
#define TZIC0_LOCK_STATUS (TZIC0_BASE + 0x20)

#define TZIC1_STATUS      (TZIC1_BASE + 0x00)
#define TZIC1_RAWSTATUS   (TZIC1_BASE + 0x04)
#define TZIC1_INTSELECT   (TZIC1_BASE + 0x08)
#define TZIC1_ENABLE      (TZIC1_BASE + 0x0C)
#define TZIC1_CLEAR       (TZIC1_BASE + 0x10)
#define TZIC1_PROTECTION  (TZIC1_BASE + 0x18)
#define TZIC1_LOCK        (TZIC1_BASE + 0x1C)
#define TZIC1_LOCK_STATUS (TZIC1_BASE + 0x20)

#define TZIC_UNLOCK_CODE 0x0ACCE550
#define TZIC_LOCK_CODE   0x00000000

typedef unsigned int uint32_t;
typedef unsigned long long	uint64_t;

void tzic_init(uint64_t int_bit_pattern)
{
	uint32_t mask0 = (uint32_t)(int_bit_pattern);
	uint32_t mask1 = (uint32_t)(int_bit_pattern >> 32);

	// unlock tzic
	*((volatile uint32_t *) TZIC0_LOCK) = TZIC_UNLOCK_CODE;
	*((volatile uint32_t *) TZIC1_LOCK) = TZIC_UNLOCK_CODE;

	// disable all interrupts
	*((volatile uint32_t *) TZIC0_CLEAR) = ~0U;
	*((volatile uint32_t *) TZIC1_CLEAR) = ~0U;

	// fiq generation
	*((volatile uint32_t *) TZIC0_INTSELECT) = mask0;
	*((volatile uint32_t *) TZIC1_INTSELECT) = mask1;

	// enable selected interrupts
	*((volatile uint32_t *) TZIC0_ENABLE) = mask0;
	*((volatile uint32_t *) TZIC1_ENABLE) = mask1;

	// lock tzic
	*((volatile uint32_t *) TZIC0_LOCK) = TZIC_LOCK_CODE;
	*((volatile uint32_t *) TZIC1_LOCK) = TZIC_LOCK_CODE;
}
