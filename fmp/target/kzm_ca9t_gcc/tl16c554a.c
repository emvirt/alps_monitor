/*
 *     TL16C554A �� �ʰ�SIO�ɥ饤��
 */

#include <sil.h>
#include "target_syssvc.h"
#include "tl16c554a.h"


/*
 *  ���ꥢ��I/O�ݡ��Ƚ�����֥�å������
 */
typedef struct sio_port_initialization_block {
    uint32_t reg_base;    /* �쥸�����Υ١������ɥ쥹 */
    uint8_t  lcr_val;     /* �⡼�ɥ쥸������������   */
    uint8_t  dlm_val;     /* �ܡ��졼�Ⱦ�̤�������   */
    uint8_t  dll_val;     /* �ܡ��졼�Ȳ��̤�������   */
    uint8_t intno;        /* ������ֹ� */
} SIOPINIB;

/*
 *  ���ꥢ��I/O�ݡ��ȴ����֥�å������
 */
struct sio_port_control_block {
    const SIOPINIB  *p_siopinib; /* ���ꥢ��I/O�ݡ��Ƚ�����֥�å� */
    intptr_t         exinf;      /* ��ĥ���� */
    bool_t             openflag;   /* �����ץ�Ѥߥե饰 */
    bool_t             sendflag;   /* ��������ߥ��͡��֥�ե饰 */
    bool_t             getready;   /* ʸ��������������� */
    bool_t             putready;   /* ʸ���������Ǥ������ */
};


/*
 * ���ꥢ��I/O�ݡ��Ƚ�����֥�å�
 */
const SIOPINIB siopinib_table[TNUM_SIOP] = {
    {UART2_CSA_BASE, LCR_VAL, DLM_VAL, DLL_VAL, INTNO_SIO0},
#if TNUM_SIOP > 2
    {UART2_CSB_BASE, LCR_VAL, DLM_VAL, DLL_VAL, INTNO_SIO1},
#endif /* TNUM_SIOP > 2 */
#if TNUM_SIOP > 3
    {UART2_CSC_BASE, LCR_VAL, DLM_VAL, DLL_VAL, INTNO_SIO2},
#endif /* TNUM_SIOP > 3 */
#if TNUM_SIOP > 4
    {UART2_CSD_BASE, LCR_VAL, DLM_VAL, DLL_VAL, INTNO_SIO3},
#endif /* TNUM_SIOP > 4 */
};

/*
 *  ���ꥢ��I/O�ݡ��Ƚ�����֥�å��μ�Ф�
 */
#define INDEX_SIOPINIB(siopid)  ((uint_t)((siopid) - 1))
#define get_siopinib(siopid)  (&(siopinib_table[INDEX_SIOPINIB(siopid)]))

/*
 *  ���ꥢ��I/O�ݡ��ȴ����֥�å��Υ��ꥢ
 */
SIOPCB  siopcb_table[TNUM_SIOP];

/*
 *  ���ꥢ��I/O�ݡ���ID��������֥�å�����Ф�����Υޥ���
 */
#define INDEX_SIOP(siopid)  ((uint_t)((siopid) - 1))
#define get_siopcb(siopid)  (&(siopcb_table[INDEX_SIOP(siopid)]))

Inline void
tl16c554a_write(uint32_t addr, uint32_t offset, uint8_t val)
{
    sil_wrb_mem((void*)(addr + offset),val);
}

Inline uint8_t
tl16c554a_read(uint32_t addr, uint32_t offset)
{
    return(sil_reb_mem((void*)(addr + offset)));
}

/*
 * ʸ�������������?
 */
Inline bool_t
tl16c554a_getready(SIOPCB *p_siopcb)
{
    uint16_t status;

    status = tl16c554a_read(p_siopcb->p_siopinib->reg_base, TL16C554A_LSR);

    return((status & LSR_DR));
}

/*
 * ʸ���������Ǥ��뤫?
 */
Inline bool_t
tl16c554a_putready(SIOPCB *p_siopcb)
{
    uint16_t status;

    status = tl16c554a_read(p_siopcb->p_siopinib->reg_base, TL16C554A_LSR);

    return (status & LSR_THRE);
}

