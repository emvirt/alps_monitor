/* This file is generated from target_rename.def by genrename. */

#ifndef TOPPERS_TARGET_RENAME_H
#define TOPPERS_TARGET_RENAME_H

/*
 *  target_config.c, target_support.S
 */
#define inh_tbl						_kernel_inh_tbl
#define cfgint_tbl					_kernel_cfgint_tbl

#define undef_handler				_kernel_undef_handler
#define swi_handler					_kernel_swi_handler
#define prefetch_handler			_kernel_prefetch_handler
#define data_abort_handler			_kernel_data_abort_handler
#define interrupt_handler			_kernel_interrupt_handler
#define fiq_handler					_kernel_fiq_handler

/*
 *  core_config.c, core_support.S
 */
#define core_initialize				_kernel_core_initialize
#define core_terminate				_kernel_core_terminate


#ifdef TOPPERS_LABEL_ASM

/*
 *  target_config.c, target_support.S
 */
#define _inh_tbl					__kernel_inh_tbl
#define _cfgint_tbl					__kernel_cfgint_tbl

#define _undef_handler				__kernel_undef_handler
#define _swi_handler				__kernel_swi_handler
#define _prefetch_handler			__kernel_prefetch_handler
#define _data_abort_handler			__kernel_data_abort_handler
#define _interrupt_handler			__kernel_interrupt_handler
#define _fiq_handler				__kernel_fiq_handler

/*
 *  core_config.c, core_support.S
 */
#define _core_initialize			__kernel_core_initialize
#define _core_terminate				__kernel_core_terminate


#endif /* TOPPERS_LABEL_ASM */

#include "arm_gcc/common/core_rename.h"

#endif /* TOPPERS_TARGET_RENAME_H */
