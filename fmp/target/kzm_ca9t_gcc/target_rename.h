/* This file is generated from target_rename.def by genrename. */

#ifndef TOPPERS_TARGET_RENAME_H
#define TOPPERS_TARGET_RENAME_H

/*
 *  target_config.c
 */
#define target_mprc_initialize		_kernel_target_mprc_initialize
#define target_mmu_init				_kernel_target_mmu_init
#define target_initialize			_kernel_target_initialize
#define target_exit					_kernel_target_exit

/*
 *  ct11mpcore.c
 */
#define gic_cpu_stop				_kernel_gic_cpu_stop
#define gic_cpu_init				_kernel_gic_cpu_init
#define gic_dis_stop				_kernel_gic_dis_stop
#define gic_dis_init				_kernel_gic_dis_init
#define gic_dis_set_priority		_kernel_gic_dis_set_priority
#define gic_dis_set_pending			_kernel_gic_dis_set_pending
#define gic_dis_clear_pending		_kernel_gic_dis_clear_pending
#define gic_dis_enable_int			_kernel_gic_dis_enable_int
#define gic_dis_disable_int			_kernel_gic_dis_disable_int
#define ebtimer_init				_kernel_ebtimer_init
#define gic_dis_config				_kernel_gic_dis_config
#define gic_dis_probe_int			_kernel_gic_dis_probe_int
#define ct11mpcore_uart_init		_kernel_ct11mpcore_uart_init
#define ct11mpcore_putc				_kernel_ct11mpcore_putc


#ifdef TOPPERS_LABEL_ASM

/*
 *  target_config.c
 */
#define _target_mprc_initialize		__kernel_target_mprc_initialize
#define _target_mmu_init			__kernel_target_mmu_init
#define _target_initialize			__kernel_target_initialize
#define _target_exit				__kernel_target_exit

/*
 *  ct11mpcore.c
 */
#define _gic_cpu_stop				__kernel_gic_cpu_stop
#define _gic_cpu_init				__kernel_gic_cpu_init
#define _gic_dis_stop				__kernel_gic_dis_stop
#define _gic_dis_init				__kernel_gic_dis_init
#define _gic_dis_set_priority		__kernel_gic_dis_set_priority
#define _gic_dis_set_pending		__kernel_gic_dis_set_pending
#define _gic_dis_clear_pending		__kernel_gic_dis_clear_pending
#define _gic_dis_enable_int			__kernel_gic_dis_enable_int
#define _gic_dis_disable_int		__kernel_gic_dis_disable_int
#define _ebtimer_init				__kernel_ebtimer_init
#define _gic_dis_config				__kernel_gic_dis_config
#define _gic_dis_probe_int			__kernel_gic_dis_probe_int
#define _ct11mpcore_uart_init		__kernel_ct11mpcore_uart_init
#define _ct11mpcore_putc			__kernel_ct11mpcore_putc


#endif /* TOPPERS_LABEL_ASM */

#include "arm_gcc/mpcore/chip_rename.h"

#endif /* TOPPERS_TARGET_RENAME_H */
