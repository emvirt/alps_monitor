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

#define AIPSTZ_BASE           0x53F00000
#define AISPTZ_MPROT0         (AIPSTZ_BASE + 0x0000)
#define AISPTZ_MPROT1         (AIPSTZ_BASE + 0x0004)
#define AISPTZ_OPACR0         (AIPSTZ_BASE + 0x0040)
#define AISPTZ_OPACR1         (AIPSTZ_BASE + 0x0044)
#define AISPTZ_OPACR2         (AIPSTZ_BASE + 0x0048)
#define AISPTZ_OPACR3         (AIPSTZ_BASE + 0x004C)
#define AISPTZ_OPACR4         (AIPSTZ_BASE + 0x0050)

void aipstz_init(void)
{
	unsigned int csu_reg;

	// Initialize the control registers (u-boot already does this job)
	__REG(AISPTZ_MPROT0) = 0x77777777;
	__REG(AISPTZ_MPROT1) = 0x77777777;
	__REG(AISPTZ_OPACR0) = 0x44444444;
	__REG(AISPTZ_OPACR1) = 0x44444444;
	__REG(AISPTZ_OPACR2) = 0x44444444;
	__REG(AISPTZ_OPACR3) = 0x44444444;
	__REG(AISPTZ_OPACR4) = 0x44000000;

	// set everything to non-secure experimentally
	// CSU values (use x S:0x63F9C000 in ds-5):
	// - 0x63F9C000 ~ 0x63F9C07C: 32 registers 0x00330033
	//   33 is probably the sec_config_X (for each X peripheral, so we can
	//   set 64 peripherals.
	//
	// - To discover the UART1 one, we use "x N:0x53FBC080" until it does not
	//   generate a precise abort (we set FF instead of 33)
	for(csu_reg = 0x63F9C000; csu_reg < 0x63F9C080; csu_reg = csu_reg + 4)
		__REG(csu_reg) = 0x00FF00FF;
}
