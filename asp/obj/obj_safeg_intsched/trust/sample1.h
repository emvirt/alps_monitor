#include "target_test.h"

#define MAIN_PRIORITY   4
#define TASK1_PRIORITY  4
#define TASK2_PRIORITY  8
#define TASK3_PRIORITY  10

#define CYC_PERIOD_MS   100

#ifndef TASK_PORTID
#define TASK_PORTID     1
#endif /* TASK_PORTID */

#ifndef STACK_SIZE
#define STACK_SIZE      4096
#endif /* STACK_SIZE */

#ifndef DEBUG
#define DEBUG(enable,x,args...)  \
    if(enable) syslog(LOG_NOTICE, "%s:" x, __func__ , ##args)
#endif /* DEBUG */

#ifndef TOPPERS_MACRO_ONLY

extern void main_task(intptr_t exinf);
extern void task1(intptr_t exinf);
extern void task2(intptr_t exinf);
extern void task3(intptr_t exinf);
extern void cyc_handler(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */
