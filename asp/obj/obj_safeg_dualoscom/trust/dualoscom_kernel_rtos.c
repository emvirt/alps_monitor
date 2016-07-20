#include "dualoscom_config.h"
#include "dualoscom_types.h"
#include "kernel_cfg.h"
#include "pb1176t.h"

/*
 * INTERNAL TYPES
 */
typedef struct {
    ID       rtos_sem;
    uint32_t *rtos2gpos_pending;
    uint32_t *gpos2rtos_pending;
    bool_t   groups[DUALOSCOM_NUM_GROUPS];
} dualoscom_channel_event_t;

typedef struct {
    ID        rtos_sem;
    uint32_t  waiting;
} dualoscom_group_event_t;

typedef struct {
    bool_t initialized;
    dualoscom_channel_event_t channel_events[DUALOSCOM_NUM_CHANNELS];
    dualoscom_group_event_t group_events[DUALOSCOM_NUM_GROUPS];
} dualoscom_kernel_t;

/*
 * DEBUG
 */
// #define DUALOSCOM_DEBUG

#ifdef DUALOSCOM_DEBUG

static void __dualoscom_print_chevent(dualoscom_channel_event_t *ev)
{
    ER      ercd;
    T_RSEM  rsem;

    syslog(LOG_NOTICE, "rtos2gpos_pending: %u", *(ev->rtos2gpos_pending));
    syslog(LOG_NOTICE, "gpos2rtos_pending: %u", *(ev->gpos2rtos_pending));

    ercd = ref_sem(ev->rtos_sem, &rsem);
    assert(ercd == E_OK);

    if (rsem.wtskid == TSK_NONE) {
        syslog(LOG_NOTICE, "rtos_sem no task waiting");
    } else {
        syslog(LOG_NOTICE, "rtos_sem task with id %d waiting", rsem.wtskid);
    }
    syslog(LOG_NOTICE, "rtos_sem value: %d", rsem.semcnt);
}

#endif

/*
 * MODULE VARIABLES
 */
static dualoscom_kernel_t the_dualoscom_kernel;

/*
 * EVENTS HANDLER
 */
void dualoscom_handler(void)
{
    ER ercd;
    int i, j;
    dualoscom_channel_event_t *chevent;
    dualoscom_group_event_t *grevent;
    bool_t group_signaled;

    syslog(LOG_EMERG, "DualOS handler");

    if (the_dualoscom_kernel.initialized == false) {
        ercd = isig_sem(DUALOSCOM_SEM_INIT);
        assert(ercd == E_OK);
        the_dualoscom_kernel.initialized = true;
        return;
    }

    // else signal all channels with pending events
    for(i=0; i<DUALOSCOM_NUM_CHANNELS; i++) {
        chevent = &the_dualoscom_kernel.channel_events[i];
        if (*(chevent->gpos2rtos_pending) == 0) continue;

        // signal group (if a task is waiting)
        group_signaled = false;
        for(j=0; j<DUALOSCOM_NUM_GROUPS; j++) {
            if (chevent->groups[j] == false) continue;
            grevent = &the_dualoscom_kernel.group_events[j];
            if (grevent->waiting == 0) continue;
            isig_sem(grevent->rtos_sem);
            *(chevent->gpos2rtos_pending) = 0;
            group_signaled = true;
        }

        // otherwise signal the channel
        if (group_signaled == true) continue;
        isig_sem(chevent->rtos_sem);
        *(chevent->gpos2rtos_pending) = 0;
    }
}

void __dualoscom_send_irq(void)
{
    #define REALVIEW_DC1176_GIC_DIST_BASE 0x10121000 /* GIC distributor, on devchip */
    #define GIC_DIST_PENDING_SET2 (REALVIEW_DC1176_GIC_DIST_BASE + 0x204)
    static uint32_t *nt_irq = (uint32_t *)GIC_DIST_PENDING_SET2;
    *nt_irq = 2;
}

/*
 * INIT
 */
int __dualoscom_init_kernel(dualoscom_time_t timeout, void *shmem)
{
    ER ret;
    int i, j;
    dualoscom_channel_event_t *chevent;
    dualoscom_group_event_t *grevent;
    bool_t groups[DUALOSCOM_NUM_CHANNELS][DUALOSCOM_NUM_GROUPS] = DUALOSCOM_CHANNEL2GROUPS;
    ID semaphores[DUALOSCOM_NUM_CHANNELS] = DUALOSCOM_SEMAPHORES;
    ID group_semaphores[DUALOSCOM_NUM_GROUPS] = DUALOSCOM_GROUP_SEMAPHORES;

    // 1.- initialize channel events
    for(i=0; i<DUALOSCOM_NUM_CHANNELS; i++) {
        chevent = &the_dualoscom_kernel.channel_events[i];
        chevent->rtos_sem = semaphores[i];
        chevent->rtos2gpos_pending = shmem + DUALOSCOM_SHMEM_PENDING_EVENTS + i*4;
        chevent->gpos2rtos_pending = chevent->rtos2gpos_pending + DUALOSCOM_NUM_CHANNELS*4;
#ifdef DUALOSCOM_DEBUG
        __dualoscom_print_chevent(chevent);
#endif
        for(j=0; j<DUALOSCOM_NUM_GROUPS; j++) {
            chevent->groups[j] = groups[i][j];
        }
    }

    // 2.- initialize group events
    for(i=0; i<DUALOSCOM_NUM_GROUPS; i++) {
        grevent = &the_dualoscom_kernel.group_events[i];
        grevent->rtos_sem = group_semaphores[i];
        grevent->waiting  = 0;
    }

    // 3.- wait for the gpos
    the_dualoscom_kernel.initialized = false;

    ret = ena_int(NT_INTNO_SOFTIRQ);
    assert(ret == E_OK);

    ret = wai_sem(DUALOSCOM_SEM_INIT);
    assert(ret == E_OK);

    return 0;
}

// TODO: add set mask or policies para los mask

int __dualoscom_kernel_sendevent(dualoscom_channel_id_t channel_id)
{
    dualoscom_channel_event_t  *ev;

    ev = &the_dualoscom_kernel.channel_events[channel_id];
    *(ev->rtos2gpos_pending) = 1;

    __dualoscom_send_irq();

    return 0;
}

int __dualoscom_kernel_waitevent(dualoscom_channel_id_t channel_id,
                                 dualoscom_time_t timeout)
{
    dualoscom_channel_event_t  *ev;

    ev = &the_dualoscom_kernel.channel_events[channel_id];

#ifdef DUALOSCOM_DEBUG
    printf("__dualoscom_kernel_waitevent\n");
    __dualoscom_print_chevent(ev);
#endif

    wai_sem(ev->rtos_sem);

    return 0;
}

int __dualoscom_kernel_waitgroup(dualoscom_group_id_t group_id,
                                 dualoscom_time_t timeout)
{
    dualoscom_group_event_t  *gr;

    gr = &the_dualoscom_kernel.group_events[group_id];

    gr->waiting++;
    wai_sem(gr->rtos_sem);
    gr->waiting--;

    return 0;
}
