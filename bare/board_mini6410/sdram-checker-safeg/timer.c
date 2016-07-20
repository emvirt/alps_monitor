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
#define INTNO_BITPAT(intno) (1U << intno)

#define S3C_TIMERREG(x) (0x7F006000 + (x))
#define S3C_TIMERREG2(tmr,reg) S3C_TIMERREG((reg)+0x0c+((tmr)*0x0c))

#define S3C_TCFG0   S3C_TIMERREG(0x00)
#define S3C_TCFG1   S3C_TIMERREG(0x04)
#define S3C_TCON    S3C_TIMERREG(0x08)

#define S3C_TINT_CSTAT  S3C_TIMERREG(0x44)

#define S3C_TCFG_PRESCALER0_MASK (255<<0)
#define S3C_TCFG_PRESCALER1_MASK (255<<8)
#define S3C_TCFG_PRESCALER1_SHIFT (8)
#define S3C_TCFG_PRESCALER0_SHIFT (0)
#define S3C_TCFG_DEADZONE_MASK   (255<<16)
#define S3C_TCFG_DEADZONE_SHIFT  (16)

#define S3C_TCFG1_MUX4_DIV1   (0<<16)
#define S3C_TCFG1_MUX4_DIV2   (1<<16)
#define S3C_TCFG1_MUX4_DIV4   (2<<16)
#define S3C_TCFG1_MUX4_DIV8   (3<<16)
#define S3C_TCFG1_MUX4_DIV16  (4<<16)
#define S3C_TCFG1_MUX4_TCLK1  (5<<16)
#define S3C_TCFG1_MUX4_MASK   (15<<16)
#define S3C_TCFG1_MUX4_SHIFT  (16)

#define S3C_TCFG1_MUX3_DIV1   (0<<12)
#define S3C_TCFG1_MUX3_DIV2   (1<<12)
#define S3C_TCFG1_MUX3_DIV4   (2<<12)
#define S3C_TCFG1_MUX3_DIV8   (3<<12)
#define S3C_TCFG1_MUX3_DIV16  (4<<12)
#define S3C_TCFG1_MUX3_TCLK1  (5<<12)
#define S3C_TCFG1_MUX3_MASK   (15<<12)

#define S3C_TCFG1_MUX2_DIV1   (0<<8)
#define S3C_TCFG1_MUX2_DIV2   (1<<8)
#define S3C_TCFG1_MUX2_DIV4   (2<<8)
#define S3C_TCFG1_MUX2_DIV8   (3<<8)
#define S3C_TCFG1_MUX2_DIV16  (4<<8)
#define S3C_TCFG1_MUX2_TCLK1  (5<<8)
#define S3C_TCFG1_MUX2_MASK   (15<<8)

#define S3C_TCFG1_MUX1_DIV1   (0<<4)
#define S3C_TCFG1_MUX1_DIV2   (1<<4)
#define S3C_TCFG1_MUX1_DIV4   (2<<16)
#define S3C_TCFG1_MUX1_DIV8   (3<<16)
#define S3C_TCFG1_MUX1_DIV16  (4<<16)
#define S3C_TCFG1_MUX1_TCLK1  (5<<16)
#define S3C_TCFG1_MUX1_MASK   (15<<16)

#define S3C_TCFG1_MUX0_DIV2	  (0<<0)
#define S3C_TCFG1_MUX0_DIV4	  (1<<0)
#define S3C_TCFG1_MUX0_DIV8	  (2<<0)
#define S3C_TCFG1_MUX0_DIV16  (3<<0)
#define S3C_TCFG1_MUX0_TCLK0  (4<<0)
#define S3C_TCFG1_MUX0_MASK	  (15<<0)

#define S3C_TCFG1_MUX_DIV2	  (0<<0)
#define S3C_TCFG1_MUX_DIV4	  (1<<0)
#define S3C_TCFG1_MUX_DIV8	  (2<<0)
#define S3C_TCFG1_MUX_DIV16   (3<<0)
#define S3C_TCFG1_MUX_TCLK    (4<<0)
#define S3C_TCFG1_MUX_MASK	  (15<<0)

#define S3C_TCFG1_MUX4_MASK   (15<<16)
#define S3C_TCFG1_SHIFT(x)	  ((x) * 4)

#define S3C_TCNTB(tmr)    S3C_TIMERREG2(tmr, 0x00)
#define S3C_TCMPB(tmr)    S3C_TIMERREG2(tmr, 0x04)
#define S3C_TCNTO(tmr)    S3C_TIMERREG2(tmr, (((tmr) == 4) ? 0x04 : 0x08))

#define S3C_TCON_T4RELOAD	  (1<<22)
#define S3C_TCON_T4MANUALUPD  (1<<21)
#define S3C_TCON_T4START	  (1<<20)

#define S3C_TCON_T3RELOAD	  (1<<19)
#define S3C_TCON_T3INVERT	  (1<<18)
#define S3C_TCON_T3MANUALUPD  (1<<17)
#define S3C_TCON_T3START	  (1<<16)

#define S3C_TCON_T2RELOAD	  (1<<15)
#define S3C_TCON_T2INVERT	  (1<<14)
#define S3C_TCON_T2MANUALUPD  (1<<13)
#define S3C_TCON_T2START	  (1<<12)

#define S3C_TCON_T1RELOAD	  (1<<11)
#define S3C_TCON_T1INVERT	  (1<<10)
#define S3C_TCON_T1MANUALUPD	  (1<<9)
#define S3C_TCON_T1START	  (1<<8)

