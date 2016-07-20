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

#define EXCH_NO_UNDEF     1
#define EXCH_NO_SWI       2
#define EXCH_NO_PABORT    3
#define EXCH_NO_DABORT    4

#define ENABLE_TASK
// #define DISABLE_TIMER
#define DBG_VERBOSE

#ifndef TOPPERS_MACRO_ONLY

extern void	task(intptr_t exinf);
extern void	btask(intptr_t exinf);
extern void	cyclic_handler(intptr_t exinf);
extern void	main_task(intptr_t exinf);
extern void	default_exch_undef(void);
extern void	default_exch_swi(void);
extern void	default_exch_pabort(void);
extern void	default_exch_dabort(void *p_excinf);

#endif /* TOPPERS_MACRO_ONLY */
