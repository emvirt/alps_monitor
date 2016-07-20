/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2008 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_timer.h 599 2010-05-07 08:00:38Z ertl-honda $
 */

/*
 *  �����ޥɥ饤�С�KZM_CA9�ѡ�
 */

#ifndef TOPPERS_TARGET_TIMER_H
#define TOPPERS_TARGET_TIMER_H

/*
 *  ���åװ�¸�⥸�塼���MPCORE�ѡ�
 */
//#include "arm_gcc/mpcore/chip_timer.h"

#include <sil.h>
//#include "mpcore.h"
#include "kzm_ca9.h"

/*
 *  �����ޡ���Ϣ
 */
//#define TIMER_BASE_REG         0x100E4000
//#define OVRTIMER_BASE_REG    ?

#define TIMER_SP804_1LOAD		0x000
#define TIMER_SP804_1VALUE		0x004
#define TIMER_SP804_1CONTROL	        0x008
#define TIMER_SP804_1CLEAR		0x00C
#define TIMER_SP804_1RIS		0x010
#define TIMER_SP804_2LOAD		0x020
#define TIMER_SP804_2VALUE		0x024
#define TIMER_SP804_2CONTROL	        0x028
#define TIMER_SP804_2CLEAR		0x02C

#define TIMER_SP804_CTRL_TIMER_EN     0x00000080
#define TIMER_SP804_CTRL_PERIODIC     0x00000040
#define TIMER_SP804_CTRL_INT_EN       0x00000020
#define TIMER_SP804_CTRL_PRESCALE_1   0x00000000
#define TIMER_SP804_CTRL_PRESCALE_16  0x00000004
#define TIMER_SP804_CTRL_PRESCALE_256 0x00000008
#define TIMER_SP804_CTRL_TIMESIZE_16  0x00000000
#define TIMER_SP804_CTRL_TIMESIZE_32  0x00000002
#define TIMER_SP804_CTRL_ONE_SHOT     0x00000001

#define INHNO_TIMER     DIC_IRQNO_TM_SP804_1  /* ����ߥϥ�ɥ��ֹ� */
#define INTNO_TIMER     DIC_IRQNO_TM_SP804_1  /* ������ֹ� */

#define INTPRI_TIMER    -6            /* �����ͥ���� */
#define INTATR_TIMER    0U            /* �����°�� */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  �������ͤ�����ɽ���η�
 */
typedef uint32_t    CLOCK;

/*
 *  �������ͤ�����ɽ���ȥߥ��á�����ñ�̤Ȥ��Ѵ�
 *  1������(1MHz) �˥�����ȥ����󤹤�
 */
#define TIMER_CLOCK          100000U
#define TO_CLOCK(nume, deno) (TIMER_CLOCK * (nume) / (deno))
#define TO_USEC(clock)       (((SYSUTM) clock) * 1000U / TIMER_CLOCK)
#define TO_NSEC(clock)		 (((SYSUTM) clock) * 100000U / TIMER_CLOCK) / 4

/*
 *  ����Ǥ������Υ����޼�����ñ�̤�����ɽ����
 */
#define MAX_CLOCK        ((CLOCK) 0xffffffffU)

/*
 *  �����ޤθ����ͤ�����ȯ�������ͤȤߤʤ�����Ƚ��
 */
#define GET_TOLERANCE    100U    /* �����٤�θ��Ѥ��͡�ñ�̤�����ɽ����*/
#define BEFORE_IREQ(clock) \
            ((clock) >= TO_CLOCK(TIC_NUME, TIC_DENO) - GET_TOLERANCE)

/*
 *  �����ޤε�ư����
 *
 *  �����ޤ�������������Ū�ʥ����޳�����׵��ȯ�������롥
 */
extern void    target_timer_initialize(intptr_t exinf);

/*
 *  �����ޤ���߽���
 *
 *  �����ޤ�ư�����ߤ����롥
 */
extern void    target_timer_terminate(intptr_t exinf);

/*
 *  �����ޤθ����ͤ��ɽФ�
 */
Inline CLOCK
target_timer_get_current(void)
{
  return(TO_CLOCK(TIC_NUME, TIC_DENO) - sil_rew_mem((void *)(TIMER_BASE_REG + TIMER_SP804_2VALUE)));
}

/*
 *  �����޳�����׵�Υ����å�
 */
Inline bool_t
target_timer_probe_int(void)
{
    return(x_probe_int(INTNO_TIMER));

}

/*
 *  �����޳���ߥϥ�ɥ�
 */
extern void    target_timer_handler(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_TARGET_TIMER_H */
