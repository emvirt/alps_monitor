#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "sample1.h"

#include "../../../../safeg/nt_safeg_syscalls.h"    /* safeg_switch_to_t() */
#include "logtrace/trace_config.h"                  /* trace_dump() */
#include "syssvc/logtask.h"                         /* target_fput_log() */
#include <sil.h>                                    /* sil_dly_nse() */
#include "lowlat.h"                                 /* latency_initialize() */

Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
    if (ercd < 0) {
        t_perror(LOG_ERROR, file, line, expr, ercd);
    }
}

#define SVC_PERROR(expr)    svc_perror(__FILE__, __LINE__, #expr, (expr))

void cyc_handler(intptr_t exinf)
{
    static int n = 0;

    SVC_PERROR(iact_tsk(NT_TASK1));
    n = (n + 1) % 2;
    if (n == 0) {
        SVC_PERROR(iact_tsk(NT_TASK2));
    }
}

void task1(intptr_t exinf)
{
    sil_dly_nse(DLY_10MS);
}

void task2(intptr_t exinf)
{
    sil_dly_nse(DLY_20MS);
}

void task3(intptr_t exinf)
{
    while(1);
}

void main_task(intptr_t exinf)
{
    // init serial
    // syslog_msk_log(0, LOG_UPTO(LOG_DEBUG));
    syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG));
    serial_opn_por(TASK_PORTID);
    serial_ctl_por(TASK_PORTID,(IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV));

    // init latency framework
    latency_initialize();

    // init tasks
    SVC_PERROR(sta_cyc(NT_CYC));

    // stop tasks after some sec
    SVC_PERROR(dly_tsk(2000));
    SVC_PERROR(stp_cyc(NT_CYC));

    // notify trace_dump phase start so that trust can stop its tasks
    safeg_switch_to_t(T_SMC_SWITCH_RET, 0);
    // trace dump
    trace_dump((intptr_t)target_fput_log);
    // notify end of trace dump
    safeg_switch_to_t(T_SMC_SWITCH_RET, 0);
}
