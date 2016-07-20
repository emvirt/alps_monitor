/*
 *   TL16C554A �� �ʰ�SIO�ɥ饤��
 */
#ifndef TOPPERS_TL16C554A_H
#define TOPPERS_TL16C554A_H

/*
 *  ���ꥢ��I/O�ݡ��ȿ������
 */
#define TNUM_SIOP		1		/* ���ݡ��Ȥ��륷�ꥢ��I/O�ݡ��Ȥο� */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ���ꥢ��I/O�ݡ��ȴ����֥�å������
 */
typedef struct sio_port_control_block	SIOPCB;

/*
 *  ������Хå��롼����μ����ֹ�
 */
#define SIO_RDY_SND	1U		/* ������ǽ������Хå� */
#define SIO_RDY_RCV	2U		/* �������Υ�����Хå� */

/*
 * �ƥ쥸�����Υ��ե��å�
 */
#define TL16C554A_RBR   0x00   /* B  LCR bit7=0 Read  */
#define TL16C554A_THR   0x00   /* B  LCR bit7=0 Write */
#define TL16C554A_DLL   0x00   /* B  LCR bit7=1       */
#define TL16C554A_IER   0x04   /* B  LCR bit7=0       */
#define TL16C554A_DLM   0x04   /* B  LCR bit7=1       */
#define TL16C554A_FCR   0x08   /* B  Write */
#define TL16C554A_IIR   0x08   /* B  Read  */
#define	TL16C554A_LCR   0x0c   /* B  */
#define TL16C554A_MCR   0x10   /* B  */
#define TL16C554A_LSR   0x14   /* B  Read */
#define TL16C554A_MSR   0x18   /* B  Read */
#define TL16C554A_SCR   0x1c   /* B  */



/*
 * �ƥ쥸������������
 */
#define IIR_TX            0x02       /* ����������ȯ�� */
#define IER_TX            0x02       /* ���������ߵ��� */
#define IIR_RX            0x01       /* ����������ȯ�� */
#define IER_RX            0x01       /* ���������ߵ��� */

#define MCR_RTS         0x02	/* Readyu to Send */
#define MCR_LOOP	0x10	/* Local loopback mode enable */
#define MCR_DTR         0x01    /* noton 8245 duart */
#define MCR_DMA_EN      0x04    /* noton 8245 duart */
#define MCR_TX_DFR      0x08    /* noton 8245 duart */
#define MCR_VAL 	MCR_DTR | MCR_RTS 

#define FCR_FIFO_DISABLE	0x00
#define FCR_FIFO_ENABLE		0x01
#define FCR_RXSR        	0x02	/*reciever soft reset */
#define FCR_TXSR        	0x04	/*transmitter soft reset */
#define FCR_DMS			0x08	/* DMA Mode Select */

#define LCR_BKSE    0x80	/* Bank select enable - aka DLAB on 8245 */
#define LCR_8N1     0x03
#define LCR_VAL LCR_8N1					/* 8 data, 1 stop, no parity */
#define FCR_VAL (FCR_FIFO_ENABLE | FCR_RXSR | FCR_TXSR)	/* Clear & enable FIFOs */

/* baud divisor���ͤη׻�
     38400*16 = 614400,     732800[Hz] / 614400 = 12 */
#define DLM_VAL       (0x0c >> 8) & 0xff	/* Baud rate �ξ���ͤ����� */
#define DLL_VAL       0x0c & 0xff		/* Baud rate �β����ͤ����� */

#define LCR_WLS_MSK	0x03		/* character length select mask */
#define LCR_WLS_5	0x00		/* 5 bit character length */
#define LCR_WLS_6	0x01		/* 6 bit character length */
#define LCR_WLS_7	0x02		/* 7 bit character length */
#define LCR_WLS_8	0x03		/* 8 bit character length */
#define LCR_STB		0x04		/* Number of stop Bits, off = 1, on = 1.5 or 2) */
#define LCR_PEN		0x08		/* Parity eneble */
#define LCR_EPS		0x10		/* Even Parity Select */
#define LCR_STKP	0x20		/* Stick Parity */
#define LCR_SBRK	0x40		/* Set Break */
#define LCR_BKSE	0x80		/* Bank select enable */

#define LSR_DR		0x01		/* Data ready */
#define LSR_OE		0x02		/* Overrun */
#define LSR_PE		0x04		/* Parity error */
#define LSR_FE		0x08		/* Framing error */
#define LSR_BI		0x10		/* Break */
#define LSR_THRE	0x20		/* Xmit holding register empty */
#define LSR_TEMT	0x40		/* Xmitter empty */
#define LSR_ERR		0x80		/* Error */



/*
 * �����ͥ뵯ư���Ѥν����(sys_putc����Ѥ��뤿��)
 */
extern void tl16c554a_init(void);

/*
 *  ���ꥢ��I/O�ݡ��ȤؤΥݡ���󥰤Ǥν���
 */
extern void tl16c554a_pol_putc(char_t c, ID siopid);

/*
 *  SIO�ɥ饤�Фν�����롼����
 */
extern void tl16c554a_initialize(void);

/*
 *  �����ץ󤷤Ƥ���ݡ��Ȥ����뤫��
 */
extern bool_t tl16c554a_openflag(ID siopid);

/*
 *  ���ꥢ��I/O�ݡ��ȤΥ����ץ�
 */
extern SIOPCB  *tl16c554a_opn_por(ID siopid, intptr_t exinf);

/*
 *  ���ꥢ��I/O�ݡ��ȤΥ�����
 */
extern void tl16c554a_cls_por(SIOPCB *siopcb);

/*
 *  ���ꥢ��I/O�ݡ��Ȥؤ�ʸ������
 */
extern bool_t tl16c554a_snd_chr(SIOPCB *siopcb, char_t c);

/*
 *  ���ꥢ��I/O�ݡ��Ȥ����ʸ������
 */
extern int_t tl16c554a_rcv_chr(SIOPCB *siopcb);

/*
 *  ���ꥢ��I/O�ݡ��Ȥ���Υ�����Хå��ε���
 */
extern void tl16c554a_ena_cbr(SIOPCB *siopcb, uint_t cbrtn);

/*
 *  ���ꥢ��I/O�ݡ��Ȥ���Υ�����Хå��ζػ�
 */
extern void tl16c554a_dis_cbr(SIOPCB *siopcb, uint_t cbrtn);

/*
 *  SIO�γ���ߥ����ӥ��롼����
 */
extern void tl16c554a_isr(intptr_t exinf);

/*
 *  ���ꥢ��I/O�ݡ��Ȥ����������ǽ������Хå�
 */
extern void tl16c554a_irdy_snd(intptr_t exinf);

/*
 *  ���ꥢ��I/O�ݡ��Ȥ���μ������Υ�����Хå�
 */
extern void tl16c554a_irdy_rcv(intptr_t exinf);

/*
 *  SIOPCB�ؤΥݥ��󥿤���ݡ����ֹ���֤�
 */
extern ID tl16c554a_get_siopid(SIOPCB *p_siopcb);

/*
 *  ������ֹ���֤�
 */
extern uint8_t tl16c554a_get_intno(SIOPCB *p_siopcb);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_TL16C554A_H */
