/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2007 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_config.h 369 2007-07-18 12:48:50Z honda $
 */

/*
 *  �������åȰ�¸�⥸�塼���PB1176T�ѡ�
 *
 *  �����ͥ�Υ������åȰ�¸���Υ��󥯥롼�ɥե����롥kernel_impl.h�Υ���
 *  ���åȰ�¸���ΰ����դ��Ȥʤ롥
 */

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H

/*
 *  �������åȥ����ƥ�Υϡ��ɥ������񸻤����
 */
#include "pb1176nt.h"

/*
 *  �ȥ졼�����˴ؤ�������
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  �٥������롼����򥫡��ͥ�ǻ��Ĥ������
 */
#define VECTOR_KERNEL

/*
 *  �٥������롼���������ʤ����Υ٥��������ɥ쥹����Ƭ����
 */ 
#define VECTOR_START  0x00

/*
 *  �ǡ������������ν�����򤷤ʤ�
 */
//#define TOPPERS_OMIT_DATA_INIT

/*
 * Use a target dependent get_utm
 */
#define OMIT_GET_UTM

/*
 *  IRQ��FIQ�ϥ�ɥ�������������ץ��å���¸���ǻ��Ĥ���
 *  �������åȰ�¸���ǻ��Ĥ��λ��� 
 */
//#define TARGET_IRQ_HANDLER
//#define TARGET_FIQ_HANDLER

/*
 *  ASP�����ͥ�ư����Υ���ޥåפȴ�Ϣ�������
 *
 *    10800000 - 1080ffff       �������ΰ�
 *    10810000 -                �ǡ����ΰ�
 *             - 11000000       �ǥե���Ȥ��󥿥�������ƥ������ѤΥ����å��ΰ�
 */

/*
 *  �ǥե���Ȥ��󥿥�������ƥ������ѤΥ����å��ΰ�����
 */
#define DEFAULT_ISTKSZ      0x2000U
// TODO: check about DEFAULT_ISTK

                                                   
/*
 *  ���������Ԥ��Τ��������������SIL�Υ������åȰ�¸����
 */
#define SIL_DLY_TIM1    50
#define SIL_DLY_TIM2    40

/*
 *  ����ߥϥ�ɥ��ֹ�ȳ�����ֹ�˴ؤ������
 */
#define TMIN_INTNO  GIC_TMIN_INTNO 
#define TMAX_INTNO  GIC_TMAX_INTNO
#define TNUM_INT    GIC_TNUM_INT

#define TMIN_INHNO  GIC_TMIN_INTNO 
#define TMAX_INHNO  GIC_TMAX_INTNO
#define TNUM_INH    GIC_TNUM_INT


#ifndef TOPPERS_MACRO_ONLY

/*
 *  ������ֹ���ϰϤ�Ƚ��
 *
 *  �ӥåȥѥ���������Τ��ưפˤ��뤿��ˡ�8�Ϸ��֤ˤʤäƤ��롥
 */
#define VALID_INTNO(intno) (TMIN_INTNO <= (intno) && (intno) <= TMAX_INTNO)
#define VALID_INTNO_DISINT(intno)	VALID_INTNO(intno)
#define VALID_INTNO_CFGINT(intno)	VALID_INTNO(intno)
#define VALID_INTNO_ATTISR(intno)   VALID_INTNO(intno)

/*
 *  ����ߥϥ�ɥ����Ͽ�ѥơ��֥�
 *   ���֤ϥ���ե�����졼������������ 
 */
extern const FP inh_tbl[TNUM_INH];

/*
 *  ����ߥϥ�ɥ������
 */
Inline void
x_define_inh(INHNO inhno, FP inthdr)
{
}

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  ����ߥϥ�ɥ�ν����������������ޥ���
 *
 */
#define INT_ENTRY(inhno, inthdr)    inthdr
#define INTHDR_ENTRY(inhno, inhno_num, inthdr) extern void inthdr(void);

/*
 *  TOPPERSɸ�����߽�����ǥ�μ¸�
 */

/*
 *  �����ͥ���٥ޥ������饤�֥��
 *
 *  GIC�Ǥϡ�GIC���ͥ���٥쥸������CPU�γ����ͥ���٥ޥ�����IPM�ˤ����ꤹ�롥
 *  �����ͥ���٥ޥ�����0x0 �� 0xf ��16�ʳ����ͤ��礭���ۤ���ͥ���٤Ǥ��롥
 *  IPM����¸���Ƥ�������ˡ������ͥ���٤γ���ɽ����-1����Ϣ³��������͡�
 *  ��Ȥ����Ȥ��ǽ�Ǥ��뤬��;�פʷ׻���ɬ�פˤʤ롥������򤱤뤿�ᡤIPM��
 *  ��¸������ˤϡ�GIC�ǰ���ͥ���٤η����Ȥ��롥�����ͤ�����ͥ���٥ޥ���
 *  ������ɽ���ȸƤӡ�IIPM�Ƚ񤯤��Ȥˤ��롥 
 */

/*
 *  �����ͥ���٥ޥ����γ���ɽ��������ɽ�����Ѵ�
 *
 *  ������֥����Υ������ե����뤫�饤�󥯥롼�ɤ�����Τ���ˡ���
 *  ���㥹�Ȥ��ʤ�������Ѱդ��Ƥ��롥
 */
