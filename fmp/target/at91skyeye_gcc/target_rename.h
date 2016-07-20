/* This file is generated from target_rename.def by genrename. */

#ifndef TOPPERS_TARGET_RENAME_H
#define TOPPERS_TARGET_RENAME_H

/*
 *  target_config.c 
 */
#define target_mprc_initialize		_kernel_target_mprc_initialize
#define target_initialize			_kernel_target_initialize
#define target_exit					_kernel_target_exit
#define x_config_int				_kernel_x_config_int
#define default_int_handler			_kernel_default_int_handler

/*
 *  target_support.S
 */
#define target_exc_handler			_kernel_target_exc_handler
#define target_ret_exc				_kernel_target_ret_exc
#define target_ret_int				_kernel_target_ret_int
#define irq_handler					_kernel_irq_handler

/*
 *  kernel_cfg.c
 */
#define p_inh_table					_kernel_p_inh_table
#define p_ipm_mask_table			_kernel_p_ipm_mask_table
#define p_inh_ipm_table				_kernel_p_inh_ipm_table
#define p_tacb_table				_kernel_p_tacb_table


#ifdef TOPPERS_LABEL_ASM

/*
 *  target_config.c 
 */
#define _target_mprc_initialize		__kernel_target_mprc_initialize
#define _target_initialize			__kernel_target_initialize
#define _target_exit				__kernel_target_exit
#define _x_config_int				__kernel_x_config_int
#define _default_int_handler		__kernel_default_int_handler

/*
 *  target_support.S
 */
#define _target_exc_handler			__kernel_target_exc_handler
#define _target_ret_exc				__kernel_target_ret_exc
#define _target_ret_int				__kernel_target_ret_int
#define _irq_handler				__kernel_irq_handler

/*
 *  kernel_cfg.c
 */
#define _p_inh_table				__kernel_p_inh_table
#define _p_ipm_mask_table			__kernel_p_ipm_mask_table
#define _p_inh_ipm_table			__kernel_p_inh_ipm_table
#define _p_tacb_table				__kernel_p_tacb_table


#endif /* TOPPERS_LABEL_ASM */

#include "arm_gcc/common/core_rename.h"

#endif /* TOPPERS_TARGET_RENAME_H */
