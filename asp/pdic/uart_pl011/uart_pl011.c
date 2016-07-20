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
 *  @(#) $Id: uart_pl011.c 324 2007-07-17 06:00:17Z hiro $
 */

/*
 *   PL011用 簡易SIOドライバ
 */

#include <sil.h>
#include "target_syssvc.h"
#include "target_serial.h"
#include "uart_pl011.h"

/*
 *  シリアルI/Oポート初期化ブロックの定義
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
 *  シリアルI/Oポート管理ブロックの定義
 */
struct sio_port_control_block {
    const SIOPINIB  *siopinib;  /* シリアルI/Oポート初期化ブロック */
    intptr_t  exinf;            /* 拡張情報 */
    bool_t    openflag;           /* オープン済みフラグ */
    bool_t    sendflag;           /* 送信割込みイネーブルフラグ */
    bool_t    getready;           /* 文字を受信した状態 */
    bool_t    putready;           /* 文字を送信できる状態 */
};

/*
 *  シリアルI/Oポート初期化ブロック
 */

/*
 * シリアルI/Oポート初期化ブロック
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
 *  シリアルI/Oポート管理ブロックのエリア
 */
SIOPCB	siopcb_table[TNUM_SIOP];

/*
 *  シリアルI/OポートIDから管理ブロックを取り出すためのマクロ
 */
#define INDEX_SIOP(siopid)	((uint_t)((siopid) - 1))
#define get_siopcb(siopid)	(&(siopcb_table[INDEX_SIOP(siopid)]))

/*
 *  文字を受信できるか？
 */
Inline bool_t
uart_pl011_getready(SIOPCB *p_siopcb)
{
    return(((sil_rew_mem((void *)(p_siopcb->siopinib->fr)) & UFR_RXFE) == 0));
}

/*
 *  文字を送信できるか？
 */
Inline bool_t
uart_pl011_putready(SIOPCB *p_siopcb)
{
    return((sil_rew_mem((void *)(p_siopcb->siopinib->fr)) & UFR_TXFF) == 0);
}

/*
 *  受信した文字の取出し
 */
Inline char_t
uart_pl011_getchar(SIOPCB *p_siopcb)
{
    return(sil_rew_mem((void *)(p_siopcb->siopinib->dr)));
}

/*
 *  送信する文字の書込み
 */
Inline void
uart_pl011_putchar(SIOPCB *p_siopcb, char_t c)
{
    sil_wrw_mem((void *)(p_siopcb->siopinib->dr), c);
}

/*
 *  送信割込み許可
 */
Inline void
uart_pl011_enable_send(SIOPCB *p_siopcb)
{
    sil_wrw_mem((void *)(p_siopcb->siopinib->imsc),
        (sil_rew_mem((void *)(p_siopcb->siopinib->imsc)) | UART_IMSC_TXIM));
}

/*
 *  送信割込み禁止
 */
Inline void
uart_pl011_disable_send(SIOPCB *p_siopcb)
{
     sil_wrw_mem((void *)(p_siopcb->siopinib->imsc),
        (sil_rew_mem((void *)(p_siopcb->siopinib->imsc)) & ~UART_IMSC_TXIM));
}


/*
 *  受信割込み許可
 */
Inline void
uart_pl011_enable_rcv(SIOPCB *p_siopcb)
{
    sil_wrw_mem((void *)(p_siopcb->siopinib->imsc),
        (sil_rew_mem((void *)(p_siopcb->siopinib->imsc)) | UART_IMSC_RXIM));
}

/*
 *  受信割込み禁止
 */
Inline void
uart_pl011_disable_rcv(SIOPCB *p_siopcb)
{
    sil_wrw_mem((void *)(p_siopcb->siopinib->imsc),
        (sil_rew_mem((void *)(p_siopcb->siopinib->imsc)) & ~UART_IMSC_RXIM));
}

/*
 *  SIOドライバの初期化
 */
void
uart_pl011_initialize(void)
{
	SIOPCB	*p_siopcb;
	uint_t	i;

	/*
	 *  シリアルI/Oポート管理ブロックの初期化
	 */
    for (p_siopcb = siopcb_table, i = 0; i < TNUM_SIOP; p_siopcb++, i++) {
        p_siopcb->siopinib = &(siopinib_table[i]);
        p_siopcb->openflag = false;
        p_siopcb->sendflag = false;
    }
}

/*
 *  オープンしているポートがあるか？
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
 *  シリアルI/Oポートのオープン
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
 *  シリアルI/Oポートのクローズ
 */
void
uart_pl011_cls_por(SIOPCB *p_siopcb)
{
//    sil_wrw_mem((void *)(p_siopcb->siopinib->cr),
//        (sil_rew_mem((void *)(p_siopcb->siopinib->cr)) & ~UCR_UARTEN)); /* Disable Interrupt */

    p_siopcb->openflag = false;
}

/*
 *  シリアルI/Oポートへの文字送信
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
 *  シリアルI/Oポートからの文字受信
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
 *  シリアルI/Oポートからのコールバックの許可
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
 *  シリアルI/Oポートからのコールバックの禁止
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
 *  シリアルI/Oポートに対する割込み処理
 */
static void
uart_pl011_isr_siop(SIOPCB *p_siopcb)
{
	if (uart_pl011_getready(p_siopcb)) {
		/*
		 *  受信通知コールバックルーチンを呼び出す．
		 */
		uart_pl011_ierdy_rcv(p_siopcb->exinf);
	}
	if (uart_pl011_putready(p_siopcb)) {
		/*
		 *  送信可能コールバックルーチンを呼び出す．
		 */
		uart_pl011_ierdy_snd(p_siopcb->exinf);
	}
}

/*
 *  SIOの割込みサービスルーチン
 */
void
uart_pl011_isr0()
{
    uart_pl011_isr_siop(&(siopcb_table[0]));
}
