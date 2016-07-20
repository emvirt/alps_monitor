/* This file is generated from prc_rename.def by genrename. */

#ifndef TOPPERS_PRC_RENAME_H
#define TOPPERS_PRC_RENAME_H

/*
 *  prc_config.c, prc_support.S
 */
#define prc_mprc_initialize			_kernel_prc_mprc_initialize
#define prc_initialize				_kernel_prc_initialize
#define prc_exit					_kernel_prc_exit
#define int_iipm_tbl				_kernel_int_iipm_tbl
#define ipr_info_tbl				_kernel_ipr_info_tbl
#define x_config_int				_kernel_x_config_int
#define iipm_disall					_kernel_iipm_disall
#define int_entry					_kernel_int_entry
#define exc_entry					_kernel_exc_entry
#define default_exc_handler			_kernel_default_exc_handler
#define default_exc_handler_entry	_kernel_default_exc_handler_entry
#define default_int_handler			_kernel_default_int_handler
#define default_int_handler_entry	_kernel_default_int_handler_entry
#define start_r						_kernel_start_r

/*
 *  kernel_cfg.c
 */
#define p_pcb_table					_kernel_p_pcb_table
#define p_vectors_table				_kernel_p_vectors_table


#ifdef TOPPERS_LABEL_ASM

/*
 *  prc_config.c, prc_support.S
 */
#define _prc_mprc_initialize		__kernel_prc_mprc_initialize
#define _prc_initialize				__kernel_prc_initialize
#define _prc_exit					__kernel_prc_exit
#define _int_iipm_tbl				__kernel_int_iipm_tbl
#define _ipr_info_tbl				__kernel_ipr_info_tbl
#define _x_config_int				__kernel_x_config_int
#define _iipm_disall				__kernel_iipm_disall
#define _int_entry					__kernel_int_entry
#define _exc_entry					__kernel_exc_entry
#define _default_exc_handler		__kernel_default_exc_handler
#define _default_exc_handler_entry	__kernel_default_exc_handler_entry
#define _default_int_handler		__kernel_default_int_handler
#define _default_int_handler_entry	__kernel_default_int_handler_entry
#define _start_r					__kernel_start_r

/*
 *  kernel_cfg.c
 */
#define _p_pcb_table				__kernel_p_pcb_table
#define _p_vectors_table			__kernel_p_vectors_table


#endif /* TOPPERS_LABEL_ASM */


#endif /* TOPPERS_PRC_RENAME_H */
