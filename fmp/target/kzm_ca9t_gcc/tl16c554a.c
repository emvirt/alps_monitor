/*
 *     TL16C554A 用 簡易SIOドライバ
 */

#include <sil.h>
#include "target_syssvc.h"
#include "tl16c554a.h"


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
 * 文字を受信したか?
 */
Inline bool_t
tl16c554a_getready(SIOPCB *p_siopcb)
{
    uint16_t status;

    status = tl16c554a_read(p_siopcb->p_siopinib->reg_base, TL16C554A_LSR);

    return((status & LSR_DR));
}

/*
 * 文字を送信できるか?
 */
Inline bool_t
tl16c554a_putready(SIOPCB *p_siopcb)
{
    uint16_t status;

    status = tl16c554a_read(p_siopcb->p_siopinib->reg_base, TL16C554A_LSR);

    return (status & LSR_THRE);
}

/*
 *  受信した文字の取り出し
 */
Inline uint8_t
tl16c554a_getchar(SIOPCB *p_siopcb)
{
    return(tl16c554a_read(p_siopcb->p_siopinib->reg_base, TL16C554A_RBR));
}

/*
 *  送信する文字の書き込み
 */
Inline void
tl16c554a_putchar(SIOPCB *p_siopcb, uint8_t c)
{

    tl16c554a_write(p_siopcb->p_siopinib->reg_base, TL16C554A_THR, c);
}

/*
 *  送信割込み許可
 */
Inline void
tl16c554a_enable_send(SIOPCB *p_siopcb)
{
    tl16c554a_write(p_siopcb->p_siopinib->reg_base, TL16C554A_IER,
                (tl16c554a_read(p_siopcb->p_siopinib->reg_base, TL16C554A_IER) | IER_TX));
}

/*
 *  送信割込み禁止
 */
Inline void
tl16c554a_disable_send(SIOPCB *p_siopcb)
{
    tl16c554a_write(p_siopcb->p_siopinib->reg_base, TL16C554A_IER,
                (tl16c554a_read(p_siopcb->p_siopinib->reg_base, TL16C554A_IER) & ~IER_TX));

}

/*
 *  受信割込み許可
 */
Inline void
tl16c554a_enable_rcv(SIOPCB *p_siopcb)
{
    tl16c554a_write(p_siopcb->p_siopinib->reg_base, TL16C554A_IER,
                (tl16c554a_read(p_siopcb->p_siopinib->reg_base, TL16C554A_IER) | IER_RX));
}

/*
 *  受信割込み禁止
 */
Inline void
tl16c554a_disable_rcv(SIOPCB *p_siopcb)
{
    tl16c554a_write(p_siopcb->p_siopinib->reg_base, TL16C554A_IER,
                (tl16c554a_read(p_siopcb->p_siopinib->reg_base, TL16C554A_IER) & ~IER_RX));

}

/*
 *  SIOドライバの初期化ルーチン
 */
void
tl16c554a_initialize(void)
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
tl16c554a_init_siopinib(const SIOPINIB *p_siopinib)
{
    /* 割り込み禁止 */
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_IER, 0x00);

    /*
     *  分周比の設定
     */
    /* Divisor Enable */
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_LCR,
                (tl16c554a_read(p_siopinib->reg_base, TL16C554A_LCR) | LCR_BKSE));
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_DLL, p_siopinib->dll_val);
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_DLM, p_siopinib->dlm_val);

    /* Divisor Disable */
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_LCR,
                (tl16c554a_read(p_siopinib->reg_base, TL16C554A_LCR) & ~LCR_BKSE));

    /* モード設定 */
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_LCR, p_siopinib->lcr_val);

    /* no RTS/DTR */
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_MCR, MCR_VAL);

    /* FIFO Disable */
    tl16c554a_write(p_siopinib->reg_base, TL16C554A_FCR, FCR_FIFO_DISABLE);
//    tl16c554a_write(p_siopinib->reg_base, TL16C554A_FCR, FCR_VAL);

}


/*
 * オープンしているポートがあるか?
 */
bool_t
tl16c554a_openflag(ID siopid)
{
	SIOPCB          *p_siopcb;

	p_siopcb = get_siopcb(siopid);

	return(p_siopcb->openflag);
}


/*
 * シリアルI/Oポートのオープン
 */
SIOPCB *
tl16c554a_opn_por(ID siopid, intptr_t exinf)
{
    SIOPCB      *p_siopcb;
    const SIOPINIB  *p_siopinib;

    p_siopcb = get_siopcb(siopid);
    p_siopinib = p_siopcb->p_siopinib;

    /*
     * 初期化
     */
    tl16c554a_init_siopinib(p_siopcb->p_siopinib);

    /* 受信割込み禁止 */
    tl16c554a_enable_rcv(p_siopcb);
    /* 送信割込み禁止 */
    tl16c554a_disable_rcv(p_siopcb);

    p_siopcb->exinf = exinf;
    p_siopcb->getready = p_siopcb->putready = false;
    p_siopcb->openflag = true;

    return(p_siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
tl16c554a_cls_por(SIOPCB *p_siopcb)
{
    /* 割込み禁止 */
    tl16c554a_write(p_siopcb->p_siopinib->reg_base, TL16C554A_IER, 0x00);
    p_siopcb->openflag = false;
}

/*
 *  シリアルI/Oポートへの文字送信
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
 *  シリアルI/Oポートからの文字受信
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
 *  シリアルI/Oポートからのコールバックの許可
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
 *  シリアルI/Oポートからのコールバックの禁止
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
 *  シリアルI/Oポートに対する割込み処理
 */
static void
tl16c554a_isr_siop(SIOPCB *p_siopcb)
{
    if (tl16c554a_getready(p_siopcb)) {
        /*
         *  受信通知コールバックルーチンを呼び出す．
         */
        tl16c554a_irdy_rcv(p_siopcb->exinf);
    }
    if (tl16c554a_putready(p_siopcb)) {
        /*
         *  送信可能コールバックルーチンを呼び出す．
         */
        tl16c554a_irdy_snd(p_siopcb->exinf);
    }
}

/*
 *  SIOの割込みサービスルーチン
 */
void
tl16c554a_isr(intptr_t exinf)
{
    tl16c554a_isr_siop(get_siopcb(exinf));
}

/*
 *  SIOPCBへのポインタからポート番号を返す
 */
ID
tl16c554a_get_siopid(SIOPCB *p_siopcb)
{
	return (ID)((p_siopcb - siopcb_table) + 1);
}

/*
 *  割込み番号を返す
 */
uint8_t
tl16c554a_get_intno(SIOPCB *p_siopcb)
{
	return p_siopcb->p_siopinib->intno;
}
