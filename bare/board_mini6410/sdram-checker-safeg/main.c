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
 */
#define Asm __asm__ volatile
#define CP15_CONTROL_WRITE(x) Asm("mcr p15, 0, %0, c1, c0, 0"::"r"(x))
#define CP15_SET_VBAR(x) Asm("mcr p15, 0, %0, c12, c0, 0"::"r"(x))
#define CP15_PPMR_WRITE(x) Asm("mcr p15, 0, %0, c15, c2, 4"::"r"(x))
#define CP15_DCACHE_CLEAN_AND_INVALIDATE() Asm("mcr p15, 0, %0, c7, c14, 0":: "r"(0))
#define CP15_PBUFFER_FLUSH()     Asm("mcr p15, 0, %0, c7, c5, 4"::"r"(0))
#define CP15_DATA_SYNC_BARRIER() Asm("mcr p15, 0, %0, c7, c10, 4"::"r"(0))

#define PCLK_GATE               (0x7e00f034)
#define S3C64XX_PA_GPIO         (0x7F008000)
#define S3C64XX_GPM_BASE        (S3C64XX_PA_GPIO + 0x0800)
#define S3C64XX_GPMCON0         (S3C64XX_GPM_BASE + 0x00)
#define S3C64XX_GPMCON1         (S3C64XX_GPM_BASE + 0x04)
#define S3C64XX_GPMDAT          (S3C64XX_GPM_BASE + 0x08)
#define S3C64XX_GPMPUD          (S3C64XX_GPM_BASE + 0x0c)

#define TRUST_MEM_START   0x5FA00000
#define TRUST_MEM_END     0x60000000
#define TRUST_FILL        0x69696969

extern void *vector_table;
extern void uart_init(void);
extern void uart_putc(char c);
extern void timer_init(void (*callback)(void));
extern void tzpc_init(void);

void trust_memset(unsigned int *start_address,
                  unsigned int *end_address,
                  unsigned int fill)
{
	while(start_address != end_address) {
		*start_address = fill;
		start_address++;
	}
}

void trust_memcheck(unsigned int *start_address,
                    unsigned int *end_address,
                    unsigned int fill)
{
	while(start_address != end_address) {
		if (*start_address != fill) {
			uart_putc('E');
			uart_putc('R');
			uart_putc('R');
			uart_putc('O');
			uart_putc('R');
			while(1);
		}
		start_address++;
	}
	uart_putc('O');
	uart_putc('K');
	uart_putc(' ');
}

void the_timer_callback(void)
{
	static unsigned int p = 0;

	trust_memcheck((unsigned int *)TRUST_MEM_START,
				   (unsigned int *)TRUST_MEM_END,
				   (unsigned int)TRUST_FILL);

	if (p++ % 2)
		*((volatile unsigned int *)S3C64XX_GPMDAT) = 0xFFFFFFFF;
	else
		*((volatile unsigned int *)S3C64XX_GPMDAT) = 0x00000000;

	CP15_DATA_SYNC_BARRIER();
}

void exc_c_handler(void)
{
	uart_putc('E'); uart_putc('X'); uart_putc('C');
}

int main(void)
{
	CP15_SET_VBAR(&vector_table);
	CP15_PPMR_WRITE(0x70000013);        // set peripheral memory
	CP15_PBUFFER_FLUSH();               // flush prefetch buffer
	CP15_DCACHE_CLEAN_AND_INVALIDATE(); // invalidate cache
	CP15_CONTROL_WRITE(0);              // disable cache and mmu

	uart_init();
	uart_putc('T'); uart_putc('\n'); uart_putc('\r');

	*((volatile unsigned int *)PCLK_GATE)      = 0xFFFFFFFF;
	*((volatile unsigned int *)S3C64XX_GPMCON0) = 0x11111111;
	timer_init(the_timer_callback);
	tzpc_init();

	trust_memset((unsigned int *)TRUST_MEM_START,
				 (unsigned int *)TRUST_MEM_END,
				 (unsigned int)TRUST_FILL);

	while(1) {
		__asm__ volatile ("smc #0");
	}

	return 0;
}