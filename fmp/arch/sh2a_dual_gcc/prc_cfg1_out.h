/*
 *  @(#) $Id$
 */

/*
 *		cfg1_out.cをリンクするために必要なスタブの定義
 */

void sta_ker(void){}
void _kernel_target_mprc_initialize(void){}

/*  スタックポインタの初期値  */
STK_T *const	_kernel_istkpt_table[TNUM_PRCID];

