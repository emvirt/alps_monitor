#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "sample1.h"

#include <sil.h>    /* sil_dly_nse() */
#include "lowlat.h" /* latency_initialize() */

Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
    if (ercd < 0) {
        t_perror(LOG_ERROR, file, line, expr, ercd);
    }
}

#define SVC_PERROR(expr)    svc_perror(__FILE__, __LINE__, #expr, (expr))

#define DLY_1MS 1000000

void cyc_handler(intptr_t exinf)
{
    static int n = 0;

    SVC_PERROR(iact_tsk(TASK1));
    n = (n + 1) % 2;
    if (n == 0) {
        SVC_PERROR(iact_tsk(TASK2));
        SVC_PERROR(iact_tsk(TASK3));
    }
}

void task1(intptr_t exinf)
{
    sil_dly_nse(DLY_1MS*10);
}

void task2(intptr_t exinf)
{
    sil_dly_nse(DLY_1MS*30);
}

void task3(intptr_t exinf)
{
    sil_dly_nse(DLY_1MS*80);
}

void main_task(intptr_t exinf)
{
    // open serial
#if 0
    syslog_msk_log(0, LOG_UPTO(LOG_DEBUG));
#else
    syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG));
#endif
    serial_opn_por(TASK_PORTID);
    serial_ctl_por(TASK_PORTID, (IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV));

    // init LTASKs
    latency_initialize();

    // start user tasks
    sta_cyc(CYC1);
}
