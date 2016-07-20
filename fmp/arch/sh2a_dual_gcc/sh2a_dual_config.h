/*
 *	TOPPERS/FMP Kernel
 *		Toyohashi Open Platform for Embedded Real-Time Systems/
 *		Advanced Standard Profile Kernel
 *	
 *	Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *				Graduate School of Information Science, Nagoya Univ., JAPAN
 *	Copyright (C) 2007-2010 by Industrial Technology Institute,
 *								Miyagi Prefectural Government, JAPAN
 *	
 *	上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *	ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *	変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *	(1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *		権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *		スコード中に含まれていること．
 *	(2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *		用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *		者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *		の無保証規定を掲載すること．
 *	(3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *		用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *		と．
 *	  (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *		  作権表示，この利用条件および下記の無保証規定を掲載すること．
 *	  (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *		  報告すること．
 *	(4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *		害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *		また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *		由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *		免責すること．
 *	
 *	本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *	よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *	に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *	アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *	の責任を負わない．
 * 
 *  @(#) $Id$
 */

/*
 *		プロセッサ依存モジュール（SH2A-DUAL用）
 *
 *	このインクルードファイルは，sh7xxx_config.h（または，そこからインク
 *	ルードされるファイル）のみからインクルードされる．他のファイルから
 *	直接インクルードしてはならない．
 */

#ifndef TOPPERS_SH2A_DUAL_CONFIG_H
#define TOPPERS_SH2A_DUAL_CONFIG_H

/*
 *	コアの種類
 */
#define SH2A_DUAL

/*
 *	使用できる命令セット
 *	　SH1用命令については、すべてのコアでサポートされるので、
 *	　定義しない。
 */
#define TOPPERS_SUPPORT_SH2_INSTRUCTION
#define TOPPERS_SUPPORT_SH2E_INSTRUCTION
#define TOPPERS_SUPPORT_SH2A_INSTRUCTION

/*
 *	ローカルな割込み番号／割込みハンドラ番号／CPU例外番号を取り出すためのマスク
 */
#define LOCAL_INTNO_MASK	0xffffU
#define LOCAL_INHNO_MASK	0xffffU
#define LOCAL_EXCNO_MASK	0xffffU

/*
 *	グローバルな割込み番号／CPU例外番号を
 *	ローカルな割込み番号／CPU例外番号に変換するマクロ
 */
#define LOCAL_INTNO(intno)		((intno) & LOCAL_INTNO_MASK)
#define LOCAL_EXCNO(excno)		((excno) & LOCAL_EXCNO_MASK)


/*
 *	割込み番号から割込み種別を判別するためのマクロ
 *	　prc_config.hで定義すべきだが、このファイル中のi_begin_int()
 *	　で使用するため、ここに記述している。
 */
/*	IRQ割込み  */
#define LOCAL_INTNO_IS_IRQ(intno) 								\
		((TMIN_INTNO_IRQ <= (intno)) && ((intno) <= TMAX_INTNO_IRQ))
#define INTNO_IS_IRQ(intno)		LOCAL_INTNO_IS_IRQ(LOCAL_INTNO(intno))

/*	内蔵周辺モジュール	*/
#define LOCAL_INTNO_IS_PERIPHERAL(intno) 								\
		((TMIN_INTNO_PERIPHERAL <= (intno)) && ((intno) <= TMAX_INTNO_PERIPHERAL))
#define INTNO_IS_PERIPHERAL(intno)	LOCAL_INTNO_IS_PERIPHERAL(LOCAL_INTNO(intno))

/*	PINT割込み	*/
#define LOCAL_INTNO_IS_PINT(intno) 								\
		((TMIN_INTNO_PINT <= (intno)) && ((intno) <= TMAX_INTNO_PINT))
#define INTNO_IS_PINT(intno)	LOCAL_INTNO_IS_PINT(LOCAL_INTNO(intno))

/*	プロセッサ間割込み	*/
#define LOCAL_INTNO_IS_IPI(intno) 								\
		((TMIN_INTNO_IPI <= (intno)) && ((intno) <= TMAX_INTNO_IPI))
#define INTNO_IS_IPI(intno)		LOCAL_INTNO_IS_IPI(LOCAL_INTNO(intno))

/*
 *	割込み番号からIRQ番号への変換
 */
#define LOCAL_INTNO_TO_IRQNO(intno) (((intno) - TMIN_INTNO_IRQ))
#define INTNO_TO_IRQNO(intno) 	LOCAL_INTNO_TO_IRQNO(LOCAL_INTNO(intno))

/*
 *	割込み番号からPINT割込み番号への変換
 */
#define LOCAL_INTNO_TO_PINTNO(intno) (((intno) - TMIN_INTNO_PINT))
#define INTNO_TO_PINTNO(intno) 	LOCAL_INTNO_TO_PINTNO(LOCAL_INTNO(intno))

/*
 *	割込み番号からIDCNTレジスタアクセス用インデックスへの変換
 */
