/* This file is generated from target_rename.def by genrename. */

/* This file is included only when target_rename.h has been included. */
#ifdef TOPPERS_TARGET_RENAME_H
#undef TOPPERS_TARGET_RENAME_H

/*
 *  target_config.c, target_support.S
 */
#undef inh_tbl
#undef cfgint_tbl

#undef undef_handler
#undef swi_handler
#undef prefetch_handler
#undef data_abort_handler
#undef interrupt_handler
#undef fiq_handler

/*
 *  core_config.c, core_support.S
 */
#undef core_initialize
#undef core_terminate


#ifdef TOPPERS_LABEL_ASM

/*
 *  target_config.c, target_support.S
 */
#undef _inh_tbl
#undef _cfgint_tbl

#undef _undef_handler
#undef _swi_handler
#undef _prefetch_handler
#undef _data_abort_handler
#undef _interrupt_handler
#undef _fiq_handler

/*
 *  core_config.c, core_support.S
 */
#undef _core_initialize
#undef _core_terminate


#endif /* TOPPERS_LABEL_ASM */

#include "arm_gcc/common/core_unrename.h"

#endif /* TOPPERS_TARGET_RENAME_H */
