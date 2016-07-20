/*
 *   TL16C554A 用 簡易SIOドライバ
 */
#ifndef TOPPERS_TL16C554A_H
#define TOPPERS_TL16C554A_H

/*
 *  シリアルI/Oポート数の定義
 */
#define TNUM_SIOP		1		/* サポートするシリアルI/Oポートの数 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  シリアルI/Oポート管理ブロックの定義
 */
typedef struct sio_port_control_block	SIOPCB;

/*
 *  コールバックルーチンの識別番号
 */
#define SIO_RDY_SND	1U		/* 送信可能コールバック */
#define SIO_RDY_RCV	2U		/* 受信通知コールバック */

/*
 * 各レジスタのオフセット
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
 * 各レジスタの設定値
 */
#define IIR_TX            0x02       /* 送信割り込み発生 */
#define IER_TX            0x02       /* 送信割り込み許可 */
#define IIR_RX            0x01       /* 受信割り込み発生 */
#define IER_RX            0x01       /* 受信割り込み許可 */

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

/* baud divisorの値の計算
     38400*16 = 614400,     732800[Hz] / 614400 = 12 */
#define DLM_VAL       (0x0c >> 8) & 0xff	/* Baud rate の上位値の設定 */
#define DLL_VAL       0x0c & 0xff		/* Baud rate の下位値の設定 */

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
 * カーネル起動時用の初期化(sys_putcを使用するため)
 */
extern void tl16c554a_init(void);

/*
 *  シリアルI/Oポートへのポーリングでの出力
 */
extern void tl16c554a_pol_putc(char_t c, ID siopid);

/*
 *  SIOドライバの初期化ルーチン
 */
extern void tl16c554a_initialize(void);

/*
 *  オープンしているポートがあるか？
 */
extern bool_t tl16c554a_openflag(ID siopid);

/*
 *  シリアルI/Oポートのオープン
 */
extern SIOPCB  *tl16c554a_opn_por(ID siopid, intptr_t exinf);

/*
 *  シリアルI/Oポートのクローズ
 */
extern void tl16c554a_cls_por(SIOPCB *siopcb);

/*
 *  シリアルI/Oポートへの文字送信
 */
extern bool_t tl16c554a_snd_chr(SIOPCB *siopcb, char_t c);

/*
 *  シリアルI/Oポートからの文字受信
 */
extern int_t tl16c554a_rcv_chr(SIOPCB *siopcb);

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
extern void tl16c554a_ena_cbr(SIOPCB *siopcb, uint_t cbrtn);

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
extern void tl16c554a_dis_cbr(SIOPCB *siopcb, uint_t cbrtn);

/*
 *  SIOの割込みサービスルーチン
 */
extern void tl16c554a_isr(intptr_t exinf);

/*
 *  シリアルI/Oポートからの送信可能コールバック
 */
extern void tl16c554a_irdy_snd(intptr_t exinf);

/*
 *  シリアルI/Oポートからの受信通知コールバック
 */
extern void tl16c554a_irdy_rcv(intptr_t exinf);

/*
 *  SIOPCBへのポインタからポート番号を返す
 */
extern ID tl16c554a_get_siopid(SIOPCB *p_siopcb);

/*
 *  割込み番号を返す
 */
extern uint8_t tl16c554a_get_intno(SIOPCB *p_siopcb);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_TL16C554A_H */
