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
#define CP15_CONTROL_WRITE(x) \
    __asm__ volatile ("mcr p15, 0, %0, c1, c0, 0"::"r"(x))

#define CP15_SET_VBAR(x)    \
    __asm__ volatile ("mcr p15, 0, %0, c12, c0, 0"::"r"(x))

#define PCLK_GATE               (0x7e00f034)
#define S3C64XX_PA_GPIO         (0x7F008000)
#define S3C64XX_GPM_BASE        (S3C64XX_PA_GPIO + 0x0800)
#define S3C64XX_GPMCON0         (S3C64XX_GPM_BASE + 0x00)
#define S3C64XX_GPMCON1         (S3C64XX_GPM_BASE + 0x04)
#define S3C64XX_GPMDAT          (S3C64XX_GPM_BASE + 0x08)
#define S3C64XX_GPMPUD          (S3C64XX_GPM_BASE + 0x0c)

extern void *vector_table;
extern void uart_init(void);
extern void uart_putc(char c);
extern void timer_init(void (*callback)(void));
extern void tzpc_init(void);

void the_timer_callback(void)
{
	static unsigned int p = 0;

	if (p++ % 2)
		*((volatile unsigned int *)S3C64XX_GPMDAT) = 0xFFFFFFFF;
	else
		*((volatile unsigned int *)S3C64XX_GPMDAT) = 0x00000000;
}

void exc_c_handler(void)
{
	uart_putc('E'); uart_putc('X'); uart_putc('C');
}

int main(void)
{
	CP15_SET_VBAR(&vector_table);
	CP15_CONTROL_WRITE(0);
	uart_init();
	*((volatile unsigned int *)PCLK_GATE)      = 0xFFFFFFFF;
	*((volatile unsigned int *)S3C64XX_GPMCON0) = 0x11111111;
	timer_init(the_timer_callback);
	tzpc_init();

	while(1);

	return 0;
}
