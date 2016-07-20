/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
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
 *  @(#) $Id: mpcore.c 791 2008-03-11 12:00:46Z honda $
 */

#include "kernel_impl.h"

/*
 * GIC CPU���󥿥ե�������Ϣ
 */

/*
 * CPU���󥿥ե������ν����
 */
void
gic_cpu_init(void)
{
    /* ����ͥ���٤����� */
    gic_cpu_set_priority(GIC_MIN_PRIORITY);

    /* �����ͥ���٤����ӥå�ͭ���� */
    gic_cpu_set_bp(GIC_CPUIF_BPR_ALL);

    /* CPU���󥿥ե�������ͭ���� */
    sil_wrw_mem((void *)GIC_CPUIF_CR, GIC_CPUIF_CR_EN);

    /* EOI �򥻥å� */
    sil_wrw_mem((void *)GIC_CPUIF_EOI, sil_rew_mem((void *)GIC_CPUIF_IAR));
}

/*
 * CPU���󥿥ե������ν�λ
 */
void
gic_cpu_stop(void)
{
    /* CPU���󥿥ե�������̵���� */
    sil_wrw_mem((void *)(GIC_CPUIF_CR), 0);
}


/*
 * GIC DIS���󥿥ե�������Ϣ
 */

/*
 * ����߶ػ�
 */
void
gic_disable_int(uint8_t id)
{
    uint16_t offset_addr;
    uint16_t offset_bit;

    offset_addr = (id / 32) * 4;
    offset_bit  = id % 32;

    sil_wrw_mem((void *)(GIC_CE + offset_addr), (1 << offset_bit));
}

/*
 * ����ߵ���
 */
void
gic_enable_int(uint8_t id)
{
    uint16_t offset_addr;
    uint16_t offset_bit;

    offset_addr = (id / 32) * 4;
    offset_bit  = id % 32;

    sil_wrw_mem((void *)(GIC_SE + offset_addr), (1 << offset_bit));
}

/*
 * ����ߥڥ�ǥ��󥰥��ꥢ
 */
void
gic_clear_pending(uint8_t id)
{
    uint16_t offset_addr;
    uint16_t offset_bit;

    offset_addr = (id / 32) * 4;
    offset_bit  = id % 32;

    sil_wrw_mem((void *)(GIC_CP + offset_addr), (1 << offset_bit));
}

/*
 * ����ߥڥ�ǥ��󥰥��å�
 */
void
gic_set_pending(uint8_t id){
    uint16_t offset_addr;
    uint16_t offset_bit;

    offset_addr = (id / 32) * 4;
    offset_bit  = id % 32;

    sil_wrw_mem((void *)(GIC_SP + offset_addr), (1 << offset_bit));
}

/*
 * ������׵�Υ����å�
 */
bool_t
gic_probe_int(uint8_t id){
    uint32_t state;
    uint16_t offset_addr;
    uint16_t offset_bit;

    offset_addr = (id / 32) * 4;
    offset_bit  = id % 32;

    state = sil_rew_mem((void *)(GIC_SP + offset_addr));

    if((state & (1 << offset_bit)) == (1 << offset_bit)){
        return(true);
    }else{
        return(false);
    }
}

/*
 * ����ߥ���ե�����졼���������
 */
void
gic_config(uint8_t id, uint8_t config){
    uint16_t offset_addr;
    uint16_t offset_bit;
    uint32_t cfgr_reg_val;

    offset_addr = (id / 16) * 4;
    offset_bit  = (id % 16) * 2;

    cfgr_reg_val  = sil_rew_mem((void *)(GIC_ICR + offset_addr));
    cfgr_reg_val &= ~(0x03U << offset_bit);
    cfgr_reg_val |= (0x03U & config) << offset_bit;

    sil_wrw_mem((void *)(GIC_ICR + offset_addr), cfgr_reg_val);
}

/*
 * �����ͥ���٤Υ��å�
 * ����ɽ�����Ϥ���
 */
void
gic_set_priority(uint8_t id, int pri){
    uint16_t offset_addr;
    uint16_t offset_bit;
    uint32_t pr_reg_val;

    offset_addr = (id / 4) * 4;
    offset_bit  = ((id % 4) * 8) + 4;

    pr_reg_val  = sil_rew_mem((void *)(GIC_IPR + offset_addr));
    pr_reg_val &= ~(0xfU << offset_bit);
    pr_reg_val |= (0xfU & pri) << (offset_bit);

    sil_wrw_mem((void *)(GIC_IPR + offset_addr), pr_reg_val);
}

/*
 * GIC����ߥ������åȤ�����
 * cpus�ϥ������åȤȤ���CPU�Υӥåȥѥ�����ǻ���
 *   CPU0 : 0x01
 *   CPU1 : 0x02
 *   CPU2 : 0x04
 *   CPU3 : 0x08
 */
void
gic_set_target(uint8_t id, uint8_t cpus)
{
    uint32_t offset_addr;
    uint32_t offset_bit;
    uint32_t itr_reg_val;

    offset_addr = (id / 4) * 4;
    offset_bit  = (id % 4) * 8;

    itr_reg_val  = sil_rew_mem((void *)(GIC_ITR + offset_addr));
    itr_reg_val &= ~(0xf << offset_bit);
    itr_reg_val |= (cpus << offset_bit);


    sil_wrw_mem((void *)(GIC_ITR + offset_addr), itr_reg_val);
}

/*
 * GIC���󥿥ե������ν����
 */
void
gic_init(void)
{
    int i;

    /* GIC���󥿥ե�������̵���� */
    sil_wrw_mem((void *)(GIC_CR), 0);

    /* ����ߤ����ƶػ� */
    for(i = 0; i < GIC_TNUM_INT/32; i++){
        sil_wrw_mem((void *)(GIC_CE + (4 * i)), 0xffffffff);
    }

    /* �ڥ�ǥ��󥰤򥯥ꥢ */
    for(i = 0; i < GIC_TNUM_INT/32; i++){
        sil_wrw_mem((void *)(GIC_CP + (4 * i)), 0xffffffff);
    }

    /* ͥ���ٺ��������  */
    for(i = 0; i < GIC_TNUM_INT/4; i++){
        sil_wrw_mem((void *)(GIC_IPR + (4 * i)), 0xffffffff);
    }

    /* �⡼�ɽ����(1-N Level) */
    for(i = 0; i < GIC_TNUM_INT/16; i++){
        sil_wrw_mem((void *)(GIC_ICR + (4 * i)), 0x55555555);
    }

    /* GIC���󥿥ե�������ͭ���� */
    sil_wrw_mem((void *)(GIC_CR), GIC_CR_ENABLE);
}

/*
 * GIC���󥿥ե������ν�λ
 */
void
gic_stop(void)
{
    /* GIC���󥿥ե�������̵���� */
    sil_wrw_mem((void *)(GIC_CR), 0);
}
