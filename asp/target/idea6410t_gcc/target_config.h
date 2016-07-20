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
 *  �������åȰ�¸�⥸�塼���idea6410T�ѡ�
 *
 *  �����ͥ�Υ������åȰ�¸���Υ��󥯥롼�ɥե����롥kernel_impl.h�Υ���
 *  ���åȰ�¸���ΰ����դ��Ȥʤ롥
 */

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H

/*
 *  �������åȥ����ƥ�Υϡ��ɥ������񸻤����
 */
#include "idea6410t.h"

/*
 *  �ȥ졼�����˴ؤ�������
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  SafeG����Ѥ�����ν��������
 */
#ifdef TOPPERS_WITH_SAFEG

// #define TOPPERS_CUSTOM_IDLE

#endif /* TOPPERS_WITH_SAFEG */

/*
 *  CPU��å��ǥ��åȤ���ӥå�
 */
#define CPSR_CPULOCK     (CPSR_FIQ_BIT|CPSR_IRQ_BIT)

/*
 *  CPSR�˾�˥��åȤ���ѥ�����
 */
#define CPSR_ALWAYS_SET CPSR_IRQ_BIT

/*
 *  support overrun handlers (can also be enabled in CFLAGS)
 */
// #define TOPPERS_SUPPORT_OVRHDR

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
// #define TOPPERS_OMIT_DATA_INIT

/*
 *  IRQ��FIQ�ϥ�ɥ�������������ץ��å���¸���ǻ��Ĥ���
 *  �������åȰ�¸���ǻ��Ĥ��λ���
 */
//#define TARGET_IRQ_HANDLER
#define TARGET_FIQ_HANDLER

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
// TODO: check about DEFAULT_ISTK??

/*
 *  ���������Ԥ��Τ��������������SIL�Υ������åȰ�¸����
 */
#define SIL_DLY_TIM1    20
#define SIL_DLY_TIM2    10

/*
 *  ����ߥϥ�ɥ��ֹ�˴ؤ������
 */
#define TMIN_INHNO 0U
#define TMAX_INHNO 63U
#define TNUM_INH   64U

/*
 *  ������ֹ�˴ؤ������
 */
#define TMIN_INTNO 0U
#define TMAX_INTNO 63U
#define TNUM_INT   64U


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
 *  ����ߤΥϥ�ɥ�γ����ͥ���٥ơ��֥�
 *
 *  TZIC�ϳ���ߥϥ�ɥ��װ����ͥ���٤�����Ǥ��ʤ����ᡤ���եȥ�
 *  ����Ū�˼¸�����
 */
extern const PRI inh_ipm_tbl[TNUM_INH];

/*
 *  �Ƴ���ߤγ�����׵�ػߥե饰�ξ���
 */
extern uint64_t idf;

/*
 *  �����ͥ���٥ޥ������饤�֥��
 *
 *  CM922T�ϳ����ͥ���٥ޥ�����ϡ��ɥ�����Ū�˻����ʤ����ᡤ�����
 *  �׵�ػߥե饰�ˤ������ͥ���٥ޥ�����¸�����
 */

/*
 *  ���ߤγ����ͥ���٥ޥ�������
 */
extern PRI ipm;

/*
 *  �����ͥ���٥ޥ�����˥��åȤ��롤������׵�ػߥե饰����
 *  �Υơ��֥�
 */
extern const uint64_t ipm_mask_tbl[(TMAX_INTPRI - TMIN_INTPRI + 1)];

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  IPM��imp_mask_tbl�Υ���ǥå������Ѵ�����ޥ���
 */
#define INDEX_IPM(ipm)  (-(ipm)-1)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  (��ǥ���)�����ͥ���٥ޥ���������
 *
 *  ���ꤵ�줿ͥ���٤����ꤵ�줿������׵�ػߥե饰�Υơ��֥���ͤȡʥ��
 *  ���Ρ˳Ƴ���ߤγ�����׵�ػߥե饰�ξ��֤��ݻ������ѿ����ͤȤ�
 *  OR��IRC�γ�����׵�ػߥե饰�˥��åȤ������ꤷ��ͥ���٤������ѿ�
 *  ipm����¸���롥
 */
