/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel    
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN    
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  @(#) $Id: ns16550.c 91 2009-01-25 16:50:43Z ertl-honda $
 */

/*
 *     ST16C2550 用 簡易SIOドライバ
 */

#include <sil.h>
#include "target_syssvc.h"
#include "ns16550.h"

/*
 *  コールバックルーチンの識別番号
 */
#define SIO_RDY_SND	1U		/* 送信可能コールバック */
#define SIO_RDY_RCV	2U		/* 受信通知コールバック */

/*
 * 各レジスタのオフセット
 */
#define NS16550_RHR   0x00   /* B  LCR bit7=0 Read  */
#define NS16550_THR   0x00   /* B  LCR bit7=0 Write */
#define NS16550_DLL   0x00   /* B  LCR bit7=1       */
#define NS16550_IER   0x04   /* B  LCR bit7=0       */
#define NS16550_DLM   0x04   /* B  LCR bit7=1       */
#define NS16550_IIR   0x08   /* B  Read  */
#define	NS16550_LCR   0x0c   /* B  */
#define NS16550_MCR   0x10   /* B  */
#define NS16550_LSR   0x14   /* B  Read */
#define NS16550_MSR   0x18   /* B  Read */
#define NS16550_SCR   0x1c   /* B  */
#define NS16550_FCR   0x20   /* B  Write */

#define IIR_TX            0x02       /* 送信割り込み発生 */
#define IER_TX            0x02       /* 送信割り込み許可 */
#define IIR_RX            0x01       /* 受信割り込み発生 */
#define IER_RX            0x01       /* 受信割り込み許可 */


#define LCR_DL_MODE       0x80       /* Divisor Enable */
#define LCR_VAL           0x03       /* 8bit,1stop,Noparity,No break */
#define FCR_FIFO_DISABLE  0x00
#define FCR_FIFO_ENABLE   0x01

#define LSR_RX_DATA_READY 0x01
#define LSR_TX_EMPTY      0x20


/*
 *  シリアルI/Oポート初期化ブロックの定義
 */
typedef struct sio_port_initialization_block {
    uint32_t reg_base;    /* レジスタのベースアドレス */
    uint8_t  lcr_val;     /* モードレジスタの設定値   */
    uint8_t  dlm_val;     /* ボーレート上位の設定値   */
    uint8_t  dll_val;     /* ボーレート下位の設定値   */
    uint8_t intno;        /* 割込み番号 */
} SIOPINIB;

/*
 *  シリアルI/Oポート管理ブロックの定義
 */
struct sio_port_control_block {
    const SIOPINIB  *p_siopinib; /* シリアルI/Oポート初期化ブロック */
    intptr_t         exinf;      /* 拡張情報 */
    bool_t             openflag;   /* オープン済みフラグ */
    bool_t             sendflag;   /* 送信割込みイネーブルフラグ */
    bool_t             getready;   /* 文字を受信した状態 */
    bool_t             putready;   /* 文字を送信できる状態 */
};


/*
 * シリアルI/Oポート初期化ブロック
 */
const SIOPINIB siopinib_table[TNUM_SIOP] = {
    {UART0_BASE, UART0_LCR_VAL, UART0_DLM_VAL, UART0_DLL_VAL, DIC_IRQNO_UART0},
#if TNUM_SIOP > 2
    {UART1_BASE, UART1_LCR_VAL, UART1_DLM_VAL, UART1_DLL_VAL, DIC_IRQNO_UART1},
#endif /* TNUM_SIOP > 2 */
#if TNUM_SIOP > 3
    {UART2_BASE, UART1_LCR_VAL, UART1_DLM_VAL, UART1_DLL_VAL, DIC_IRQNO_UART2},
#endif /* TNUM_SIOP > 3 */
#if TNUM_SIOP > 4
    {UART3_BASE, UART1_LCR_VAL, UART1_DLM_VAL, UART1_DLL_VAL, DIC_IRQNO_UART3},
#endif /* TNUM_SIOP > 4 */
};

/*
 *  シリアルI/Oポート初期化ブロックの取出し
 */
#define INDEX_SIOPINIB(siopid)  ((uint_t)((siopid) - 1))
#define get_siopinib(siopid)  (&(siopinib_table[INDEX_SIOPINIB(siopid)]))

/*
 *  シリアルI/Oポート管理ブロックのエリア
 */
SIOPCB  siopcb_table[TNUM_SIOP];

/*
 *  シリアルI/OポートIDから管理ブロックを取り出すためのマクロ
 */
#define INDEX_SIOP(siopid)  ((uint_t)((siopid) - 1))
#define get_siopcb(siopid)  (&(siopcb_table[INDEX_SIOP(siopid)]))

Inline void
ns16550_write(uint32_t addr, uint32_t offset, uint8_t val)
{
    sil_wrb_mem((void*)(addr + offset),val);
}

Inline uint8_t
ns16550_read(uint32_t addr, uint32_t offset)
{
    return(sil_reb_mem((void*)(addr + offset)));
}

/*
 * 文字を受信したか?
 */ 
Inline bool_t
ns16550_getready(SIOPCB *p_siopcb)
{
    uint16_t status;

    status = ns16550_read(p_siopcb->p_siopinib->reg_base, NS16550_LSR);
    
    return((status & LSR_RX_DATA_READY));
}

/*
 * 文字を送信できるか?
 */
Inline bool_t
ns16550_putready(SIOPCB *p_siopcb)
{
    uint16_t status;

    status = ns16550_read(p_siopcb->p_siopinib->reg_base, NS16550_LSR);
    
    return (status & LSR_TX_EMPTY);
}

/*
 *  受信した文字の取り出し
 */
Inline uint8_t
ns16550_getchar(SIOPCB *p_siopcb)
{
    return(ns16550_read(p_siopcb->p_siopinib->reg_base, NS16550_RHR));
}

/*
 *  送信する文字の書き込み
 */
Inline void
ns16550_putchar(SIOPCB *p_siopcb, uint8_t c)
{
    ns16550_write(p_siopcb->p_siopinib->reg_base, NS16550_THR, c);
}

/*
 *  送信割込み許可
 */
Inline void
ns16550_enable_send(SIOPCB *p_siopcb)
{
    ns16550_write(p_siopcb->p_siopinib->reg_base, NS16550_IER,
                (ns16550_read(p_siopcb->p_siopinib->reg_base, NS16550_IER) | IER_TX));
}

/*
 *  送信割込み禁止
 */
Inline void
ns16550_disable_send(SIOPCB *p_siopcb)
{
    ns16550_write(p_siopcb->p_siopinib->reg_base, NS16550_IER,
                (ns16550_read(p_siopcb->p_siopinib->reg_base, NS16550_IER) & ~IER_TX));
                
}

/*
 *  受信割込み許可
 */
Inline void
ns16550_enable_rcv(SIOPCB *p_siopcb)
{
    ns16550_write(p_siopcb->p_siopinib->reg_base, NS16550_IER,
                (ns16550_read(p_siopcb->p_siopinib->reg_base, NS16550_IER) | IER_RX));
}

/*
 *  受信割込み禁止
 */
Inline void
ns16550_disable_rcv(SIOPCB *p_siopcb)
{
    ns16550_write(p_siopcb->p_siopinib->reg_base, NS16550_IER,
                (ns16550_read(p_siopcb->p_siopinib->reg_base, NS16550_IER) & ~IER_RX));
                
}

/*
 *  SIOドライバの初期化ルーチン
 */
void
ns16550_initialize()
{
    SIOPCB  *p_siopcb;
    uint_t  i;

    /*
     *  シリアルI/Oポート管理ブロックの初期化
     */
    for (p_siopcb = siopcb_table, i = 0; i < TNUM_SIOP; p_siopcb++, i++) {
        p_siopcb->p_siopinib = &(siopinib_table[i]);
        p_siopcb->openflag = false;
        p_siopcb->sendflag = false;
    }    
}

    
/*
 * SIOPINIBの初期化
 */
void
ns16550_init_siopinib(const SIOPINIB *p_siopinib)
{
    /*
     *  分周比の設定
     */
    /* Divisor Enable */
    ns16550_write(p_siopinib->reg_base, NS16550_LCR,
                (ns16550_read(p_siopinib->reg_base, NS16550_LCR) | LCR_DL_MODE));
    ns16550_write(p_siopinib->reg_base, NS16550_DLL, p_siopinib->dll_val);
    ns16550_write(p_siopinib->reg_base, NS16550_DLM, p_siopinib->dlm_val);

    /* Divisor Disable */
    ns16550_write(p_siopinib->reg_base, NS16550_LCR,
                (ns16550_read(p_siopinib->reg_base, NS16550_LCR) & ~LCR_DL_MODE));

    /* モード設定 */
    ns16550_write(p_siopinib->reg_base, NS16550_LCR, p_siopinib->lcr_val);

    /* FIFO Disable */
    ns16550_write(p_siopinib->reg_base, NS16550_FCR, FCR_FIFO_DISABLE);

    /* 割込み禁止 */
    ns16550_write(p_siopinib->reg_base, NS16550_IER, 0x00);
}


/*
 * オープンしているポートがあるか?
 */
bool_t
ns16550_openflag(ID siopid)
{
	SIOPCB          *p_siopcb;

	p_siopcb = get_siopcb(siopid);

	return(p_siopcb->openflag);
}


/*
 * シリアルI/Oポートのオープン
 */
SIOPCB *
ns16550_opn_por(ID siopid, intptr_t exinf)
{
    SIOPCB      *p_siopcb;
    const SIOPINIB  *p_siopinib;

    p_siopcb = get_siopcb(siopid);
    p_siopinib = p_siopcb->p_siopinib;
    
    /*
     * 初期化
     */
    ns16550_init_siopinib(p_siopcb->p_siopinib);
    
    /* 受信割込み禁止 */
    ns16550_enable_rcv(p_siopcb);
    /* 送信割込み禁止 */
    ns16550_disable_rcv(p_siopcb);
    
    p_siopcb->exinf = exinf;
    p_siopcb->getready = p_siopcb->putready = false;
    p_siopcb->openflag = true;

    return(p_siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
ns16550_cls_por(SIOPCB *p_siopcb)
{
    /* 割込み禁止 */
    ns16550_write(p_siopcb->p_siopinib->reg_base, NS16550_IER, 0x00);   
    p_siopcb->openflag = false;
}

/*
 *  シリアルI/Oポートへの文字送信
 */
bool_t
ns16550_snd_chr(SIOPCB *p_siopcb, char_t c)
{
    if (ns16550_putready(p_siopcb)){
        ns16550_putchar(p_siopcb, c);
        return(true);
    }
    return(false);
}

/*
 *  シリアルI/Oポートからの文字受信
 */
int
ns16550_rcv_chr(SIOPCB *p_siopcb)
{
    if (ns16550_getready(p_siopcb)) {
        return((int)(uint8_t)ns16550_getchar(p_siopcb));
    }
    return(-1);
}

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
void
ns16550_ena_cbr(SIOPCB *p_siopcb, uint_t cbrtn)
{
    switch (cbrtn) {
        case SIO_RDY_SND:
            ns16550_enable_send(p_siopcb);
            break;
        case SIO_RDY_RCV:
            ns16550_enable_rcv(p_siopcb);
            break;
    }
}

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
void
ns16550_dis_cbr(SIOPCB *p_siopcb, uint_t cbrtn)
{
    switch (cbrtn) {
        case SIO_RDY_SND:
            ns16550_disable_send(p_siopcb);
            break;
        case SIO_RDY_RCV:
            ns16550_disable_rcv(p_siopcb);
            break;
    }
}

/*
 *  シリアルI/Oポートに対する割込み処理
 */
static void
ns16550_isr_siop(SIOPCB *p_siopcb)
{
    if (ns16550_getready(p_siopcb)) {
        /*
         *  受信通知コールバックルーチンを呼び出す．
         */
        ns16550_irdy_rcv(p_siopcb->exinf);
    }
    if (ns16550_putready(p_siopcb)) {
        /*
         *  送信可能コールバックルーチンを呼び出す．
         */
        ns16550_irdy_snd(p_siopcb->exinf);
    }
}

/*
 *  SIOの割込みサービスルーチン
 */
void
ns16550_isr(intptr_t exinf)
{
    ns16550_isr_siop(get_siopcb(exinf));
}

/*
 *  SIOPCBへのポインタからポート番号を返す
 */
ID
ns16550_get_siopid(SIOPCB *p_siopcb)
{
	return (ID)((p_siopcb - siopcb_table) + 1);
}

/*
 *  割込み番号を返す
 */
uint8_t
ns16550_get_intno(SIOPCB *p_siopcb)
{
	return p_siopcb->p_siopinib->intno;
}
