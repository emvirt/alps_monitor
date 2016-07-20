#include <kernel.h>
#include <t_stdlib.h>
#include "kernel_cfg.h"
#include "sample1.h"
#include <sil.h>
#include "../../../../safeg/nt_safeg_syscalls.h"

typedef volatile unsigned long	vu_long;
typedef volatile unsigned short vu_short;
typedef volatile unsigned char	vu_char;

typedef vu_char		S3C64XX_REG8;
typedef vu_short	S3C64XX_REG16;
typedef vu_long		S3C64XX_REG32;

typedef struct {
	S3C64XX_REG32	ULCON;
	S3C64XX_REG32	UCON;
	S3C64XX_REG32	UFCON;
	S3C64XX_REG32	UMCON;
	S3C64XX_REG32	UTRSTAT;
	S3C64XX_REG32	UERSTAT;
	S3C64XX_REG32	UFSTAT;
	S3C64XX_REG32	UMSTAT;
	S3C64XX_REG8	UTXH;
	S3C64XX_REG8	res1[3];
	S3C64XX_REG8	URXH;
	S3C64XX_REG8	res2[3];
	S3C64XX_REG32	UBRDIV;
	S3C64XX_REG32	UDIVSLOT;
} S3C64XX_UART;

void cyclic_handler(intptr_t exinf)
{
	volatile S3C64XX_UART *const uart =  (S3C64XX_UART *)0x7F005000; // UART0: 0x7F005000 UART1: 0x7F005400

	while (!(uart->UTRSTAT & 0x2));
	uart->UTXH = 'c';
	
	while (!(uart->UTRSTAT & 0x2));
	uart->UTXH = 'y';
	
	while (!(uart->UTRSTAT & 0x2));
	uart->UTXH = 'c';
	
	while (!(uart->UTRSTAT & 0x2));
	uart->UTXH = 'l';
	
	while (!(uart->UTRSTAT & 0x2));
	uart->UTXH = 'i';
	
	while (!(uart->UTRSTAT & 0x2));
	uart->UTXH = 'c';
}

void main_task(intptr_t exinf)
{
	volatile S3C64XX_UART *const uart =  (S3C64XX_UART *)0x7F005000; // UART0: 0x7F005000 UART1: 0x7F005400

	uart->UFCON = 0;   // FIFO disabled
	uart->UMCON = 0;   // RTS AFC disabled
	uart->ULCON = 0x3; // 8-N-1
	uart->UCON = 0xe45;  // polling mode - external clock: DIV_VAL = (53213366¡?(115200¡?16))¡¾1=27.87
	uart->UBRDIV = 0x22; // integer part of div_val = 27
	uart->UDIVSLOT = 0x1FFF; // defined(CONFIG_UART_66)

	/* wait for room in the tx FIFO */
	while (!(uart->UTRSTAT & 0x2));
	uart->UTXH = 'h';

	while (!(uart->UTRSTAT & 0x2));
	uart->UTXH = 'e';

	while (!(uart->UTRSTAT & 0x2));
	uart->UTXH = 'l';

	while (!(uart->UTRSTAT & 0x2));
	uart->UTXH = 'l';

	while (!(uart->UTRSTAT & 0x2));
	uart->UTXH = 'o';

	while (!(uart->UTRSTAT & 0x2));
	uart->UTXH = '!';

// 	safeg_switch_to_t(T_SMC_SWITCH_RET, 0);

	sta_cyc(CYCHDR1);

	while(1);
}
