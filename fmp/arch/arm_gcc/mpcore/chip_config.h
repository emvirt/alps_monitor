/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2010 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ�
 *      ���դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 * 
 *  @(#) $Id: chip_config.h 553 2010-03-16 02:51:56Z ertl-honda $
 */

/*
 *  ���åװ�¸�⥸�塼���MPCORE�ѡ�
 *
 *  �����ͥ�Υ��åװ�¸���Υ��󥯥롼�ɥե����롥kernel_impl.h�Υ���
 *  ���åȰ�¸���ΰ����դ��Ȥʤ롥
 */

#ifndef TOPPERS_CHIP_CONFIG_H
#define TOPPERS_CHIP_CONFIG_H

/*
 *  ���åװ�¸���ΤΥϡ��ɥ������񸻤����
 */
#include "mpcore.h"

#ifdef USE_IPI_DIS_HANDER_BYPASS
#define OMIT_KER_REQ_ON_IPI

#ifndef TOPPERS_MACRO_ONLY
/*
 *  �����ͥ뽪λ�����׵�
 */
extern void ext_ker_request(void);
#endif /* TOPPERS_MACRO_ONLY */
#endif /* USE_IPI_DIS_HANDER_BYPASS */

/*
 *  �٥������롼����򥫡��ͥ�ǻ��Ĥ������
 */
#define VECTOR_KERNEL

/*
 *  ������ֹ�Υޥ����ʲ���16bit�Τߤ�ͭ����
 */
#define INTNO_MASK(intno) (intno & 0xffffU)

/*
 *  ����ߥϥ�ɥ��ֹ�Υޥ����ʲ���16bit�Τߤ�ͭ����
 */
#define INHNO_MASK(intno) (intno & 0xffffU)

/*
 *  ����ߥϥ�ɥ��ֹ�ȳ�����ֹ�˴ؤ������
 */
#define TMIN_INTNO  DIC_TMIN_INTNO
#define TMAX_INTNO  DIC_TMAX_INTNO
#define TNUM_INT    DIC_TNUM_INT

#define TMIN_INHNO  DIC_TMIN_INTNO
#define TMAX_INHNO  DIC_TMAX_INTNO
#define TNUM_INH    DIC_TNUM_INT

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ���ץ��å���APCB�ؤΥݥ��󥿤μ���
 *  ���󥯥롼�ɽ���δط��塤target_pcb.h �ˤϵ��ҤǤ��ʤ����ᤳ����������롥
 */
Inline APCB*
get_my_p_apcb(void)
{
	return &((get_my_p_tpcb())->arm_pcb);
}

/*
 *  �ޥ����ץ��å������֤�
 */
Inline bool_t
x_sense_mprc(void){
	if(x_prc_index() == (TOPPERS_MASTER_PRCID - 1)) {
		return true;
	}
	else {
		return false;
	}
}

/*
 *  ��å��ط�
 */

/*
 *  ���㥤����ȥ�å��ν������G_LOCK�ξ���
 */
Inline void
x_initialize_giant_lock(LOCK *p_giant_lock)
{
	*p_giant_lock = 0;
}

/*
 *  ��������å��ν����
 */
Inline void
x_initialize_tsk_lock(LOCK *p_tsk_lock)
{
	*p_tsk_lock = 0;
}

/*
 *  ���֥������ȥ�å��ν����
 */
Inline void
x_initialize_obj_lock(LOCK *p_obj_lock)
{
	*p_obj_lock = 0;
}

/*
 *  ������ֹ���ϰϤ�Ƚ��
 */
#define VALID_INTNO(intno) ((TMIN_INTNO <= INTNO_MASK(intno)) && (INTNO_MASK(intno) <= TMAX_INTNO))
#define VALID_INTNO_DISINT(intno)    VALID_INTNO(intno)
#define VALID_INTNO_CFGINT(intno)    VALID_INTNO(intno)
#define VALID_INTNO_ATTISR(intno)    VALID_INTNO(intno)

/*
 *  ����ߥϥ�ɥ����Ͽ�ѥơ��֥��kernel_cfg.c��
 */
extern const FP* const p_inh_table[TNUM_PRCID];

/*
 *  ����ߥϥ�ɥ������
 * 
 *  ����ߥϥ�ɥ��ֹ�inhno�γ���ߥϥ�ɥ�ε�ư���Ϥ�inthdr�����ꤹ��
 */
Inline void
x_define_inh(INHNO inhno, FP int_entry, uint_t affinity_mask)
{
	assert(((TMIN_INHNO <= INHNO_MASK(inhno)) && (INHNO_MASK(inhno) <= TMAX_INHNO)));
}

/*
 *  ����ߥϥ�ɥ�ν����������������ޥ���
 *
 */
#define INT_ENTRY(inhno, inthdr)    inthdr
#define INTHDR_ENTRY(inhno, inhno_num, inthdr)

#endif /* TOPPERS_MACRO_ONLY */  

/*
 *  �����ͥ���٥ޥ������饤�֥��
 *
 *  DIC�Ǥϡ�DIC���ͥ���٥쥸������CPU�γ����ͥ���٥ޥ�����IPM�ˤ����ꤹ�롥  
 *  �����ͥ���٥ޥ������ͤ��礭���ۤ���ͥ���٤Ǥ��롥
 *  �����ͥ���٥ޥ����Ϥ��ʳ���ARM11��CA9�ǰۤʤ롥
 *    ��ARM11(ARMv6) : 16�ʳ���0x00 �� 0x0f��
 *    ��CA9(ARMv7)   : 32�ʳ���0x00 �� 0x1f��  
 *  IPM����¸���Ƥ�������ˡ������ͥ���٤γ���ɽ����-1����Ϣ³��������͡�
 *  ��Ȥ����Ȥ��ǽ�Ǥ��뤬��;�פʷ׻���ɬ�פˤʤ롥������򤱤뤿�ᡤIPM��
 *  ��¸������ˤϡ�DIC�ǰ���ͥ���٤η����Ȥ��롥�����ͤ�����ͥ���٥ޥ���
 *  ������ɽ���ȸƤӡ�IIPM�Ƚ񤯤��Ȥˤ��롥 
 */

/*
 *  �����ͥ���٥ޥ����Υӥåȥޥ���
 */
#if __TARGET_ARCH_ARM == 6
#ifndef TOPPERS_MACRO_ONLY
#define IPM_BIT_MASK  0x0fU
#else /* TOPPERS_MACRO_ONLY */
#define IPM_BIT_MASK  0x0f
#endif /* TOPPERS_MACRO_ONLY */
#define IPM_BIT_OFFSET 4
#elif __TARGET_ARCH_ARM == 7
#ifndef TOPPERS_MACRO_ONLY
#define IPM_BIT_MASK  0x1fU
#else /* TOPPERS_MACRO_ONLY */
#define IPM_BIT_MASK  0x1f
#endif /* TOPPERS_MACRO_ONLY */
#define IPM_BIT_OFFSET 3
#endif /* __TARGET_ARCH_ARM == 7 */

/*
 *  �����ͥ���٥ޥ����γ���ɽ��������ɽ�����Ѵ�
 *
 *  ������֥����Υ������ե����뤫�饤�󥯥롼�ɤ�����Τ���ˡ���
 *  ���㥹�Ȥ��ʤ�������Ѱդ��Ƥ��롥
 */
#ifndef TOPPERS_MACRO_ONLY
#define EXT_IPM(iipm)    ((PRI)(iipm - IPM_BIT_MASK))    /* ����ɽ������ɽ���� */
#define INT_IPM(ipm)    ((uint8_t)(ipm + IPM_BIT_MASK))  /* ����ɽ��������ɽ���� */
#else /* TOPPERS_MACRO_ONLY */
#define EXT_IPM(iipm)    (iipm - IPM_BIT_MASK)           /* ����ɽ������ɽ���� */
#define INT_IPM(ipm)    (ipm + IPM_BIT_MASK)             /* ����ɽ��������ɽ���� */
#endif /* TOPPERS_MACRO_ONLY */

/*
 *  TIPM_ENAALL�ʳ����ͥ���٥ޥ���������ˤ�����ɽ��
 */
#define IIPM_ENAALL        (INT_IPM(TIPM_ENAALL))

#ifndef TOPPERS_MACRO_ONLY

/*
 *  IPM�ʥϡ��ɥ������γ����ͥ���٥ޥ���������ɽ���ˤθ����ͤ��ɽФ�
 */
Inline uint8_t
current_iipm(void)
{
	return(dic_cpu_current_priority());
}

/*
 *  IPM�ʥϡ��ɥ������γ����ͥ���٥ޥ���������ɽ���ˤθ����ͤ�����
 */
Inline void
set_iipm(uint8_t iipm)
{
	dic_cpu_set_priority(iipm);
}

/*
 * (��ǥ���)�����ͥ���٥ޥ���������
 */
Inline void
x_set_ipm(PRI intpri)
{
	set_iipm(INT_IPM(intpri));
}

#define t_set_ipm(intpri) x_set_ipm(intpri)
#define i_set_ipm(intpri) x_set_ipm(intpri)

/*
 *  (��ǥ���)�����ͥ���٥ޥ����λ���
 */
Inline PRI
x_get_ipm(void)
{
	// return(EXT_IPM(current_iipm()));
	return TIPM_ENAALL;
}

#define t_get_ipm() x_get_ipm()
#define i_get_ipm() x_get_ipm()

/*
 *  �����°�����������ơ��֥�
 *  
 *  �����°���ơ��֥�ؤΥݥ��󥿡������°���ơ��֥�ϡ������°��������
 *  ����Ƥ����"1"�����ꤵ��Ƥ��ʤ����"0"�Ȥʤ�
 */
extern const uint8_t* const p_cfgint_table[];

#define my_cfgint_table (p_cfgint_table[x_prc_index()])

/*
 * �ʥ�ǥ��Ρ˳�����׵�ػߥե饰�Υ��å�
 *
 *  ���ꤵ�줿��������ֹ�γ�����׵�ػߥե饰�Υ��åȤ��ơ�����ߤ�
 *  �ػߤ��롥
 *    
 *  �����°�������ꤵ��Ƥ��ʤ�������׵�饤����Ф��Ƴ�����׵�ػ�
 *  �ե饰�򥯥ꥢ���褦�Ȥ������ˤϡ�false���֤���  
 */
Inline bool_t
x_disable_int(INTNO intno)
{
	if (my_cfgint_table[INTNO_MASK(intno)] == 0){
		return(false);
	}
    
	dic_disable_int(INTNO_MASK(intno));
	return(true);
}

#define t_disable_int(intno)  x_disable_int(intno)
#define i_disable_int(intno)  t_disable_int(intno)

/* 
 *  (��ǥ���)����׵�ػߥե饰�β��
 *
 *  ���ꤵ�줿��������ֹ�γ�����׵�ػߥե饰�Υ��ꥢ���ơ�����ߤ�
 *  ���Ĥ��롥
 *
 *  �����°�������ꤵ��Ƥ��ʤ�������׵�饤����Ф��Ƴ�����׵�ػ�
 *  �ե饰�򥯥ꥢ���褦�Ȥ������ˤϡ�false���֤���
 */
Inline bool_t
x_enable_int(INTNO intno)
{
	if (my_cfgint_table[INTNO_MASK(intno)] == 0){
		return(false);
	}
    
	dic_enable_int(INTNO_MASK(intno));
	return(true);
}

#define t_enable_int(intno) x_enable_int(intno)
#define i_enable_int(intno) x_enable_int(intno)

/*
 * ������׵�Υ��ꥢ
 */
Inline void
x_clear_int(INTNO intno)
{
	dic_clear_pending(INTNO_MASK(intno));
}

#define t_clear_int(intno) x_clear_int(intno) 
#define i_clear_int(intno) x_clear_int(intno) 

/*
 *  ������׵�Υ����å�
 */
Inline bool_t
x_probe_int(INTNO intno)
{
	return(dic_probe_int(INTNO_MASK(intno)));
}

#define t_probe_int(intno) x_probe_int(intno)
#define i_probe_int(intno) x_probe_int(intno)

/*
 *  ������׵�饤���°��������
 *
 */
extern void x_config_int(INTNO intno, ATR intatr, PRI intpri, uint_t affinity_mask);

/*
 * ����ߥϥ�ɥ���������ɬ�פ�IRC���
 *
 */
Inline void
i_begin_int(INTNO intno)
{    
}

/*
 * ����ߥϥ�ɥ�νи���ɬ�פ�IRC���
 */
Inline void
i_end_int(INTNO intno)
{
}

/*
 *  str_ker() �����ǥޥ����ץ��å��ǹԤ������
 */
extern void chip_mprc_initialize(void);

/*
 *  ���åװ�¸�ν����
 */
extern void chip_initialize(void);

/*
 *  ���åװ�¸���ν�λ
 *
 *  �����ƥ��λ������˻Ȥ���
 */
extern void chip_exit(void);

/*
 *  ����ߥϥ�ɥ��chip_support.S��
 */
extern void interrupt_handler(void);

/*
 *  ̤����γ���ߤ����ä����ν���
 */
extern void default_int_handler(void);


#endif /* TOPPERS_MACRO_ONLY */

/*
 *  ARM��¸�⥸�塼��
 */
#include "arm_gcc/common/core_config.h"

#ifndef TOPPERS_MACRO_ONLY
/*
 *  ��å����������ؿ�
 *  x_lock_cpu()/x_unlock_cpu()��Ȥ����ᡤ������¸�⥸�塼���ARM�ѡˤ�
 *  �ɤ߹���Ǥ���������롥 
 */

/*
 *  ��å��������Ի��γ�����Ԥ�
 *
 *  MPCore�Ǥϡ���å��������Ի��˥��ԥ�򷫤��֤��ΤǤϤʤ���wfe�ˤ�ꡤ
 *  ���٥���Ԥ��Ȥ��뤳�Ȥ��侩����Ƥ��롥
 *  wfe�ˤ�륤�٥���Ԥ��κݡ�����߱��������㲼�����ʤ����ᡤ����ߤ��
 *  �Ĥ������֤ơ����٥���Ԥ��Ȥ���٤��Ǥ��롥wfe̿�Ἣ���ϡ�����ߤ��
 *  �ߡ����Ĥ����ʤ����ᡤ����msr̿��ˤ�����ߤ���Ĥ��롥
 *
 *  ���κݡ�msr�¹Ԥˤ�����ߤ����Ĥ��줿ľ��(wfe��¹Ԥ�������)�˳��
 *  �ߤ����ꡤ���θ����ߥϥ�ɥ餫��Υ꥿������wfe�ˤʤꡤ���٥�Ȥ�
 *  ƨ����ǽ�����ͤ����롥
 *
 *  �������ʤ��顤 ����ߥϥ�ɥ餫��Υ꥿����(���Τˤ��㳰���̤���Υ꥿
 *  ����)��Ԥ��ȡ����٥�ȥ쥸���������åȤ��졤wfe�¹Ի��˥��٥�ȥ쥸��
 *  �������åȤ���Ƥ���ȡ����ꥢ����wfe����¨�¤˥꥿���󤹤뤿�������
 *  �ʤ��� 
 */
  
/*
 *  1���ܤΥ�å������ʥ��������󥿥�������ƥ����ȶ��ѡ�
 */
Inline void
x_acquire_lock(LOCK *p_lock)
{
	int		locked;

	while(true) {
		Asm("   mov      r2, #0x01   \n"
			"\t ldrex    r1, [%1]    \n"
			"\t cmp      r1, #0x00   \n"
			"\t msrne    cpsr_c, %2  \n" /* ����ߤε���(bit0-7�Τ߽񤭴���) */
			"\t wfene                \n"
			"\t msrne    cpsr_c, %3  \n" /* ����ߤζػ�(bit0-7�Τ߽񤭴���) */
			"\t strexeq  r1, r2,[%1] \n"
			"\t mov      %0, r1      \n"
			:"=r"(locked)
			:"r"(p_lock),"I"(CPSR_SVC|CPSR_ALWAYS_SET),"I"(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET)
			:"r1","r2", "cc");

		if(locked == 0){
			/* ��å��������� */
			mpcore_data_memory_barrier();
			Asm("":::"memory");
			return;
		}
	}
}

#define t_acquire_lock(p_lock) x_acquire_lock(p_lock)
#define i_acquire_lock(p_lock) x_acquire_lock(p_lock)

#if TTYPE_KLOCK != G_KLOCK

/*
 *  2���ܰʾ�Υ�å������ʥ��������󥿥�������ƥ����ȶ��ѡ�
 */
Inline bool_t
x_acquire_nested_lock(LOCK *p_lock)
{
	PCB		*p_pcb;
	int		locked;

	while(true) {
		Asm("   mov      r2, #0x01   \n"
			"\t ldrex    r1, [%1]    \n"
			"\t cmp      r1, #0x00   \n"
			"\t msrne    cpsr_c, %2  \n" /* ����ߤε���(bit0-7�Τ߽񤭴���) */
			"\t wfene                \n"
			"\t msrne    cpsr_c, %3  \n" /* ����ߤζػ�(bit0-7�Τ߽񤭴���) */
			"\t strexeq  r1, r2,[%1] \n"
			"\t mov      %0, r1      \n"
			:"=r"(locked)
			:"r"(p_lock),"I"(CPSR_SVC|CPSR_ALWAYS_SET),"I"(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET)
			:"r1","r2", "cc");
		if(locked == 0){
			/* ��å��������� */
			mpcore_data_memory_barrier();
			Asm("":::"memory");
			return(false);
		}
		/*
		 * �ޥ����졼����󤹤��ǽ��������ΤǤ�����������  
		 * �󥿥�������ƥ����Ȥξ�硤�ޥ����졼����󤷤ʤ����ᡤ
		 * while���˼¹Ԥ��Ƥ�褤����1��ǥ�å����Ȥ줿��硤
		 * ��Ψ�������Τǡ������Ǽ������롥
		 */
		p_pcb = get_my_p_pcb(); 
		if (p_pcb->p_firstlock == NULL) {
			return(true);
		}
	}
}

#define t_acquire_nested_lock(p_lock) x_acquire_nested_lock(p_lock)
#define i_acquire_nested_lock(p_lock) x_acquire_nested_lock(p_lock)

#endif /* TTYPE_KLOCK != G_KLOCK */

/*
 *  ��å��β����ʥ��������󥿥�������ƥ����ȶ��ѡ�
 */
Inline void
x_release_lock(LOCK *p_lock)
{
	Asm("":::"memory");
	mpcore_data_memory_barrier();
	*p_lock = 0;
	mpcore_data_sync_barrier();
	Asm("sev");
}

/*
 *  �������Ĥ��ʤ���å������ؿ�
 */
Inline void 
x_acquire_lock_without_preemption(LOCK *p_lock)
{
	int		locked;

	while(true) {
		Asm("   mov      r2, #0x01   \n"
			"\t ldrex    r1, [%1]    \n"
			"\t cmp      r1, #0x00   \n"
			"\t wfene                \n"
			"\t strexeq  r1, r2,[%1] \n"
			"\t mov      %0, r1      \n"
			:"=r"(locked):"r"(p_lock):"r1","r2", "cc");
		if(locked == 0){
			/* ��å��������� */
			mpcore_data_memory_barrier();
			Asm("":::"memory");
			return;
		}
	}
}

/*
 *  ���ԥ��å���ǽ�Υͥ��ƥ��֥�å������ǻ��Ѥ���ؿ�
 */

/*
 *  ���ԥ��å��η�
 */
typedef uint32_t SPNLOCK;

/*
 *  ���ԥ��å��ν����
 */
Inline void
x_initialize_spin(ID spnid, SPNLOCK *p_spn_lock)
{
	*p_spn_lock = 0;
}

/*
 *  ���ԥ��å��μ���
 */
Inline void
x_lock_spin(SPNLOCK *p_spn_lock)
{
	x_acquire_lock(p_spn_lock);
}

#define t_lock_spin(p_spn_lock)  x_lock_spin(p_spn_lock)
#define i_lock_spin(p_spn_lock)  x_lock_spin(p_spn_lock)

/*
 *  ���ԥ��å��μ����λ��
 */
Inline bool_t
x_try_lock_spin(SPNLOCK *p_spn_lock)
{
	int locked;

	Asm("   mov      r2, #0x01   \n"
		"\t ldrex    r1, [%1]    \n"
		"\t cmp      r1, #0x00   \n"
		"\t strexeq  r1, r2,[%1] \n"
		"\t mov      %0, r1      \n"
		:"=r"(locked):"r"(p_spn_lock):"r1","r2", "cc");

	if (locked == 0) {
		/* ����������� */
		mpcore_data_memory_barrier();
		Asm("":::"memory");
		return(false);
	}

	return(true);
}

/*
 *  ���ԥ��å����ֵ�
 */
Inline void
x_unlock_spin(SPNLOCK *p_spn_lock)
{
	x_release_lock(p_spn_lock);
}
#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_CHIP_CONFIG_H */
