#include "target_test.h"

#define NT_INT_PRIORITY     -2

#define NT_INTNO_TIMER      8   /* Linux timer(0,1): TZIC=8 GIC=40) */
#define NT_INTNO_SOFTIRQ    1

#define LMANG_PRIORITY      6
#define LTASK0_PRIORITY     7
#define LTASK1_PRIORITY     9
#define BTASK_PRIORITY      11

#define PERIOD_LTASK0_MS    150
#define PERIOD_LTASK1_MS    300

#define BUDGET_LTASK0_US    15000
#define BUDGET_LTASK1_US    25000

/* 1 (softirq) + 1 irq levels + 1 ovr handler + 2 replcyc*/
#define LATENCY_SEM_MAX     5

#ifndef STACK_SIZE
#define	STACK_SIZE		4096
#endif /* STACK_SIZE */

#ifndef DEBUG
#define DEBUG(enable,x,args...)  \
	if(enable) syslog(LOG_NOTICE, "" x, ##args)
#endif /* DEBUG */

#ifndef TOPPERS_MACRO_ONLY

extern void	latency_initialize();
extern void	ltask(intptr_t exinf);
extern void latency_manager(intptr_t exinf);
extern void latency_handler_softirq(void);
extern void latency_handler_timer(void);
extern void overrun_handler_function(ID tskid, intptr_t exinf);
extern void cyc_repl_ltask(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */
