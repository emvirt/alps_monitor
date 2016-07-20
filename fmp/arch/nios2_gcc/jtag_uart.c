/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel  
 *
 *  Copyright (C) 2008 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: jtag_uart.c 238 2009-04-28 08:26:25Z ertl-honda $
 */

/*
 *  シリアルI/Oデバイス（SIO）ドライバ（JTAG UART用）
 */

#include <kernel.h>
#include <t_syslog.h>
#include "target_syssvc.h"
#include "target_serial.h"

/*
 *  シリアルI/Oポート管理ブロックの定義
 */
struct sio_port_control_block 
{
	const SIOPINIB  *p_siopinib;  /* シリアルI/Oポート初期化ブロック */
	intptr_t        exinf;        /* 拡張情報 */
	bool_t    openflag;             /* オープン済みフラグ */
	bool_t    sendflag;             /* 送信割込みイネーブルフラグ */
	bool_t    getready;             /* 文字を受信した状態 */
	bool_t    putready;             /* 文字を送信できる状態 */
	/*
	 *  JTAG_UART はステータスとデータを同時に取得するため，ステータスを
	 *  チェックするとデータを読み込んでしまう．そのため，uart_getready()
	 *  を実行した後のデータを入れる．
	 */ 
	int8_t    jtag_uart_buf;
};

/*
 *  シリアルI/Oポート管理ブロックのエリア
 */
SIOPCB	siopcb_table[TNUM_PORT];

/*
 *  シリアルI/OポートIDから管理ブロックを取り出すためのマクロ
 */
#define INDEX_SIOP(siopid)  ((uint_t)((siopid) - 1))
#define get_siopcb(siopid)  (&(siopcb_table[INDEX_SIOP(siopid)]))

/*
 * 文字を受信したか?
 */ 
Inline bool_t
uart_getready(SIOPCB *p_siopcb)
{
	uint32_t tmp = sil_rew_iop(p_siopcb->p_siopinib->data);
	p_siopcb->jtag_uart_buf = (uint8_t)tmp;
	return ((tmp & JTAG_UART_DATA_RVALID) != 0);
}

/*
 * 文字を送信できるか?
 */
Inline bool_t
uart_putready(SIOPCB *p_siopcb)
{
		return((sil_rew_iop(p_siopcb->p_siopinib->ctrl)
				& JTAG_UART_CONTROL_WSAPCE) > 0);
}

/*
 *  受信した文字の取り出し
 */
Inline uint8_t
uart_getchar(SIOPCB *p_siopcb)
{
	return((uint8_t)(sil_rew_iop(p_siopcb->p_siopinib->data)));
}

/*
 *  送信する文字の書き込み
 */
Inline void
uart_putchar(SIOPCB *p_siopcb, uint8_t c)
{
	sil_wrw_iop(p_siopcb->p_siopinib->data, c);
}

/*
 * 送信制御関数
 */
/*
 *  送信割込み許可
 */
Inline void
uart_enable_send(SIOPCB *p_siopcb)
{
	sil_wrw_iop(p_siopcb->p_siopinib->ctrl,
				sil_rew_iop(p_siopcb->p_siopinib->ctrl)
				|JTAG_UART_CONTROL_WIE);
}

/*
 *  送信割込み禁止
 */
Inline void
uart_disable_send(SIOPCB *p_siopcb)
{
	sil_wrw_iop(p_siopcb->p_siopinib->ctrl,
				sil_rew_iop(p_siopcb->p_siopinib->ctrl)
				& ~JTAG_UART_CONTROL_WIE);
}


/*
 *  受信割込み許可
 */
Inline void
uart_enable_rcv(SIOPCB *p_siopcb)
{
	sil_wrw_iop(p_siopcb->p_siopinib->ctrl,
				sil_rew_iop(p_siopcb->p_siopinib->ctrl)
				| JTAG_UART_CONTROL_RIE);
}

/*
 *  受信割込み禁止
 */
Inline void
uart_disable_rcv(SIOPCB *p_siopcb)
{
	sil_wrw_iop(p_siopcb->p_siopinib->ctrl,
				sil_rew_iop(p_siopcb->p_siopinib->ctrl)
				& ~JTAG_UART_CONTROL_RIE);
}


/*
 *  SIOドライバの初期化
 */
void
sio_initialize(intptr_t exinf)
{
	SIOPCB  *p_siopcb;
	uint_t  i;

	/*
	 *  シリアルI/Oポート管理ブロックの初期化
	 */
	for (p_siopcb = siopcb_table, i = 0; i < TNUM_PORT; p_siopcb++, i++) {
		p_siopcb->p_siopinib = &(siopinib_table[i]);
		p_siopcb->openflag = false;
		p_siopcb->sendflag = false;
		p_siopcb->jtag_uart_buf = -1;
	}
}

/*
 * シリアルI/Oポートのオープン
 */
SIOPCB *
jtag_uartl_opn_por(SIOPCB *p_siopcb, intptr_t exinf)
{
	const SIOPINIB  *p_siopinib;
	p_siopinib = p_siopcb->p_siopinib;

	p_siopcb->exinf = exinf;
	p_siopcb->getready = p_siopcb->putready = false;
	p_siopcb->openflag = true;
	

	return(p_siopcb);
}

/*
 *  シリアルI/Oポートのオープン
 */
SIOPCB *
sio_opn_por(ID siopid, intptr_t exinf)
{
	SIOPCB  *p_siopcb = get_siopcb(siopid);
	bool_t    opnflg;
	ER      ercd;

	/*
	 *  オープンしたポートがあるかをopnflgに読んでおく．
	 */
	opnflg = p_siopcb->openflag;

	/*
	 *  デバイス依存のオープン処理．
	 */
	jtag_uartl_opn_por(p_siopcb, exinf);

	/*
	 *  シリアルI/O割込みのマスクを解除する．
	 */
	if (!opnflg) {
		ercd = ena_int(p_siopcb->p_siopinib->intno);
		assert(ercd == E_OK);
	}
	return(p_siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
sio_cls_por(SIOPCB *p_siopcb)
{
	const SIOPINIB  *p_siopinib;

	p_siopinib = p_siopcb->p_siopinib;
	
	/* 割込みの禁止 */
	sil_wrw_iop(p_siopcb->p_siopinib->ctrl, 0x00);

	p_siopcb->openflag = false;

	/*
	 *  シリアルI/O割込みをマスクする．
	 */
	dis_int(p_siopcb->p_siopinib->intno);
}

/*
 *  SIOの割込みハンドラ
 */
void
sio_isr(intptr_t siopid)
{
	SIOPCB *p_siopcb = get_siopcb(siopid);

	if (uart_getready(p_siopcb)) {
		/*
		 *  受信通知コールバックルーチンを呼び出す．
		 */
		sio_irdy_rcv(p_siopcb->exinf);
	}
	if (uart_putready(p_siopcb)) {
		/*
		 *  送信可能コールバックルーチンを呼び出す．
		 */
		sio_irdy_snd(p_siopcb->exinf);
	}
}

/*
 *  シリアルI/Oポートへの文字送信
 */
bool_t
sio_snd_chr(SIOPCB *p_siopcb, char_t c)
{
	if (uart_putready(p_siopcb)) {
		uart_putchar(p_siopcb, c);
		return(true);
	}
	return(false);
}

/*
 *  シリアルI/Oポートからの文字受信
 */
int_t
sio_rcv_chr(SIOPCB *p_siopcb)
{
	uint8_t tmp;

	if (p_siopcb->jtag_uart_buf != -1) {
		tmp = p_siopcb->jtag_uart_buf;
		p_siopcb->jtag_uart_buf = -1;
		return tmp;
	}
	return(false);
}

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
void
sio_ena_cbr(SIOPCB *p_siopcb, uint_t cbrtn)
{
	switch (cbrtn) {
	  case SIO_RDY_SND:
		uart_enable_send(p_siopcb);
		break;
	  case SIO_RDY_RCV:
		uart_enable_rcv(p_siopcb);
		break;
	}
}

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
void
sio_dis_cbr(SIOPCB *p_siopcb, uint_t cbrtn)
{
	switch (cbrtn) {
	  case SIO_RDY_SND:
		uart_disable_send(p_siopcb);
		break;
	  case SIO_RDY_RCV:
		uart_disable_rcv(p_siopcb);
		break;
	}
}
