/* This file is generated from target_rename.def by genrename. */

/* This file is included only when target_rename.h has been included. */
#ifdef TOPPERS_TARGET_RENAME_H
#undef TOPPERS_TARGET_RENAME_H

/*
 *  target_config.c
 */
#undef target_mprc_initialize
#undef target_mmu_init
#undef target_initialize
#undef target_exit

/*
 *  ct11mpcore.c
 */
#undef gic_cpu_stop
#undef gic_cpu_init
#undef gic_dis_stop
#undef gic_dis_init
#undef gic_dis_set_priority
#undef gic_dis_set_pending
#undef gic_dis_clear_pending
#undef gic_dis_enable_int
#undef gic_dis_disable_int
#undef ebtimer_init
#undef gic_dis_config
#undef gic_dis_probe_int
#undef ct11mpcore_uart_init
#undef ct11mpcore_putc


#ifdef TOPPERS_LABEL_ASM

/*
 *  target_config.c
 */
#undef _target_mprc_initialize
#undef _target_mmu_init
#undef _target_initialize
#undef _target_exit

/*
 *  ct11mpcore.c
 */
#undef _gic_cpu_stop
#undef _gic_cpu_init
#undef _gic_dis_stop
#undef _gic_dis_init
#undef _gic_dis_set_priority
#undef _gic_dis_set_pending
#undef _gic_dis_clear_pending
#undef _gic_dis_enable_int
#undef _gic_dis_disable_int
#undef _ebtimer_init
#undef _gic_dis_config
#undef _gic_dis_probe_int
#undef _ct11mpcore_uart_init
#undef _ct11mpcore_putc


#endif /* TOPPERS_LABEL_ASM */

#include "arm_gcc/mpcore/chip_unrename.h"

#endif /* TOPPERS_TARGET_RENAME_H */
