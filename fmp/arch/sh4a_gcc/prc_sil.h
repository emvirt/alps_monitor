/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2010 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: prc_sil.h 512 2010-02-01 05:33:27Z ertl-honda $
 */

/*
 *  sil.hのプロセッサ依存部（SH4A用）
 */
#ifndef TOPPERS_PRC_SIL_H
#define TOPPERS_PRC_SIL_H

#ifndef TOPPERS_MACRO_ONLY

/*
 *  NMIを除くすべての割込みの禁止
 */
Inline uint8_t
TOPPERS_disint(void)
{
	uint32_t  TOPPERS_sr;

	Asm("stc  sr,%0" : "=r"(TOPPERS_sr));
	Asm("ldc %0, sr" : : "r"(TOPPERS_sr | 0x000000f0U) : "memory");
	return((uint8_t)(TOPPERS_sr & 0x000000f0U));
}

/*
 *  割込み優先度マスク（内部表現）の現在値の設定
 */
Inline void
TOPPERS_set_iipm(uint8_t TOPPERS_iipm)
{
	uint32_t  TOPPERS_sr;

	Asm("stc  sr,%0" : "=r"(TOPPERS_sr));
	Asm("ldc %0, sr" : : "r"((TOPPERS_sr & ~0x000000f0U) | TOPPERS_iipm) : "memory");
}

/*
 *  割込みロック状態の制御
 */
#define SIL_PRE_LOC      uint8_t TOPPERS_iipm
#define SIL_LOC_INT()    ((void)(TOPPERS_iipm = TOPPERS_disint()))
#define SIL_UNL_INT()    (TOPPERS_set_iipm(TOPPERS_iipm))

/*
 *  プロセッサIDの取得
 */
Inline void
sil_get_pid(ID *p_prcid)
{
	*p_prcid = *((volatile uint_t *) 0xFF000048) + 1;
}

/*
 *  スピンロック変数（prc_config.c）
 */
extern uint32_t TOPPERS_spn_var;

/*
 *  ロック取得時に使用するダミー変数
 */
extern  uint32_t dangling_dummy;

/*
 *  スピンロックの取得（通常版）
 */
Inline uint32_t
TOPPERS_sil_loc_spn(void)
{
	uint8_t   iipm;
	uint32_t  result;

retry:
	/* 全割込みロック状態へ*/
	iipm = TOPPERS_disint();

	Asm("     movli.l	@%2,r0             \n"
		"\t   mov       #0,r3              \n"
		"\t   cmp/eq	r3,r0              \n"
		"\t   bf		1f                 \n"
		"\t   mov		#1,r0              \n"
		"\t   movco.l	r0,@%2             \n"
		"\t   bf		2f           	   \n"  //失敗
		"\t   mov		#0,%1          	   \n"
		"\t   bra       3f                 \n"
		"\t    nop                         \n"
		"1:\t                              \n"  //ダングリング　
		"\t   movco.l	r0,%0              \n"
		"2:\t                              \n"  //失敗のパターン
		"\t   mov       #1,%1              \n"
		"3:\t                              \n"  //終了
		:"=m"(dangling_dummy),"=r"(result)
		:"r"(&TOPPERS_spn_var):"r0","r3", "cc");

	if (result != 0) {
		/* スピンロックが取得できなかった場合 */
		/* 割込みロック状態を前の状態へ */
		TOPPERS_set_iipm(iipm);
		goto retry;
	}

	return iipm;
}

/*
 *  スピンロックの解放（通常版）
 *　  中身はrelease_lockと同じ
 */
Inline void
TOPPERS_sil_unl_spn(uint8_t iipm)
{
	TOPPERS_spn_var = 0;
	/* スピンロック前の状態に */
	TOPPERS_set_iipm(iipm);
}

/*
 *  スピンロック
 */
#define SIL_LOC_SPN() ((void)(TOPPERS_iipm = TOPPERS_sil_loc_spn()))
#define SIL_UNL_SPN() (TOPPERS_sil_unl_spn(TOPPERS_iipm))

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_PRC_SIL_H */