#ifndef TOPPERS_MACRO_ONLY
#define EXT_IPM(iipm)    ((PRI)(iipm - 0x0fU))    /* ����ɽ������ɽ���� */
#define INT_IPM(ipm)    ((uint8_t)(ipm + 0x0fU))/* ����ɽ��������ɽ���� */
#else /* TOPPERS_MACRO_ONLY */
#define EXT_IPM(iipm)    (iipm - 0xf)            /* ����ɽ������ɽ���� */
#define INT_IPM(ipm)    (ipm + 0xf)                /* ����ɽ��������ɽ���� */
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
    return(gic_cpu_current_priority());
}

/*
 *  IPM�ʥϡ��ɥ������γ����ͥ���٥ޥ���������ɽ���ˤθ����ͤ�����
 */
Inline void
set_iipm(uint8_t iipm)
{
    gic_cpu_set_priority(iipm);
}

/*
 *  (��ǥ���)�����ͥ���٥ޥ���������
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
 *
 */
Inline PRI
x_get_ipm(void)
{
    return(EXT_IPM(current_iipm()));
}

#define t_get_ipm() x_get_ipm()
#define i_get_ipm() x_get_ipm()

/*
 *  �����°���ơ��֥�
 * 
 *  �����°�������ꤵ��Ƥ����"1"�����ꤵ��Ƥ��ʤ����"0"�Ȥʤ�
 */
extern const uint8_t cfgint_tbl[TNUM_INT];

/*
 * �ʥ�ǥ��Ρ˳�����׵�ػߥե饰�Υ��å�
 *
 *  ���ꤵ�줿��������ֹ�γ�����׵�ػߥե饰�Υ��åȤ��ơ�����ߤ�
 *  �ػߤ��롥
 *    
 *  �����°�������ꤵ��Ƥ��ʤ�������׵�饤����Ф��Ƴ�����׵�ػ�
 *  �ե饰�򥯥ꥢ���褦�Ȥ������ˤϡ�FALSE���֤���  
 */
#define SUPPORT_DIS_INT /* dis_int�򥵥ݡ��Ȥ��� */

Inline bool_t
x_disable_int(INTNO intno)
{
    if (cfgint_tbl[intno] == 0){
        return(false);
    }
    
    gic_disable_int(intno);
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
 *  �ե饰�򥯥ꥢ���褦�Ȥ������ˤϡ�FALSE���֤���
 */
#define SUPPORT_ENA_INT /* ena_int�򥵥ݡ��Ȥ��� */

Inline bool_t
x_enable_int(INTNO intno)
{
    if (cfgint_tbl[intno] == 0){
        return(false);
    }
    
    gic_enable_int(intno);
    return(true);
}

#define t_enable_int(intno) x_enable_int(intno)
#define i_enable_int(intno) x_enable_int(intno)

/*
 *  ������׵�Υ��ꥢ
 */
Inline void
x_clear_int(INTNO intno)
{
    gic_clear_pending(intno);
}

#define t_clear_int(intno) x_clear_int(intno) 
#define i_clear_int(intno) x_clear_int(intno) 


/*
 *  ������׵�Υ����å�
 */
Inline bool_t
x_probe_int(INTNO intno)
{
    return(gic_probe_int(intno));
}

#define t_probe_int(intno) x_probe_int(intno)
#define i_probe_int(intno) x_probe_int(intno)

/*
 *  ������׵�饤���°��������
 *
 *  PIC�ϡ����ƥ�٥�ȥꥬ�Τ��ᡤ°���ǡ��ȥꥬ�λ���ϤǤ�
 *  �ʤ�  
 */
extern void    x_config_int(INTNO intno, ATR intatr, PRI intpri);

/*
 *  ����ߥϥ�ɥ���������ɬ�פ�IRC���
 *
 *  CM922T�Ǥϡ�ɬ�פʽ����Ϥʤ�
 */
Inline void
i_begin_int(INTNO intno)
{
}

/*
 *  ����ߥϥ�ɥ�νи���ɬ�פ�IRC���
 *
 *  PIC�Ǥϡ�ɬ�פʽ����Ϥʤ�
 */
Inline void
i_end_int(INTNO intno)
{
	gic_cpu_end_int(intno);
}

/*
 *  �������åȥ����ƥ��¸�ν����
 */
extern void target_initialize(void);

/*
 *  �������åȥ����ƥ�ν�λ
 *
 *  �����ƥ��λ������˻Ȥ���
 */
extern void target_exit(void) NoReturn;

/*
 *  atexit�ν����ȥǥ��ȥ饯���μ¹�
 */
Inline void
call_atexit(void)
{
    extern void    software_term_hook(void);
    void (*volatile fp)(void) = software_term_hook;

    /*
     *  software_term_hook�ؤΥݥ��󥿤򡤰�övolatile����Τ���fp����
     *  �����Ƥ���Ȥ��Τϡ�0�Ȥ���Ӥ���Ŭ���Ǻ������ʤ��褦�ˤ��뤿
     *  ��Ǥ��롥
     */
    if (fp != 0) {
        (*fp)();
    }
}

/*
 *  ����ߥϥ�ɥ��target_support.S��
 */
extern void interrupt_handler(void);

/*
 *  ̤����γ���ߤ����ä����ν���
 */
extern void default_int_handler(void);


#endif /* TOPPERS_MACRO_ONLY */

/*
 *  ������¸�⥸�塼���ARM�ѡ�
 */
#include "arm_gcc/common/core_config.h"

/*
 *  �ȥ졼�����˴ؤ�������
 */
// #include "nulltrace/trace_config.h"

#endif /* TOPPERS_TARGET_CONFIG_H */