#define S3C_TCON_T0DEADZONE	  (1<<4)
#define S3C_TCON_T0RELOAD	  (1<<3)
#define S3C_TCON_T0INVERT	  (1<<2)
#define S3C_TCON_T0MANUALUPD  (1<<1)
#define S3C_TCON_T0START	  (1<<0)

#define S3C_TINT_CSTAT_T4INT    (1<<9)
#define S3C_TINT_CSTAT_T3INT    (1<<8)
#define S3C_TINT_CSTAT_T2INT    (1<<7)
#define S3C_TINT_CSTAT_T1INT    (1<<6)
#define S3C_TINT_CSTAT_T0INT    (1<<5)
#define S3C_TINT_CSTAT_T4INTEN  (1<<4)
#define S3C_TINT_CSTAT_T3INTEN  (1<<3)
#define S3C_TINT_CSTAT_T2INTEN  (1<<2)
#define S3C_TINT_CSTAT_T1INTEN  (1<<1)
#define S3C_TINT_CSTAT_T0INTEN  (1<<0)

#define TIMER_FIQ0	23

#define IRQNO_TIMER							TIMER_FIQ0
#define TARGET_TIMER_PREESCALER_MASK		S3C_TCFG_PRESCALER0_MASK
#define TARGET_PREESCALER_VALUE				(0)
#define TARGET_TIMER_DIVIDER_MASK			S3C_TCFG1_MUX0_MASK
#define TARGET_TIMER_DIVIDER_VALUE			(0)
#define TARGET_TIMER_TCNTB					S3C_TCNTB(0)
#define TARGET_TIMER_CONTROL_MASK			(0xF)
#define TARGET_TIMER_CONTROL_UPDATE			(S3C_TCON_T0RELOAD|S3C_TCON_T0MANUALUPD)
#define TARGET_TIMER_INT_ENABLE				S3C_TINT_CSTAT_T0INTEN
#define TARGET_TIMER_CONTROL_START			(S3C_TCON_T0RELOAD|S3C_TCON_T0START)
#define TARGET_TIMER_TCNTO					S3C_TCNTO(0)
#define TARGET_TIMER_INT_CLEAR				S3C_TINT_CSTAT_T0INT

#define TIMER_CLOCK          66000U
#define TO_CLOCK(nume, deno) (TIMER_CLOCK * (nume) / (deno))
#define TIC_NUME     1U
#define TIC_DENO     1U

typedef unsigned int uint32_t;
typedef unsigned long long	uint64_t;

static inline uint32_t sil_rew_mem(void *mem)
{
	uint32_t	data;
	
	data = *((volatile uint32_t *) mem);
	return(data);
}

static inline void sil_wrw_mem(void *mem, uint32_t data)
{
	*((volatile uint32_t *) mem) = data;
}

static void (*timer_callback)(void);

extern void tzic_init(uint64_t int_bit_pattern);

void timer_init(void (*callback)(void))
{
	unsigned int bits;

	timer_callback = callback;

	// clear and disable the timer interrupt
	bits = sil_rew_mem((void *)S3C_TINT_CSTAT);
	bits &= ~(0x1F << 5);
	bits |= TARGET_TIMER_INT_CLEAR;
	bits &= ~TARGET_TIMER_INT_ENABLE;
	sil_wrw_mem((void *)(S3C_TINT_CSTAT), bits);
	
	// set preescaler
	bits = sil_rew_mem((void *)S3C_TCFG0);
	bits &= ~TARGET_TIMER_PREESCALER_MASK;
	bits |= TARGET_PREESCALER_VALUE;
	sil_wrw_mem((void *)(S3C_TCFG0), bits);
	
	// set the divider
	bits = sil_rew_mem((void *)S3C_TCFG1);
	bits &= ~TARGET_TIMER_DIVIDER_MASK;
	bits |= TARGET_TIMER_DIVIDER_VALUE;
	sil_wrw_mem((void *)(S3C_TCFG1), bits);
	
	// set the load for 1000 ms
	sil_wrw_mem((void *)(TARGET_TIMER_TCNTB), 10000*TO_CLOCK(TIC_NUME, TIC_DENO));
	
	// set periodic and update value
	bits = sil_rew_mem((void *)S3C_TCON);
	bits &= ~TARGET_TIMER_CONTROL_MASK;
	bits |= TARGET_TIMER_CONTROL_UPDATE;
	sil_wrw_mem((void *)(S3C_TCON), bits);
	
	// enable the timer interrupt
	bits = sil_rew_mem((void *)S3C_TINT_CSTAT);
	bits &= ~(0x1F << 5);
	bits |= TARGET_TIMER_INT_ENABLE;
	sil_wrw_mem((void *)(S3C_TINT_CSTAT), bits);
	
	// start the timer
	bits = sil_rew_mem((void *)S3C_TCON);
	bits &= ~TARGET_TIMER_CONTROL_MASK;
	bits |= TARGET_TIMER_CONTROL_START;
	sil_wrw_mem((void *)(S3C_TCON), bits);

	tzic_init(INTNO_BITPAT(TIMER_FIQ0));
	
	__asm__ volatile ("msr cpsr, #(0x13|0x80)");
}

void fiq_c_handler(void)
{
	uint32_t bits;

	// clear the interrupt bit
	bits = sil_rew_mem((void *)S3C_TINT_CSTAT);
	bits &= ~(0x1F << 5);
	bits |= TARGET_TIMER_INT_CLEAR;
	sil_wrw_mem((void *)(S3C_TINT_CSTAT), bits);

	timer_callback();
}