#define LOCAL_INTNO_TO_IDCNT(intno) (((intno) - TMIN_INTNO_IDCNT))
#define INTNO_TO_IDCNT(intno) 	LOCAL_INTNO_TO_IDCNT(LOCAL_INTNO(intno))

/*
 *	割込み番号からプロセッサ間割込みの割込み優先レベルへの変換
 */
#define LOCAL_INTNO_TO_IP_INT_LEVEL(intno) \
		(15U - ((intno) - IPI15_VECTOR))
#define INTNO_TO_IP_INT_LEVEL(intno) 	LOCAL_INTNO_TO_IP_INT_LEVEL(LOCAL_INTNO(intno))

/*
 *	コア０とコア１のIPRレジスタのアドレスの差
 */
#define PRC_IPR_OFFSET	(C1IPR01 - C0IPR01)


#ifndef TOPPERS_MACRO_ONLY

#include <sil.h>

/*
 *	割込みコントローラのレジスタ定義
 *	　
 * 　ペリファラルの数によって、割込みコントローラのレジスタ構成が
 * 　異なるため、チップ依存部で定義する。
 */
extern const IRC_REG irc_reg_tbl[TNUM_PRCID];

/*
 *	割込み検出制御レジスタの定義
 */
extern uint16_t *const idcnt_tbl[TNUM_IDCNT];

/*
 *	セマフォ制御レジスタの定義
 */
extern uint8_t *const semr_tbl[TNUM_SEMR];

/*
 *  str_ker() の前でマスタプロセッサで行う初期化
 */
extern void prc_mprc_initialize(void);


/*
 * IRCの初期化
 */
Inline void
irc_initialize(uint_t prcindex)
{
   	uint_t i;
   	const IRC_REG * p_irc_reg = &irc_reg_tbl[prcindex];
	
	/*	要検討：具体的な設定値	*/
	/*	要検討：両プロセッサで同じ値にする	*/
	sil_wrh_reg(p_irc_reg->icr0, 0x0000U);
	sil_wrh_reg(p_irc_reg->icr1, 0x0000U);
	sil_wrh_reg(p_irc_reg->icr2, 0x0000U);
	
#ifdef USE_BANKED_REG
	/*
	 * ・NMI,UBC以外のすべての割込みでレジスタバンクの使用を許可
	 * ・レジスタバンク・オーバーフロー例外を許可
	 */
	sil_wrh_reg(p_irc_reg->ibnr, (IBNR_BE0 | IBNR_BOVE));
#else
	/* レジスタバンク使用禁止 */
	sil_wrh_reg(p_irc_reg->ibnr, 0x0000U);
#endif /* USE_BANKED_REG */

   	/*	割込み優先レベル設定レジスタの初期化  */
	for (i=0; i < TNUM_IPR; ++i) {
		sil_wrh_reg(p_irc_reg->ipr[i], 0x0000U);
	}
}


/*
 *	IRQ割込みの属性設定
 *	 INTNO local_intno：プロセッサ内でのローカルな割込み番号
 *	 ATR intatr：割込み属性
 *	 uint_t prcindex：プロセッサ・インデックス（0オリジン）
 */
Inline void
x_config_int_irq(INTNO local_intno, ATR intatr, uint_t prcindex)
{
	uint16_t *p_irqer, *p_irqrr;
	uint32_t icr1_val	= sil_reh_reg((uint16_t *)C0ICR1);
	uint32_t irqno		= LOCAL_INTNO_TO_IRQNO(local_intno);
	uint32_t offset		= irqno << 1U;	/*	irqno * 2  */

	/*
	 *	エッジトリガの指定がなければ、デフォルトでローレベルトリガ
	 *	とする。
	 *	該当ビットをクリアしたままであれば、ローレベルトリガとなる。
	 */
	icr1_val &=  ~(0x03U << offset);
	
	if((intatr & TA_POSEDGE) != 0U) {
		/*
		 *	ポジティブエッジ
		 */
		icr1_val |= (IRQ_POSEDGE << offset);
	}

	/*
	 *	ネガティブエッジの指定がある、
	 *	または、エッジトリガで、極性の指定がなければ、
	 *	ネガティブエッジとする。
	 */
	if(((intatr & TA_NEGEDGE) != 0U) ||
	   (((intatr & TA_EDGE) != 0U) && ((intatr & TA_BOTHEDGE) == 0U))) {
		/*
		 *	ネガティブエッジ
		 */
		icr1_val |= (IRQ_NEGEDGE << offset);
	}
	/*
	 *	C0ICR1とC0ICR1に同じ内容を設定しなければならない。
	 *	　要検討：プロセッサ間で排他制御する必要がある。
	 */
	sil_wrh_reg((uint16_t *)C0ICR1, icr1_val);
	sil_wrh_reg((uint16_t *)C1ICR1, icr1_val);
	
	/*
	 *	CnIRQERレジスタのIRQnビットをセットする。
	 */
	p_irqer = irc_reg_tbl[prcindex].irqer;
	sil_seth_bit(p_irqer, irqno);
	
	/*
	 *	割込み要求のクリア
	 *	　CnIRQRRレジスタのIRQnFビットをクリアする。
	 */
	p_irqrr = irc_reg_tbl[prcindex].irqrr;
	sil_clrh_bit(p_irqrr, irqno);
}