/*
 *  ��������ʸ���μ��Ф�
 */
Inline uint8_t
tl16c554a_getchar(SIOPCB *p_siopcb)
{
    return(tl16c554a_read(p_siopcb->p_siopinib->reg_base, TL16C554A_RBR));
}

/*
 *  ��������ʸ���ν񤭹���
 */
Inline void
tl16c554a_putchar(SIOPCB *p_siopcb, uint8_t c)
{

    tl16c554a_write(p_siopcb->p_siopinib->reg_base, TL16C554A_THR, c);
}

/*
 *  ��������ߵ���
 */
Inline void
tl16c554a_enable_send(SIOPCB *p_siopcb)
{
    tl16c554a_write(p_siopcb->p_siopinib->reg_base, TL16C554A_IER,
                (tl16c554a_read(p_siopcb->p_siopinib->reg_base, TL16C554A_IER) | IER_TX));
}

/*
 *  ��������߶ػ�
 */
Inline void
tl16c554a_disable_send(SIOPCB *p_siopcb)
{
    tl16c554a_write(p_siopcb->p_siopinib->reg_base, TL16C554A_IER,
                (tl16c554a_read(p_siopcb->p_siopinib->reg_base, TL16C554A_IER) & ~IER_TX));

}

/*
 *  ��������ߵ���
 */
Inline void
tl16c554a_enable_rcv(SIOPCB *p_siopcb)
{
    tl16c554a_write(p_siopcb->p_siopinib->reg_base, TL16C554A_IER,
                (tl16c554a_read(p_siopcb->p_siopinib->reg_base, TL16C554A_IER) | IER_RX));
}

/*
 *  ��������߶ػ�
 */
Inline void
tl16c554a_disable_rcv(SIOPCB *p_siopcb)
{
    tl16c554a_write(p_siopcb->p_siopinib->reg_base, TL16C554A_IER,
                (tl16c554a_read(p_siopcb->p_siopinib->reg_base, TL16C554A_IER) & ~IER_RX));

}

/*
 *  SIO�ɥ饤�Фν�����롼����
 */
void
tl16c554a_initialize(void)
{
    SIOPCB  *p_siopcb;
    uint_t  i;

    /*
     *  ���ꥢ��I/O�ݡ��ȴ����֥�å��ν����
     */
    for (p_siopcb = siopcb_table, i = 0; i < TNUM_SIOP; p_siopcb++, i++) {
        p_siopcb->p_siopinib = &(siopinib_table[i]);
        p_siopcb->openflag = false;
        p_siopcb->sendflag = false;
    }
}


/*
 * SIOPINIB�ν����
 */
void
tl16c554a_init_siopinib(const SIOPINIB *p_siopinib)
{
    /* �����߶ػ� */
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_IER, 0x00);

    /*
     *  ʬ���������
     */
    /* Divisor Enable */
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_LCR,
                (tl16c554a_read(p_siopinib->reg_base, TL16C554A_LCR) | LCR_BKSE));
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_DLL, p_siopinib->dll_val);
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_DLM, p_siopinib->dlm_val);

    /* Divisor Disable */
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_LCR,
                (tl16c554a_read(p_siopinib->reg_base, TL16C554A_LCR) & ~LCR_BKSE));

    /* �⡼������ */
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_LCR, p_siopinib->lcr_val);

    /* no RTS/DTR */
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_MCR, MCR_VAL);

    /* FIFO Disable */
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_FCR, FCR_FIFO_DISABLE);
//    tl16c554a_write(p_siopinib->reg_base, TL16C554A_FCR, FCR_VAL);

}


/*
 * �����ץ󤷤Ƥ���ݡ��Ȥ����뤫?
 */
bool_t
tl16c554a_openflag(ID siopid)
{
	SIOPCB          *p_siopcb;

	p_siopcb = get_siopcb(siopid);

	return(p_siopcb->openflag);
}


/*
 * ���ꥢ��I/O�ݡ��ȤΥ����ץ�
 */
