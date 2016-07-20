/* This file is generated from target_rename.def by genrename. */

#ifndef TOPPERS_TARGET_RENAME_H
#define TOPPERS_TARGET_RENAME_H


/*
 *  target_config.c
 */
#define target_mprc_initialize		_kernel_target_mprc_initialize
#define target_obj_lock_table		_kernel_target_obj_lock_table
#define target_tsk_lock_table		_kernel_target_tsk_lock_table
#define target_prc_int_base_table	_kernel_target_prc_int_base_table
#define target_fput_log_base_table	_kernel_target_fput_log_base_table
#define target_sys_clk_timer_intno_table	_kernel_target_sys_clk_timer_intno_table
#define target_sys_clk_timer_base_table	_kernel_target_sys_clk_timer_base_table

#ifdef TOPPERS_LABEL_ASM


/*
 *  target_config.c
 */
#define _target_mprc_initialize		__kernel_target_mprc_initialize
#define _target_obj_lock_table		__kernel_target_obj_lock_table
#define _target_tsk_lock_table		__kernel_target_tsk_lock_table
#define _target_prc_int_base_table	__kernel_target_prc_int_base_table
#define _target_fput_log_base_table	__kernel_target_fput_log_base_table
#define _target_sys_clk_timer_intno_table	__kernel_target_sys_clk_timer_intno_table
#define _target_sys_clk_timer_base_table	__kernel_target_sys_clk_timer_base_table

#endif /* TOPPERS_LABEL_ASM */

#include "nios2_gcc/prc_rename.h"

#endif /* TOPPERS_TARGET_RENAME_H */