/*
 *	内蔵周辺モジュールからの割込みの属性設定
 *	 INTNO local_intno：プロセッサ内でのローカルな割込み番号
 *	 ATR intatr：割込み属性
 *	 uint_t prcindex：プロセッサ・インデックス（0オリジン）
 */
Inline void
x_config_int_peripheral(INTNO local_intno, ATR intatr, uint_t prcindex)
{
	int_t index 		= LOCAL_INTNO_TO_IDCNT(local_intno);
	uint16_t *p_idcnt 	= idcnt_tbl[index];

	/*
	 *	割込み要求を受け付けるCPUを設定する。
	 */
	if (prcindex == 0) {
		sil_anh_reg(p_idcnt, ~IDCNT_CPUN);
	}
	else {
		sil_orh_reg(p_idcnt, IDCNT_CPUN);
	}
}

/*
 *	PINT割込みの属性設定
 *	 INTNO local_intno：プロセッサ内でのローカルな割込み番号
 *	 ATR intatr：割込み属性
 *	 uint_t prcindex：プロセッサ・インデックス（0オリジン）
 */
Inline void
x_config_int_pint(INTNO local_intno, ATR intatr, uint_t prcindex)
{
	uint32_t icr2_val	= sil_reh_reg((uint16_t *)C0ICR2);
	uint_t pintno 		= LOCAL_INTNO_TO_PINTNO(local_intno);
	uint32_t bitptn		= 1U << pintno;

	/*
	 *	レベルトリガの極性を設定
	 *	
	 *	デフォルトでローレベルトリガ、
	 *	明示的な指定があれば、ハイレベルトリガとする。
	 */
	if((intatr & TA_HIGHLEVEL) != 0U) {
		/*
		 *	ハイレベルトリガ
		 */
		icr2_val |= bitptn;
	}
	else {
		/*
		 *	ローレベルトリガ
		 */
		icr2_val &= ~bitptn;
	}

	/*
	 *	C0ICR2とC0ICR2に同じ内容を設定しなければならない。
	 *	　要検討：プロセッサ間で排他制御する必要がある。
	 */
	sil_wrh_reg((uint16_t *)C0ICR2, icr2_val);
	sil_wrh_reg((uint16_t *)C1ICR2, icr2_val);
}

/*
 *	割込みハンドラ入口で必要なIRC操作
 *
 *	IRQ割込みでかつエッジトリガの場合は割込み要求をクリアする．
 *	エッジトリガかレベルトリガかを判別するとオーバーヘッドが大きいので，
 *	常にクリアする．
 *	（レベルトリガでクリア処理をしても害はない。）
 *
 *	IRQRRレジスタを一度、ダミーリードして、 該当ビットに0を書き込む。
 *	この間、全割込みロック状態にしているのは、以下のようなケースを防ぐ
 *	ため。
 *
 *	3種類の割込みを考える。
 *	割込み優先度は、l > m > nの順とする。
 *	（割込みlの優先度が高い。）
 *	
 *	1.	mの割込みが発生(mビットがセット)
 *	2.	mの割込みハンドラが実行(nは割込み禁止)
 *	3.	IRQRRリード(l = 0, m = 1, n = 0)
 *	4.	lの割込みが発生(lビットがセット)
 *	5.	lの割込みハンドラが実行(nは割込み禁止)
 *	6.	nの割込みが発生（保留）(nビットがセット)
 *	7.	IRQRRリード(l = 1, m = 1, n = 1)
 *	8.	IRQRRライト(l = 0, m = 1, n = 1)
 *	9.	lの割込みハンドラ終了
 *	10. mの割込みハンドラ再開
 *	11. IRQQライト(l = 0, m = 0, n = 0)
 *	
 *	11の段階で n がクリアされる可能性がある．
 */
Inline void
i_begin_int(INTNO intno)
{
	SIL_PRE_LOC;
	uint_t index;
	uint32_t irqpr, bitptn;
	uint16_t *irqrr_addr;
	INTNO local_intno = LOCAL_INTNO(intno);

	if (LOCAL_INTNO_IS_IRQ(local_intno)) {
		index = x_prc_index();
		irqrr_addr = irc_reg_tbl[index].irqrr;
		/*
		 * 一度、ダミーリードして、 該当ビットに0を書き込む。
		 *	　変数bitptnを用いているのは、全割込みロックの区間を
		 *	　短くするため
		 */
		bitptn = ~(0x01U << LOCAL_INTNO_TO_IRQNO(local_intno));
		SIL_LOC_INT();
		irqpr = sil_reh_reg(irqrr_addr);
		irqpr &= bitptn;
		sil_wrh_reg(irqrr_addr, irqpr);
		SIL_UNL_INT();
	}
}

#endif /* TOPPERS_MACRO_ONLY */

/*
 *	プロセッサ依存モジュール（SH2A-DUAL用）
 */
#include "prc_config.h"

#endif /* TOPPERS_SH2A_DUAL_CONFIG_H */
