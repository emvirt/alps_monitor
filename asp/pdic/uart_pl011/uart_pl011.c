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
 *  @(#) $Id: uart_pl011.c 324 2007-07-17 06:00:17Z hiro $
 */

/*
 *   PL011�� �ʰ�SIO�ɥ饤��
 */

#include <sil.h>
#include "target_syssvc.h"
#include "target_serial.h"
#include "uart_pl011.h"

/*
 *  ���ꥢ��I/O�ݡ��Ƚ�����֥�å������
 */
typedef struct sio_port_initialization_block {
    void  *dr;
    void  *sr_cr;
    void  *fr;
    void  *lpr;
    void  *ibrd;
    void  *fbrd;
    void  *lcr_h;
    void  *cr;
    void  *ifls;
    void  *imsc;
    void  *ris;
    void  *mis;
    void  *icr;
    void  *dmacr;
} SIOPINIB;

/*
 *  ���ꥢ��I/O�ݡ��ȴ����֥�å������
 */
struct sio_port_control_block {
    const SIOPINIB  *siopinib;  /* ���ꥢ��I/O�ݡ��Ƚ�����֥�å� */
    intptr_t  exinf;            /* ��ĥ���� */
    bool_t    openflag;           /* �����ץ�Ѥߥե饰 */
    bool_t    sendflag;           /* ��������ߥ��͡��֥�ե饰 */
    bool_t    getready;           /* ʸ��������������� */
    bool_t    putready;           /* ʸ���������Ǥ������ */
};

/*
 *  ���ꥢ��I/O�ݡ��Ƚ�����֥�å�
 */

/*
 * ���ꥢ��I/O�ݡ��Ƚ�����֥�å�
 */
const SIOPINIB siopinib_table[TNUM_SIOP] = {
	{(void *)(UART_BASE_REG + UART_PL011_DR),
	(void *)(UART_BASE_REG + UART_PL011_SR_CR),
	(void *)(UART_BASE_REG + UART_PL011_FR),
	(void *)(UART_BASE_REG + UART_PL011_LPR),
	(void *)(UART_BASE_REG + UART_PL011_IBRD),
	(void *)(UART_BASE_REG + UART_PL011_FBRD),
	(void *)(UART_BASE_REG + UART_PL011_LCR_H),
	(void *)(UART_BASE_REG + UART_PL011_CR),
	(void *)(UART_BASE_REG + UART_PL011_IFLS),
	(void *)(UART_BASE_REG + UART_PL011_IMSC),
	(void *)(UART_BASE_REG + UART_PL011_RIS),
	(void *)(UART_BASE_REG + UART_PL011_MIS),
	(void *)(UART_BASE_REG + UART_PL011_ICR),
	(void *)(UART_BASE_REG + UART_PL011_DMACR)
    }
};


/*
 *  ���ꥢ��I/O�ݡ��ȴ����֥�å��Υ��ꥢ
 */
SIOPCB	siopcb_table[TNUM_SIOP];

/*
 *  ���ꥢ��I/O�ݡ���ID��������֥�å�����Ф�����Υޥ���
 */
#define INDEX_SIOP(siopid)	((uint_t)((siopid) - 1))
#define get_siopcb(siopid)	(&(siopcb_table[INDEX_SIOP(siopid)]))

/*
 *  ʸ��������Ǥ��뤫��
 */
Inline bool_t
uart_pl011_getready(SIOPCB *p_siopcb)
{
    return(((sil_rew_mem((void *)(p_siopcb->siopinib->fr)) & UFR_RXFE) == 0));
}

/*
 *  ʸ���������Ǥ��뤫��
 */
Inline bool_t
uart_pl011_putready(SIOPCB *p_siopcb)
{
    return((sil_rew_mem((void *)(p_siopcb->siopinib->fr)) & UFR_TXFF) == 0);
}

/*
 *  ��������ʸ���μ�Ф�
 */
Inline char_t
uart_pl011_getchar(SIOPCB *p_siopcb)
{
    return(sil_rew_mem((void *)(p_siopcb->siopinib->dr)));
}

/*
 *  ��������ʸ���ν����
 */
Inline void
uart_pl011_putchar(SIOPCB *p_siopcb, char_t c)
{
    sil_wrw_mem((void *)(p_siopcb->siopinib->dr), c);
}

/*
 *  ��������ߵ���
 */
Inline void
uart_pl011_enable_send(SIOPCB *p_siopcb)
{
    sil_wrw_mem((void *)(p_siopcb->siopinib->imsc),
        (sil_rew_mem((void *)(p_siopcb->siopinib->imsc)) | UART_IMSC_TXIM));
}

/*
 *  ��������߶ػ�
 */
Inline void
uart_pl011_disable_send(SIOPCB *p_siopcb)
{
     sil_wrw_mem((void *)(p_siopcb->siopinib->imsc),
        (sil_rew_mem((void *)(p_siopcb->siopinib->imsc)) & ~UART_IMSC_TXIM));
}


/*
 *  ��������ߵ���
 */
Inline void
uart_pl011_enable_rcv(SIOPCB *p_siopcb)
{
    sil_wrw_mem((void *)(p_siopcb->siopinib->imsc),
        (sil_rew_mem((void *)(p_siopcb->siopinib->imsc)) | UART_IMSC_RXIM));
}

/*
 *  ��������߶ػ�
 */
Inline void
uart_pl011_disable_rcv(SIOPCB *p_siopcb)
{
    sil_wrw_mem((void *)(p_siopcb->siopinib->imsc),
        (sil_rew_mem((void *)(p_siopcb->siopinib->imsc)) & ~UART_IMSC_RXIM));
}

/*
 *  SIO�ɥ饤�Фν����
 */
void
uart_pl011_initialize(void)
{
	SIOPCB	*p_siopcb;
	uint_t	i;

	/*
	 *  ���ꥢ��I/O�ݡ��ȴ����֥�å��ν����
	 */
    for (p_siopcb = siopcb_table, i = 0; i < TNUM_SIOP; p_siopcb++, i++) {
        p_siopcb->siopinib = &(siopinib_table[i]);
        p_siopcb->openflag = false;
        p_siopcb->sendflag = false;
    }
}

/*
 *  �����ץ󤷤Ƥ���ݡ��Ȥ����뤫��
 */
bool_t
uart_pl011_openflag(void)
{

#if TNUM_SIOP == 1
    return(siopcb_table[0].openflag);
#else /* TNUM_SIOP == 1 */
	return(siopcb_table[0].openflag || siopcb_table[1].openflag);
#endif
}

/*
 *  ���ꥢ��I/O�ݡ��ȤΥ����ץ�
 */
SIOPCB *
uart_pl011_opn_por(ID siopid, intptr_t exinf)
{
    SIOPCB          *p_siopcb;
    const SIOPINIB  *p_siopinib;

    p_siopcb = get_siopcb(siopid);
    p_siopinib = p_siopcb->siopinib;

	/* setup baud rate divisor */
	sil_wrw_mem((void *)(p_siopinib->ibrd), 0x027);
	sil_wrw_mem((void *)(p_siopinib->fbrd), 0x004);

	/* refer to the TRM for details */
	sil_wrw_mem((void *)(p_siopinib->cr), 0x301);
	sil_wrw_mem((void *)(p_siopinib->lcr_h), 0x060);
	sil_wrw_mem((void *)(p_siopinib->ifls), 0x000);

	/* Mask all interrupt */
	sil_wrw_mem((void *)(p_siopinib->imsc), 0);

	p_siopcb->exinf = exinf;
	p_siopcb->getready = p_siopcb->putready = false;
	p_siopcb->openflag = true;

	return(p_siopcb);
}

/*
 *  ���ꥢ��I/O�ݡ��ȤΥ�����
 */
void
uart_pl011_cls_por(SIOPCB *p_siopcb)
{
//    sil_wrw_mem((void *)(p_siopcb->siopinib->cr),
//        (sil_rew_mem((void *)(p_siopcb->siopinib->cr)) & ~UCR_UARTEN)); /* Disable Interrupt */

    p_siopcb->openflag = false;
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥؤ�ʸ������
 */
bool_t
uart_pl011_snd_chr(SIOPCB *p_siopcb, char_t c)
{
	if (uart_pl011_putready(p_siopcb)){
		uart_pl011_putchar(p_siopcb, c);
		return(true);
	}
	return(false);
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ����ʸ������
 */
int_t
uart_pl011_rcv_chr(SIOPCB *p_siopcb)
{
	if (uart_pl011_getready(p_siopcb)) {
		return((int_t)(uint8_t) uart_pl011_getchar(p_siopcb));
	}
	return(-1);
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ���Υ�����Хå��ε���
 */
void
uart_pl011_ena_cbr(SIOPCB *p_siopcb, uint_t cbrtn)
{
    switch (cbrtn) {
      case SIO_ERDY_SND:
        uart_pl011_enable_send(p_siopcb);
        break;
      case SIO_ERDY_RCV:
        uart_pl011_enable_rcv(p_siopcb);
        break;
    }
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ���Υ�����Хå��ζػ�
 */
void
uart_pl011_dis_cbr(SIOPCB *p_siopcb, uint_t cbrtn)
{
	switch (cbrtn) {
	  case SIO_ERDY_SND:
		uart_pl011_disable_send(p_siopcb);
		break;
	  case SIO_ERDY_RCV:
		uart_pl011_disable_rcv(p_siopcb);
		break;
	}
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ��Ф������߽���
 */
static void
uart_pl011_isr_siop(SIOPCB *p_siopcb)
{
	if (uart_pl011_getready(p_siopcb)) {
		/*
		 *  �������Υ�����Хå��롼�����ƤӽФ���
		 */
		uart_pl011_ierdy_rcv(p_siopcb->exinf);
	}
	if (uart_pl011_putready(p_siopcb)) {
		/*
		 *  ������ǽ������Хå��롼�����ƤӽФ���
		 */
		uart_pl011_ierdy_snd(p_siopcb->exinf);
	}
}

/*
 *  SIO�γ���ߥ����ӥ��롼����
 */
void
uart_pl011_isr0()
{
    uart_pl011_isr_siop(&(siopcb_table[0]));
}