Inline void
x_set_ipm(PRI intpri)
{
    uint64_t ipm_mask;

    if (intpri == TIPM_ENAALL) {
        ipm_mask = 0U;
    } else {
        ipm_mask = ipm_mask_tbl[INDEX_IPM(intpri)];
    }

    /*
     *  CM922T�γ���ߥ���ȥ���ϥ��͡��֥�쥸������
     *  ���ꥢ���쥸���������뤿�ᡤ��ö���Ƥγ���ߤ�ػߤ��Ƥ��顤
     *  ����γ���ߤΤߵ��Ĥ���ɬ�פ�����
     */
    /* ������߶ػ� */
    tzic_disable_int(~0U);

    /* �ޥ������ꤵ��Ƥ��ʤ�����ߤΤߵ��� */
    tzic_enable_int(~(ipm_mask|idf));

    ipm = intpri;
}

#define t_set_ipm(intpri) x_set_ipm(intpri)
#define i_set_ipm(intpri) x_set_ipm(intpri)

/*
 *  (��ǥ���)�����ͥ���٥ޥ����λ���
 *
 *  ipm���ͤ��֤�
 */
Inline PRI
x_get_ipm(void)
{
    return(ipm);
}

#define t_get_ipm() x_get_ipm()
#define i_get_ipm() x_get_ipm()

/*
 *  �����°�������ꤵ��Ƥ��뤫��Ƚ�̤��뤿����ѿ�
 */
extern uint64_t	bitpat_cfgint;

/*
 * �ʥ�ǥ��Ρ˳�����׵�ػߥե饰�Υ��å�
 *
 *  ���ꤵ�줿��������ֹ�γ�����׵�ػߥե饰�Υ��åȤ��ơ�����ߤ�
 *  �ػߤ��롥�ޤ����ʥ�ǥ��Ρ˳�����׵�ػߥե饰���������idf����
 *  ������ӥåȤ�򥻥åȤ��롥
 *  ������׵��ޥ������뵡ǽ�򥵥ݡ��Ȥ��Ƥ��ʤ����ˤϡ�false���֤�
 */
#define SUPPORT_DIS_INT /* dis_int�򥵥ݡ��Ȥ��� */

Inline bool_t
x_disable_int(INTNO intno)
{
    if ((bitpat_cfgint & INTNO_BITPAT(intno)) == 0U) {
        return(false);
    }
    tzic_disable_int(INTNO_BITPAT(intno));
    idf |= INTNO_BITPAT(intno);
    return(true);
}

#define t_disable_int(intno)  x_disable_int(intno)
#define i_disable_int(intno)  t_disable_int(intno)

/*
 *  (��ǥ���)����׵�ػߥե饰�β��
 *
 *  ���ꤵ�줿��������ֹ�γ�����׵�ػߥե饰�Υ��ꥢ���ơ�����ߤ�
 *  ���Ĥ��롥�ޤ����ʥ�ǥ��Ρ˳�����׵�ػߥե饰���������idf����
 *  ������ӥåȤ�򥯥ꥢ���롥
 *  ������׵��ޥ������뵡ǽ�򥵥ݡ��Ȥ��Ƥ��ʤ����ˤϡ�false���֤�
 */
#define SUPPORT_ENA_INT /* ena_int�򥵥ݡ��Ȥ��� */

Inline bool_t
x_enable_int(INTNO intno)
{
    if ((bitpat_cfgint & INTNO_BITPAT(intno)) == 0U) {
        return(false);
    }
    tzic_enable_int(INTNO_BITPAT(intno));
    idf &= ~INTNO_BITPAT(intno);
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
    tzic_clear_int(INTNO_BITPAT(intno));
}

#define t_clear_int(intno) x_clear_int(intno)
#define i_clear_int(intno) x_clear_int(intno)


/*
 *  ������׵�Υ����å�
 */
Inline bool_t
x_probe_int(INTNO intno)
{
    return(tzic_probe_int(INTNO_BITPAT(intno)));
}

#define t_probe_int(intno) x_probe_int(intno)
#define i_probe_int(intno) x_probe_int(intno)

/*
 *  ������׵�饤���°��������
 *
 *  TZIC�ϡ����ƥ�٥�ȥꥬ�Τ��ᡤ°���ǡ��ȥꥬ�λ���ϤǤ�
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
 *  TZIC�Ǥϡ�ɬ�פʽ����Ϥʤ�
 */
Inline void
i_end_int(INTNO intno)
{
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
