/*
 *  TOPPERS/SafeG Dual-OS monitor
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Safety Gate Dual-OS monitor
 *
 *  Copyright (C) 2009-2011 by Embedded and Real-Time Systems Laboratory
 *     Graduate School of Information Science, Nagoya University, JAPAN
 *
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 */
#define Asm __asm__ volatile
#define CP15_CONTROL_WRITE(x) Asm("mcr p15, 0, %0, c1, c0, 0"::"r"(x))
#define CP15_SET_VBAR(x) Asm("mcr p15, 0, %0, c12, c0, 0"::"r"(x))
#define CP15_GET_SCR(x) Asm("mrc p15, 0, %0, c1, c1, 0":"=r"(x))
#define ISB() Asm("isb")

extern void *vector_table;
extern void aipstz_init(void);
extern void tzic_init(void);
extern void uart_init(void);
extern void uart_putc(char c);
extern char uart_getc(void);
extern void uart_puts(const char *s);
extern void uart_putnum(unsigned int number, int base);

void delay(void)
{
	volatile int i;

	for (i=0; i<100000; i++);
}

void fiq_c_handler(void)
{
	char c;

	uart_puts("FIQ pressed char: ");
	c = uart_getc();
	uart_putc(c);
	uart_putc('\n');

// 	delay();
}

void exc_c_handler(void)
{
	uart_putc('E'); uart_putc('X'); uart_putc('C');
}

int main(void)
{
	unsigned int reg;

	CP15_SET_VBAR(&vector_table);
	CP15_CONTROL_WRITE(0);
	ISB();

	aipstz_init();
	uart_init();

	CP15_GET_SCR(reg);

	if (reg & 1) {
		uart_puts("\nNon secure ");
	} else {
		uart_puts("\nSecure ");
	}

	uart_puts("hello world\n");

	tzic_init();

	__asm__ volatile ("msr cpsr, #(0x13|0x80)"); // enable FIQ, IRQ disabled

	while(1) {
		__asm__ volatile ("smc #0");
	}

	return 0;
}
