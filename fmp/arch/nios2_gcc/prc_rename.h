/* This file is generated from prc_rename.def by genrename. */

#ifndef TOPPERS_PRC_RENAME_H
#define TOPPERS_PRC_RENAME_H

/*
 *  prc_config.c
 */
#define x_config_int				_kernel_x_config_int
#define int_bitmap_search_table		_kernel_int_bitmap_search_table
#define prc_initialize				_kernel_prc_initialize
#define prc_terminate				_kernel_prc_terminate
#define start_sync					_kernel_start_sync

/*
 *  prc_support.S
 */
#define exception_entry				_kernel_exception_entry
#define ret_int						_kernel_ret_int
#define ret_exc						_kernel_ret_exc
#define start_r						_kernel_start_r

/*
 * kernel_cfg.c
 */
#define p_npcb_table				_kernel_p_npcb_table
#define p_inh_table					_kernel_p_inh_table
#define p_iipm_mask_table			_kernel_p_iipm_mask_table
#define p_inh_iipm_table			_kernel_p_inh_iipm_table
#define p_exch_table				_kernel_p_exch_table

#ifdef TOPPERS_LABEL_ASM

/*
 *  prc_config.c
 */
#define _x_config_int				__kernel_x_config_int
#define _int_bitmap_search_table	__kernel_int_bitmap_search_table
#define _prc_initialize				__kernel_prc_initialize
#define _prc_terminate				__kernel_prc_terminate
#define _start_sync					__kernel_start_sync

/*
 *  prc_support.S
 */
#define _exception_entry			__kernel_exception_entry
#define _ret_int					__kernel_ret_int
#define _ret_exc					__kernel_ret_exc
#define _start_r					__kernel_start_r

/*
 * kernel_cfg.c
 */
#define _p_npcb_table				__kernel_p_npcb_table
#define _p_inh_table				__kernel_p_inh_table
#define _p_iipm_mask_table			__kernel_p_iipm_mask_table
#define _p_inh_iipm_table			__kernel_p_inh_iipm_table
#define _p_exch_table				__kernel_p_exch_table

#endif /* TOPPERS_LABEL_ASM */


#endif /* TOPPERS_PRC_RENAME_H */
