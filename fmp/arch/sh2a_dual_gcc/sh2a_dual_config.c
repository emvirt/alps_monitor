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
 */

#include "kernel_impl.h"
#include "check.h"
#include "task.h"


/*
 *	割込みコントローラのレジスタ定義
 */
const IRC_REG irc_reg_tbl[TNUM_PRCID] = {
	IRC_REG_DATA
};

/*
 *	割込み検出制御レジスタの定義
 */
uint16_t *const idcnt_tbl[TNUM_IDCNT] = {
	IDCNT_REG_DATA
};

/*
 *	セマフォ制御レジスタの定義
 */
uint8_t *const semr_tbl[TNUM_SEMR] = {
	SEMR_REG_DATA
};


/*
 *  str_ker() の実行前にマスタプロセッサのみ実行される初期化処理
 */
void
prc_mprc_initialize(void)
{
	int_t i;
	uint16_t *p_idcnt;
	
	/*
	 *	セマフォ制御レジスタの初期化
	 *	　SEMRnのSEMFビット←1で、リソースの解放を表す。
	 */
	for(i = 0; i < TNUM_SEMR; ++i) {
		sil_wrb_mem(semr_tbl[i], SEMR_SEMF_BIT);
	}

	/*
	 * 割込み検出制御レジスタの設定
	 * 　ペリファラルの数によって、割込み検出制御レジスタの構成が
	 * 　異なるため、チップ依存部で定義する。
	 */
	for (i = 0; i < TNUM_IDCNT; ++i) {
		p_idcnt = idcnt_tbl[i];
		if (p_idcnt != NULL) {
			/*
			 *	IDCNTnレジスタのINTENビットをクリア
			 *    INTENビットが下位8ビットに収まっていないため、
			 *    sil_clrh_bitを用いても、最適化されない。
			 *    （~IDCNT_INTENをコンパイル時に求めた方が得策）
			 */
			sil_anh_reg(p_idcnt, ~IDCNT_INTEN);
		}
	}
}

/*
 *	割込み要求ライン属性の設定
 */
void
x_config_int(INTNO intno, ATR intatr, PRI intpri, uint_t affinity_mask)
{
	SIL_PRE_LOC;
	INTNO iintno, local_intno;
	uint_t prcindex;
	uint32_t ipr_val;
#ifndef NDEBUG
	bool_t ret;
	uint_t prcid_in_intno = intno >> 16U;
	uint_t bitptn = 1U << (prcid_in_intno - 1U);

	assert((bitptn & affinity_mask) != 0);
	assert(VALID_INTNO_CFGINT(intno));
	assert((-15 <= intpri) && (intpri <= TMAX_INTPRI));
#endif /* NDEBUG */

	local_intno	= LOCAL_INTNO(intno);
	prcindex = x_prc_index();

	SIL_LOC_INT();

	/*
	 *	一旦、個別に割込みを禁止する（割込み禁止フラグをセット）
	 */
#ifdef NDEBUG
	(void)x_disable_int(intno);
#else /* NDEBUG */
	ret = x_disable_int(intno);
	assert(ret == true);
#endif /* NDEBUG */

	/*
	 *	割込み要因毎に割込み属性を設定する。
	 *	　プロセッサ間割込みのみ割込み優先レベルが固定なので、
	 *	　先に振り分ける。
	 */
	if (LOCAL_INTNO_IS_IPI(local_intno)) {
		/*
		 *	プロセッサ間割込みの場合
		 *　　　何もしない。
		 */
	}
	else {
		iintno = INT_INTNO(local_intno);			/*	要検討：NMIは該当しない  */
		ipr_val = int_iipm_tbl[prcindex][iintno];	/*	要検討：NMIは該当しない  */
		/*
		 *	割込み優先レベルの設定
		 *　　　割込み番号に対応したIPRの値を設定する．
		 *　　　IRQ割込みの場合、割込み禁止フラグがクリアされるが、
		 *　　　(x_enable_int_irq( )相当）全割込みロック状態なので、
		 *　　　害はない。(要検討：これは正しくない。）
		 */
		irc_set_ipr(local_intno, ipr_val, prcindex);
		
		/*
		 *	残りの割込み要因毎に処理を振り分ける。
		 */
		if (LOCAL_INTNO_IS_IRQ(local_intno)) {
			/*	IRQの場合  */
			x_config_int_irq(local_intno, intatr, prcindex);
		}
		else if (LOCAL_INTNO_IS_PERIPHERAL(local_intno)) {
			/*	内蔵周辺モジュールの場合  */
			x_config_int_peripheral(local_intno, intatr, prcindex);
		}
		else if (LOCAL_INTNO_IS_PINT(local_intno)) {
			/*	PINT割込みの場合  */
			x_config_int_pint(local_intno, intatr, prcindex);
		}
		else {
			/*	その他：何もしない	*/
			assert(0);
		}
	}

	/*
	 *	割込み禁止フラグをクリア(必要な場合)
	 */
	if ((intatr & TA_ENAINT) != 0U) {
#ifdef NDEBUG
		(void) x_enable_int(intno);
#else /* NDEBUG */
		ret = x_enable_int(intno);
		assert(ret == true);
#endif /* NDEBUG */
	}

	SIL_UNL_INT();
}

