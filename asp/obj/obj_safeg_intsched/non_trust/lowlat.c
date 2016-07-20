#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <t_stddef.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "lowlat.h"

#include "../../../../safeg/nt_safeg_syscalls.h" /* safeg_raise_softfiq */
#include "sample1.h"                             /* MAIN_PRIORITY */

Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
    if (ercd < 0) {
        t_perror(LOG_ERROR, file, line, expr, ercd);
    }
}

#define SVC_PERROR(expr)    svc_perror(__FILE__, __LINE__, #expr, (expr))

/*
 * Global variables
 */

static volatile bool_t vret_initialized = false;

/*
 * Shared variables (check mmu.c settings)
 */

struct shared_vars_t {
    int      nt_prio; // nt priority (-32..140)
    uint32_t ltask;   // current ltask (0..NUM_LTASKS)
};

volatile struct shared_vars_t *shvars = (struct shared_vars_t *)0x6200000;

/*
 * LTASK management data
 */

#define NUM_LTASKS          2
#define NUM_PRI             12
#define THE_BTASK           NUM_LTASKS
uint32_t ntprio_to_ltask[NUM_PRI] = {0,0,0,0,0,0,1,1,1,1,1,1};

/*
 * Voluntary return hook
 */

extern uint8_t *_kernel_p_runtsk;
#define EXT_TSKPRI(x)  ((PRI)(x) + TMIN_TPRI)
#define TCB_PRIO_OFFSET 13

void safeg_task_vret_hook()
{
    uint32_t next_ltask;

    if (!vret_initialized) return;

    shvars->nt_prio = EXT_TSKPRI(*(_kernel_p_runtsk + TCB_PRIO_OFFSET));

    if (shvars->nt_prio >= NUM_PRI)
        next_ltask = THE_BTASK;
    else
        next_ltask = ntprio_to_ltask[shvars->nt_prio];

    DEBUG(false, "vret hook: nt_prio:%d ltask:%d next_ltask:%d",
          shvars->nt_prio, shvars->ltask, next_ltask);

    if (shvars->ltask != next_ltask) {
        DEBUG(false, "return to trust");
        safeg_raise_softfiq();
    }
}

/*
 * Initialization
 */

void latency_initialize()
{
    // init shared variables
    shvars->nt_prio  = MAIN_PRIORITY;
    shvars->ltask    = NUM_LTASKS;

    // give shared variables address
    safeg_switch_to_t(T_SMC_SWITCH_SHVAR, (int_t)shvars);

    // set init flag to true
    vret_initialized = true;
}
