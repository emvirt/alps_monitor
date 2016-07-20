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

#ifndef LOOP_REF
#define LOOP_REF		ULONG_C(1000000)
#endif /* LOOP_REF */

#ifndef TOPPERS_MACRO_ONLY

extern void	main_task(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */