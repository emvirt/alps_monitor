#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "sample1.h"
#include "../../../../safeg/t_safeg_syscalls.h"
#include "pb1176t.h"
#include "dualoscom.h"

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
	int_t	i;
	static int_t	n = 0;
	int_t		tskno = (int_t) exinf;
	const char	*graph[] = { "|", "  +", "    *" };

	syslog(LOG_NOTICE, "task%d is running (%03d).   %s",
				tskno, ++n, graph[tskno-1]);
	for(i=0; i<10000; i++);
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

bool_t filter_dummy(void *buffer, uint32_t size)
{
    return true;
}

bool_t filter_even(void *buffer, uint32_t size)
{
    uint32_t value;

    value = *((uint32_t *)buffer);

    if ((value % 2) == 0)  return true;
    return false;
}

static void *memcpy(void *dest, const void *src, size_t count) {
    char *dst8 = (char *)dest;
    char *src8 = (char *)src;

    while (count--) *dst8++ = *src8++;
    return dest;
}

void main_task(intptr_t exinf)
{
	ER_UINT	ercd;
    int ret;
    dualoscom_block_id_t block_id;
    void *buffer;
    uint32_t size;
    char msg[] = "hello world";

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
#ifdef ENABLE_TASK
	SVC_PERROR(sta_cyc(CYCHDR1));
#endif

#ifdef DISABLE_TIMER
	SVC_PERROR(dis_int(IRQNO_TIMER));
#endif

    syslog(LOG_NOTICE, "RTOS dualoscom_init");
    ret = dualoscom_init(true, 0);
    assert(ret == 0);

    syslog(LOG_NOTICE, "Dual-OS communications initialized");

    syslog(LOG_NOTICE, "RTOS send message\n");

    ret = dualoscom_block_alloc(DUALOSCOM_CHANNEL_0, &block_id);
    assert(ret == 0);

    ret = dualoscom_block_getbuffer(block_id, &buffer, &size);
    assert(ret == 0);

    assert(sizeof(msg) < size);
    memcpy(buffer, (void *)msg, sizeof(msg));

    ret = dualoscom_block_enqueue(block_id);
    syslog(LOG_NOTICE, "ret %d", ret);
    assert(ret == 0);

    ret = dualoscom_channel_event_send(DUALOSCOM_CHANNEL_0);
    assert(ret == 0);

    syslog(LOG_NOTICE, "RTOS wait for ack");
    ret = dualoscom_channel_event_wait(DUALOSCOM_CHANNEL_0, 0);
    assert(ret == 0);

    syslog(LOG_NOTICE, "RTOS send params 3 and 5");

    *((uint32_t *)buffer) = 3;
    *((uint32_t *)(buffer + 4)) = 5;

    ret = dualoscom_channel_event_send(DUALOSCOM_CHANNEL_0);
    assert(ret == 0);

    syslog(LOG_NOTICE, "RTOS wait result");
    ret = dualoscom_channel_event_wait(DUALOSCOM_CHANNEL_0, 0);
    assert(ret == 0);

    syslog(LOG_NOTICE, "RTOS rpc result: 3 x 5 = %u", *((uint32_t *)buffer));

    syslog(LOG_NOTICE, "RTOS send params 4 and 6");

    *((uint32_t *)buffer) = 4;
    *((uint32_t *)(buffer + 4)) = 6;

    ret = dualoscom_channel_event_send(DUALOSCOM_CHANNEL_0);
    assert(ret == 0);

    syslog(LOG_NOTICE, "RTOS wait result");
    ret = dualoscom_group_event_wait(DUALOSCOM_GROUP_0, 0);
    assert(ret == 0);

    syslog(LOG_NOTICE, "RTOS rpc result: 4 x 6 = %u", *((uint32_t *)buffer));

    syslog(LOG_NOTICE, "RTOS for sentence from GPOS in channel 1");
    ret = dualoscom_channel_event_wait(DUALOSCOM_CHANNEL_1, 0);
    assert(ret == 0);

    ret = dualoscom_block_dequeue(DUALOSCOM_CHANNEL_1, &block_id);
    assert(ret == 0);

    ret = dualoscom_block_getbuffer(block_id, &buffer, &size);
    assert(ret == 0);

    syslog(LOG_NOTICE, "MSG: %s", (char *)buffer);

    syslog(LOG_NOTICE, "RTOS dualoscom_block_free");
    ret = dualoscom_block_free(block_id);
    assert(ret == 0);

    syslog(LOG_NOTICE, "RTOS wait for EVEN messages");
    ret = dualoscom_filter_set(DUALOSCOM_CHANNEL_0, DUALOSCOM_FILTER_ID_EVEN);
    assert(ret == 0);

    ret = dualoscom_channel_event_wait(DUALOSCOM_CHANNEL_0, 0);
    assert(ret == 0);

    while(1) {
        ret = dualoscom_block_dequeue(DUALOSCOM_CHANNEL_0, &block_id);
        if (ret == DUALOSCOM_EMPTY) break;
        assert(ret == 0);

        ret = dualoscom_block_getbuffer(block_id, &buffer, &size);
        assert(ret == 0);

        syslog(LOG_NOTICE, "number: %u", *((uint32_t *)buffer));

        ret = dualoscom_block_free(block_id);
        assert(ret == 0);
    }

	SVC_PERROR(slp_tsk());

	SVC_PERROR(ext_ker());
	assert(0);
}
