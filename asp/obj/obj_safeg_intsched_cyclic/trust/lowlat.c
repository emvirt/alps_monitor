#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <t_stddef.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "lowlat.h"

#include "../../../../safeg/t_safeg_syscalls.h" /* safeg_switch_to_nt */
#include "pb1176t.h"                            /* tzic_xxxx, INTNO_BITPAT */

Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
    if (ercd < 0) {
        t_perror(LOG_ERROR, file, line, expr, ercd);
    }
}

#define SVC_PERROR(expr)    svc_perror(__FILE__, __LINE__, #expr, (expr))

/*
 * Shared variables
 */

struct shared_vars_t {
    int      nt_prio; // nt priority (-32..140)
    uint32_t ltask;   // current ltask (0..NUM_LTASKS)
} *shvars;

/*
 * LTASK management data
 */

#define NUM_LTASKS          2
#define NUM_PRI             12
#define THE_BTASK           NUM_LTASKS

ID ltask_id[NUM_LTASKS]               = {LTASK0, LTASK1};
uint32_t ntprio_to_ltask[NUM_PRI]     = {0,0,0,0,0,0,1,1,1,1,1,1};
uint32_t ltask_to_irqmask[NUM_LTASKS] = {INTNO_BITPAT(NT_INTNO_TIMER), 0};
bool_t ltask_to_overrun[NUM_LTASKS]   = {true, true};
uint32_t ltask_to_budget[NUM_LTASKS]  = {BUDGET_LTASK0_US, BUDGET_LTASK1_US};
uint32_t running_ltask = THE_BTASK;

#define ALL_LTASKS_IRQ_MASK  (INTNO_BITPAT(NT_INTNO_TIMER))

/*
 * Non-Trust interrupt handlers (all have same IPM so they are sequential)
 */

static volatile bool_t vret_initialized;

void latency_handler_softirq(void)
{
    if (!vret_initialized) return;

    if (shvars->nt_prio > (NUM_PRI - 1))
        shvars->ltask = THE_BTASK;
    else
        shvars->ltask = ntprio_to_ltask[shvars->nt_prio];

    DEBUG(false, "softirq (nt_prio:%d): set ltask=%d",
          shvars->nt_prio, shvars->ltask);

    tzic_use_fiq_gen(ALL_LTASKS_IRQ_MASK);
    SVC_PERROR(isig_sem(LATENCY_SEM));
}

void latency_handler_timer(void)
{
    DEBUG(false, "timer handler: nt_prio:%d -> -8, ltask:%d -> 0",
          shvars->nt_prio, shvars->ltask);

    tzic_pass_through_irqout(ltask_to_irqmask[0]);
    shvars->nt_prio = -NT_INTNO_TIMER;
    shvars->ltask   = 0;
    SVC_PERROR(isig_sem(LATENCY_SEM));
}

/*
 * Overrun handling  
 * Note: tick and overrun timer use same IPM so they are sequential, but
 * they have higher priority than NT interrupt handlers. That is not a
 * problem since we dont touch the shared variables here.
 */

void overrun_handler_function(ID tskid, intptr_t exinf)
{
    uint32_t ltask = (uint32_t)exinf;

    DEBUG(false, "ovr_handler (tsk:%u)", ltask);

    ltask_to_overrun[ltask] = true;
    SVC_PERROR(isig_sem(LATENCY_SEM));
}

void cyc_repl_ltask(intptr_t exinf)
{
    uint32_t ltask = (uint32_t)exinf;

    DEBUG(false, "replenishment (tsk:%u)", ltask);

    ista_ovr(ltask_id[ltask], ltask_to_budget[ltask]);

    if (ltask_to_overrun[ltask] == true) {
        ltask_to_overrun[ltask] = false;
        if (shvars->ltask != running_ltask) {
            SVC_PERROR(isig_sem(LATENCY_SEM));
        }
    }
}

void latency_manager(intptr_t exinf)
{
    uint32_t ltask;

    while(1) {
        SVC_PERROR(wai_sem(LATENCY_SEM));

        // activate LTASK
        for (ltask = shvars->ltask; ltask != THE_BTASK; ltask++) {
            if (ltask_to_overrun[ltask] == false) {
                if (ltask != running_ltask) {
                    DEBUG(false, "resume task %u", ltask);
                    SVC_PERROR(rsm_tsk(ltask_id[ltask]));
                }
                break;
            }
        }

        // suspend previous LTASK
        if ((running_ltask != THE_BTASK) && (running_ltask != ltask)) {
            DEBUG(false, "suspend task %u", running_ltask);
            SVC_PERROR(sus_tsk(ltask_id[running_ltask]));
        }

        running_ltask = ltask;
    }
}

/*
 * LTASK body
 */

void ltask(intptr_t exinf)
{
    volatile struct safeg_smc_args ret_args;

    /* loop turning to nt */
    safeg_switch_to_nt(&ret_args);

    /* stop everything */
    if ((uint32_t)exinf != BTASK) stp_ovr(ltask_id[(uint32_t)exinf]);
    tzic_pass_through_irqout(ALL_LTASKS_IRQ_MASK);
    SVC_PERROR(stp_cyc(CYC_LTASK0));
    SVC_PERROR(stp_cyc(CYC_LTASK1));
    SVC_PERROR(stp_cyc(CYC1));

    /* trace dump */
    safeg_switch_to_nt(&ret_args);
    SVC_PERROR(ext_ker());
}

/*
 * Initialization
 */

void latency_initialize()
{
    volatile struct safeg_smc_args ret_args;

    // set init flag to false (ignores NT voluntary returns)
    vret_initialized = false;

    // set NT interrupts to be handled on NT
    tzic_pass_through_irqout(ALL_LTASKS_IRQ_MASK);
    SVC_PERROR(ena_int(NT_INTNO_TIMER));

    // start NT and wait to receive the shared variables
    safeg_switch_to_nt(&ret_args);
    shvars = (struct shared_vars_t *)ret_args.arg1;

    SVC_PERROR(sus_tsk(ltask_id[0]));
    SVC_PERROR(sus_tsk(ltask_id[1]));

    SVC_PERROR(sta_cyc(CYC_LTASK0));
    SVC_PERROR(sta_cyc(CYC_LTASK1));

    // set init flag to true
    DEBUG(true, "init shvars:0x%X", shvars);
    vret_initialized = true;

    // signal the latency manager for the first time
    SVC_PERROR(sig_sem(LATENCY_SEM));
}
