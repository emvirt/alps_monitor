/* This file is generated from core_rename.def by genrename. */

/* This file is included only when core_rename.h has been included. */
#ifdef TOPPERS_CORE_RENAME_H
#undef TOPPERS_CORE_RENAME_H

/*
 *  start.S
 */
#undef start_sync

/*
 *  core_support.S
 */
#undef start_r
#undef ret_int
#undef ret_exc
#undef vector_table
#undef vector_ref_tbl
#undef undef_handler
#undef swi_handler
#undef prefetch_handler
#undef data_abort_handler
#undef interrupt_handler
#undef fiq_handler

/*
 *  core_config.c
 */
#undef arm_vector_ref_tbl_add
#undef arm_saved_exch_add
#undef core_initialize
#undef core_terminate
#undef xlog_sys
#undef x_install_exc

/*
 *  kernel_cfg.c
 */
#undef p_accb_table
#undef p_exch_table

#ifdef TOPPERS_LABEL_ASM

/*
 *  start.S
 */
#undef _start_sync

/*
 *  core_support.S
 */
#undef _start_r
#undef _ret_int
#undef _ret_exc
#undef _vector_table
#undef _vector_ref_tbl
#undef _undef_handler
#undef _swi_handler
#undef _prefetch_handler
#undef _data_abort_handler
#undef _interrupt_handler
#undef _fiq_handler

/*
 *  core_config.c
 */
#undef _arm_vector_ref_tbl_add
#undef _arm_saved_exch_add
#undef _core_initialize
#undef _core_terminate
#undef _xlog_sys
#undef _x_install_exc

/*
 *  kernel_cfg.c
 */
#undef _p_accb_table
#undef _p_exch_table

#endif /* TOPPERS_LABEL_ASM */


#endif /* TOPPERS_CORE_RENAME_H */
