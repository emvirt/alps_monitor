#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "sample1.h"
#include "sil.h"
#include "../../../../safeg/t_safeg_syscalls.h"

Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		t_perror(LOG_ERROR, file, line, expr, ercd);
	}
}

#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))

#define PCLK_GATE               (0x7e00f034)
#define S3C64XX_PA_GPIO         (0x7F008000)
#define S3C64XX_GPM_BASE        (S3C64XX_PA_GPIO + 0x0800)
#define S3C64XX_GPMCON0         (S3C64XX_GPM_BASE + 0x00)
#define S3C64XX_GPMCON1         (S3C64XX_GPM_BASE + 0x04)
#define S3C64XX_GPMDAT          (S3C64XX_GPM_BASE + 0x08)
#define S3C64XX_GPMPUD          (S3C64XX_GPM_BASE + 0x0c)

void	default_exch_undef(void)
{
	syslog(LOG_NOTICE, "UNDEF exception!!");
	while(1){
		*((volatile uint32_t *)S3C64XX_GPMDAT) &= ~0xF;
		*((volatile uint32_t *)S3C64XX_GPMDAT) |= 0xF;
	}
}
void	default_exch_swi(void)
{
	syslog(LOG_NOTICE, "SWI exception!!");
	while(1){
		*((volatile uint32_t *)S3C64XX_GPMDAT) &= ~0xF;
		*((volatile uint32_t *)S3C64XX_GPMDAT) |= 0xF;
	}
}

void	default_exch_pabort(void)
{
	syslog(LOG_NOTICE, "PABORT exception!!");
	while(1){
		*((volatile uint32_t *)S3C64XX_GPMDAT) &= ~0xF;
		*((volatile uint32_t *)S3C64XX_GPMDAT) |= 0xF;
	}
}

void	default_exch_dabort(void *p_excinf) {
	uint32_t inst = 0;

// To obtain the data abort number change the code in the data_abort_handler
// at arch/arm_gcc/common/core_support.S and uncomment the following line
// ------------------------------------------------------------------------
// asm("mov %0, r7"::"r"(inst));
// ------------------------------------------------------------------------
// data_abort_handler:
//     /*
//      *  タスクの動作時モード(スーパーバイザーモード)へ
//      */
//     msr   cpsr, #(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET)
//     stmfd sp!, {r0-r3,ip,lr,pc} /* pcはダミー */
// 
//     /*
//      * spsrと戻り番地を取得するためにアボートモードへ
//      */
//     msr   cpsr, #(CPSR_ABT|CPSR_CPULOCK|CPSR_ALWAYS_SET)
//     mov   r0, lr
//     mov   r7, lr    <---- HERE WE SAVE THE VALUE!!
//     mrs   r1, spsr
//     mov   r2, #EXCH_NO_DABORT
//     b     target_exc_handler
// ------------------------------------------------------------------------

	syslog(LOG_NOTICE, "DABORT exception!!");
	syslog(LOG_NOTICE, "nest count: 0x%08X", *((uint32_t *)p_excinf + 0));
	syslog(LOG_NOTICE, "prio mask: 0x%08X", *((uint32_t *)p_excinf + 1));
	syslog(LOG_NOTICE, "prev CPSR: 0x%08X", *((uint32_t *)p_excinf + 2));
	syslog(LOG_NOTICE, "R0: 0x%08X", *((uint32_t *)p_excinf + 3));
	syslog(LOG_NOTICE, "R1: 0x%08X", *((uint32_t *)p_excinf + 4));
	syslog(LOG_NOTICE, "R2: 0x%08X", *((uint32_t *)p_excinf + 5));
	syslog(LOG_NOTICE, "R3: 0x%08X", *((uint32_t *)p_excinf + 6));
	syslog(LOG_NOTICE, "IP: 0x%08X", *((uint32_t *)p_excinf + 7));
	syslog(LOG_NOTICE, "LR svc: 0x%08X", *((uint32_t *)p_excinf + 8));
	syslog(LOG_NOTICE, "prev PC: 0x%08X", *((uint32_t *)p_excinf + 9));

	while(1);
}

#define CP15_DATA_SYNC_BARRIER() Asm("mcr p15, 0, %0, c7, c10, 4"::"r"(0))

void task(intptr_t exinf)
{
	static unsigned int p = 0;
	
	if (p++ % 2)
		*((volatile unsigned int *)S3C64XX_GPMDAT) = 0xFFFFFFFF;
	else
		*((volatile unsigned int *)S3C64XX_GPMDAT) = 0x00000000;

	CP15_DATA_SYNC_BARRIER();
}

void btask(intptr_t exinf)
{
	volatile struct safeg_smc_args ret_args;
	safeg_switch_to_nt(&ret_args);
	syslog(LOG_NOTICE, "BTASK should never return!");
}

void cyclic_handler(intptr_t exinf)
{
	SVC_PERROR(iact_tsk(TASK1));
}

void main_task(intptr_t exinf)
{
	ER_UINT	ercd;

#ifdef DBG_VERBOSE
	SVC_PERROR(syslog_msk_log(0, LOG_UPTO(LOG_DEBUG)));
#else
	SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
#endif
	syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);

	ercd = serial_opn_por(TASK_PORTID);
	if (ercd < 0 && MERCD(ercd) != E_OBJ) {
		syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.",
									itron_strerror(ercd), SERCD(ercd));
	}
	SVC_PERROR(serial_ctl_por(TASK_PORTID,
							(IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV)));

	*((volatile unsigned int *)PCLK_GATE)      = 0xFFFFFFFF;
	*((volatile unsigned int *)S3C64XX_GPMCON0) = 0x11111111;

#ifdef DISABLE_TIMER
	SVC_PERROR(dis_int(23));
#endif

#ifdef ENABLE_TASK
	SVC_PERROR(sta_cyc(CYCHDR1));
#endif

	SVC_PERROR(slp_tsk());

	syslog(LOG_NOTICE, "Sample program ends.");
	SVC_PERROR(ext_ker());
	assert(0);
}