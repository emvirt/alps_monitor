/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2007 by Embedded and Real-Time Systems Laboratory
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
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ��������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ�
 *      ���դ��뤳�ȡ�
 *
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ��������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 *
 *  @(#) $Id: target_serial.c 449 2007-08-03 11:29:48Z honda $
 */

/*
 *  ���ꥢ��I/O�ǥХ�����SIO�˥ɥ饤�С�PB1176T�ѡ�
 */

#include <kernel.h>
#include <t_syslog.h>
#include "target_serial.h"

/*
 *  SIO�ɥ饤�Фν����
 */
void
sio_initialize(intptr_t exinf)
{
    uart_pl011_initialize();
}

/*
 *  ���ꥢ��I/O�ݡ��ȤΥ����ץ�
 */
SIOPCB *
sio_opn_por(ID siopid, intptr_t exinf)
{
    SIOPCB  *p_siopcb;
    bool_t    opnflg;
    ER      ercd;

    /*
     *  �����ץ󤷤��ݡ��Ȥ����뤫��opnflg���ɤ�Ǥ�����
     */
    opnflg = uart_pl011_openflag(siopid);

    /*
     *  �ǥХ�����¸�Υ����ץ������
     */
    p_siopcb = uart_pl011_opn_por(siopid, exinf);

    /*
     *  ���ꥢ��I/O����ߤΥޥ����������롥
     */
    if (!opnflg) {
        ercd = ena_int(uart_pl011_get_irqno(p_siopcb));
        assert(ercd == E_OK);
    }
    return(p_siopcb);
}

/*
 *  ���ꥢ��I/O�ݡ��ȤΥ�������
 */
void
sio_cls_por(SIOPCB *p_siopcb)
{
    /*
     *  �ǥХ�����¸�Υ�������������
     */
    uart_pl011_cls_por(p_siopcb);

    /*
     *  ���ꥢ��I/O����ߤ�ޥ������롥
     */
//    if (!uart_pl011_openflag()) {
        dis_int(uart_pl011_get_irqno(p_siopcb));
//    }
}

/*
 *  SIO�γ���ߥ����ӥ��롼����
 */
void
sio_isr(intptr_t exinf)
{
    uart_pl011_isr(exinf);
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥؤ�ʸ������
 */
bool_t
sio_snd_chr(SIOPCB *siopcb, char_t c)
{
    return(uart_pl011_snd_chr(siopcb, c));
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ����ʸ������
 */
int_t
sio_rcv_chr(SIOPCB *siopcb)
{
    return(uart_pl011_rcv_chr(siopcb));
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ���Υ�����Хå��ε���
 */
void
sio_ena_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
    uart_pl011_ena_cbr(siopcb, cbrtn);
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ���Υ�����Хå��ζػ�
 */
void
sio_dis_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
    uart_pl011_dis_cbr(siopcb, cbrtn);
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ����������ǽ������Хå�
 */
void
uart_pl011_ierdy_snd(intptr_t exinf)
{
    sio_irdy_snd(exinf);
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ���μ������Υ�����Хå�
 */
void
uart_pl011_ierdy_rcv(intptr_t exinf)
{
    sio_irdy_rcv(exinf);
}