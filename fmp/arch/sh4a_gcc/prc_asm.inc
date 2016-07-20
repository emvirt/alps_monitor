#ifndef TOPPERS_TARGET_ASM_INC
#define TOPPERS_TARGET_ASM_INC

.macro my_core_index re_reg label
	mov.l _cpidr_addr_\label, \re_reg
	mov.l @\re_reg, \re_reg
.endm

.macro my_pcb re_reg sc_reg1 label
	mov.l _cpidr_addr_\label, \re_reg
	mov.l @\re_reg, \re_reg
	shll2 \re_reg
	mov.l _kernel_p_pcb_table_\label,\sc_reg1
	mov.l @(\re_reg,\sc_reg1),\re_reg
.endm

/*
 *  re_reg�ˤϡ�r0��������Ǥ��ʤ���
 *  R0���˲����Ƥ��ޤ���ǽ��������Τǡ������ƥޥ���������r0����ꤹ��
 *  �褦�ˤ��Ƥ��롥
 */
.macro my_istkpt re_reg sc_reg1 label
	mov.l _cpidr_addr_\label, \re_reg
	mov.l @\re_reg, \re_reg
	shll2 \re_reg
	mov.l _kernel_istkpt_table_\label,\sc_reg1
	mov.l @(\re_reg,\sc_reg1),\re_reg
.endm

/*
 *  re_reg�ˤϡ�r0��������Ǥ��ʤ���
 *  R0���˲����Ƥ��ޤ���ǽ��������Τǡ������ƥޥ���������r0����ꤹ��
 *  �褦�ˤ��Ƥ��롥
 */
.macro my_exch_table re_reg sc_reg1 label
	mov.l _cpidr_addr_\label, \re_reg
	mov.l @\re_reg, \re_reg
	shll2 \re_reg
	mov.l _kernel_p_exch_table_\label,\sc_reg1
	mov.l @(\re_reg,\sc_reg1),\re_reg
.endm

#endif /* TOPPERS_TARGET_ASM_INC */