SIOPCB *
tl16c554a_opn_por(ID siopid, intptr_t exinf)
{
    SIOPCB      *p_siopcb;
    const SIOPINIB  *p_siopinib;

    p_siopcb = get_siopcb(siopid);
    p_siopinib = p_siopcb->p_siopinib;

    /*
     * �����
     */
    tl16c554a_init_siopinib(p_siopcb->p_siopinib);

    /* ��������߶ػ� */
    tl16c554a_enable_rcv(p_siopcb);
    /* ��������߶ػ� */
    tl16c554a_disable_rcv(p_siopcb);

    p_siopcb->exinf = exinf;
    p_siopcb->getready = p_siopcb->putready = false;
    p_siopcb->openflag = true;

    return(p_siopcb);
}

/*
 *  ���ꥢ��I/O�ݡ��ȤΥ�����
 */
void
tl16c554a_cls_por(SIOPCB *p_siopcb)
{
    /* ����߶ػ� */
    tl16c554a_write(p_siopcb->p_siopinib->reg_base, TL16C554A_IER, 0x00);
    p_siopcb->openflag = false;
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥؤ�ʸ������
 */
bool_t
tl16c554a_snd_chr(SIOPCB *p_siopcb, char_t c)
{
    if (tl16c554a_putready(p_siopcb)){
        tl16c554a_putchar(p_siopcb, c);
        return(true);
    }
    return(false);
//	while(!tl16c554a_putready(p_siopcb));
//	tl16c554a_putchar(p_siopcb, c);
//	return(true);
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ����ʸ������
 */
int
tl16c554a_rcv_chr(SIOPCB *p_siopcb)
{
    if (tl16c554a_getready(p_siopcb)) {
        return((int)(uint8_t)tl16c554a_getchar(p_siopcb));
    }
    return(-1);
//	while(!tl16c554a_getready(p_siopcb));
//	return((int)(uint8_t)tl16c554a_getchar(p_siopcb));
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ���Υ�����Хå��ε���
 */
void
tl16c554a_ena_cbr(SIOPCB *p_siopcb, uint_t cbrtn)
{
    switch (cbrtn) {
        case SIO_RDY_SND:
            tl16c554a_enable_send(p_siopcb);
            break;
        case SIO_RDY_RCV:
            tl16c554a_enable_rcv(p_siopcb);
            break;
    }
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ���Υ�����Хå��ζػ�
 */
void
tl16c554a_dis_cbr(SIOPCB *p_siopcb, uint_t cbrtn)
{
    switch (cbrtn) {
        case SIO_RDY_SND:
            tl16c554a_disable_send(p_siopcb);
            break;
        case SIO_RDY_RCV:
            tl16c554a_disable_rcv(p_siopcb);
            break;
    }
}

/*
 *  ���ꥢ��I/O�ݡ��Ȥ��Ф������߽���
 */
static void
tl16c554a_isr_siop(SIOPCB *p_siopcb)
{
    if (tl16c554a_getready(p_siopcb)) {
        /*
         *  �������Υ�����Хå��롼�����ƤӽФ���
         */
        tl16c554a_irdy_rcv(p_siopcb->exinf);
    }
    if (tl16c554a_putready(p_siopcb)) {
        /*
         *  ������ǽ������Хå��롼�����ƤӽФ���
         */
        tl16c554a_irdy_snd(p_siopcb->exinf);
    }
}

/*
 *  SIO�γ���ߥ����ӥ��롼����
 */
void
tl16c554a_isr(intptr_t exinf)
{
    tl16c554a_isr_siop(get_siopcb(exinf));
}

/*
 *  SIOPCB�ؤΥݥ��󥿤���ݡ����ֹ���֤�
 */
ID
tl16c554a_get_siopid(SIOPCB *p_siopcb)
{
	return (ID)((p_siopcb - siopcb_table) + 1);
}

/*
 *  ������ֹ���֤�
 */
uint8_t
tl16c554a_get_intno(SIOPCB *p_siopcb)
{
	return p_siopcb->p_siopinib->intno;
}
