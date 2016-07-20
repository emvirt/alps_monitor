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
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

/*
 * VIC interrupt controller
 */

#define VIC_MAX_PRIORITY   15
#define VIC_MIN_PRIORITY    0
#define VIC_TMIN_INTNO      0U
#define VIC_TMAX_INTNO     63U
#define VIC_TNUM_INT       64U

#define S3C64XX_VIC0                (0x71200000)
#define S3C64XX_VIC1                (0x71300000)

#define S3C_VIC0REG(x)              ((x) + S3C64XX_VIC0)
#define S3C_VIC1REG(x)              ((x) + S3C64XX_VIC1)

#define S3C64XX_VIC0IRQSTATUS       S3C_VIC0REG(0x000)
#define S3C64XX_VIC1IRQSTATUS       S3C_VIC1REG(0x000)

#define S3C64XX_VIC0FIQSTATUS       S3C_VIC0REG(0x004)
#define S3C64XX_VIC1FIQSTATUS       S3C_VIC1REG(0x004)

#define S3C64XX_VIC0RAWINTR         S3C_VIC0REG(0x008)
#define S3C64XX_VIC1RAWINTR         S3C_VIC1REG(0x008)

#define S3C64XX_VIC0INTSELECT       S3C_VIC0REG(0x00C)
#define S3C64XX_VIC1INTSELECT       S3C_VIC1REG(0x00C)

#define S3C64XX_VIC0INTENABLE       S3C_VIC0REG(0x010)
#define S3C64XX_VIC1INTENABLE       S3C_VIC1REG(0x010)

#define S3C64XX_VIC0INTENCLEAR      S3C_VIC0REG(0x014)
#define S3C64XX_VIC1INTENCLEAR      S3C_VIC1REG(0x014)

#define S3C64XX_VIC0SOFTINT         S3C_VIC0REG(0x018)
#define S3C64XX_VIC1SOFTINT         S3C_VIC1REG(0x018)

#define S3C64XX_VIC0SOFTINTCLEAR    S3C_VIC0REG(0x01C)
#define S3C64XX_VIC1SOFTINTCLEAR    S3C_VIC1REG(0x01C)

#define S3C64XX_VIC0PROTECTION      S3C_VIC0REG(0x020)
#define S3C64XX_VIC1PROTECTION      S3C_VIC1REG(0x020)

#define S3C64XX_VIC0SWPRIORITYMASK  S3C_VIC0REG(0x024)
#define S3C64XX_VIC1SWPRIORITYMASK  S3C_VIC1REG(0x024)

#define S3C64XX_VIC0PRIORITYDAISY   S3C_VIC0REG(0x028)
#define S3C64XX_VIC1PRIORITYDAISY   S3C_VIC1REG(0x028)

#define S3C64XX_VIC0VECTADDR0       S3C_VIC0REG(0x100)
#define S3C64XX_VIC1VECTADDR0       S3C_VIC1REG(0x100)

#define S3C64XX_VIC0VECTPRIORITY0   S3C_VIC0REG(0x200)
#define S3C64XX_VIC1VECTPRIORITY0   S3C_VIC1REG(0x200)

#define S3C64XX_VIC0ADDRESS         S3C_VIC0REG(0xF00)
#define S3C64XX_VIC1ADDRESS         S3C_VIC1REG(0xF00)

void vic_init(uint64_t int_bit_pattern)
{
	uint32_t mask0 = (uint32_t)(int_bit_pattern);
	uint32_t mask1 = (uint32_t)(int_bit_pattern >> 32);

	// disable all interrupts
	*((volatile uint32_t *)S3C64XX_VIC0INTENCLEAR) = (uint32_t)~0;
	*((volatile uint32_t *)S3C64XX_VIC1INTENCLEAR) = (uint32_t)~0;

	// set protected (privileged access only) mode
	*((volatile uint32_t *)S3C64XX_VIC0PROTECTION) = (uint32_t)1;
	*((volatile uint32_t *)S3C64XX_VIC1PROTECTION) = (uint32_t)1;

	// set interrupts to generate IRQ requests (instead of FIQ requests)
	*((volatile uint32_t *)S3C64XX_VIC0INTSELECT) = (uint32_t)0;
	*((volatile uint32_t *)S3C64XX_VIC1INTSELECT) = (uint32_t)0;

	// enable selected interrupts
	*((volatile uint32_t *)S3C64XX_VIC0INTENABLE) = mask0;
	*((volatile uint32_t *)S3C64XX_VIC1INTENABLE) = mask1;
}
