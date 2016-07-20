#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "sample1.h"

/*
 *  サービスコールのエラーのログ出力
 */
Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		t_perror(LOG_ERROR, file, line, expr, ercd);
	}
}

#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))

void task(intptr_t exinf)
{
	int_t	i,j;
	static int_t	n = 0;
	int_t		tskno = (int_t) exinf;
	const char	*graph[] = { "|", "  +", "    *", "     #" };

	while(1){
		syslog(LOG_NOTICE, "task%d is running (%03d).   %s",
					tskno, ++n, graph[tskno-1]);
	
		for (i = 0; i < 1000; i++)
			for(j = 0; j < 1000; j++);
		
	}
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

	act_tsk(TASK1_1);
#if TNUM_PRCID >= 2
	act_tsk(TASK2_1);
#endif /* TNUM_PRCID >= 2 */	
#if TNUM_PRCID >= 3
	act_tsk(TASK3_1);
#endif /* TNUM_PRCID >= 3 */	
#if TNUM_PRCID >= 4
	act_tsk(TASK4_1);
#endif /* TNUM_PRCID >= 4 */	

	slp_tsk();

	syslog(LOG_NOTICE, "Sample program ends.");
	SVC_PERROR(ext_ker());
	assert(0);
}
