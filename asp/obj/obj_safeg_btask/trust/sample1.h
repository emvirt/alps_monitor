#include "target_test.h"

#define MAIN_PRIORITY	5
#define HIGH_PRIORITY	9
#define MID_PRIORITY	10
#define LOW_PRIORITY	11

#ifndef TASK_PORTID
#define	TASK_PORTID		1
#endif /* TASK_PORTID */

#ifndef STACK_SIZE
#define	STACK_SIZE		4096
#endif /* STACK_SIZE */

#define ENABLE_TASK
// #define DISABLE_TIMER
#define DBG_VERBOSE

#ifndef TOPPERS_MACRO_ONLY

extern void	task(intptr_t exinf);
extern void	btask(intptr_t exinf);
extern void	cyclic_handler(intptr_t exinf);
extern void	main_task(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */
