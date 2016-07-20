#include "target_test.h"

#ifndef DEBUG
#define DEBUG(enable,x,args...)  \
	if(enable) syslog(LOG_NOTICE, "%s:" x, __func__ , ##args)
#endif /* DEBUG */

#ifndef TOPPERS_MACRO_ONLY

extern void	latency_initialize();

#endif /* TOPPERS_MACRO_ONLY */