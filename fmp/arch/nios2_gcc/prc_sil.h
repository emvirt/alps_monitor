/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
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
 *  @(#) $Id: prc_sil.h 473 2009-11-20 02:03:13Z ertl-honda $
 */

/*
 *  sil.hのプロセッサ依存部（Nios2用）
 */

#ifndef TOPPERS_PRC_SIL_H
#define TOPPERS_PRC_SIL_H

/*
 *  プロセッサのエンディアン
 */
#define SIL_ENDIAN_LITTLE

#ifndef TOPPERS_MACRO_ONLY

/*
 *  NMIを除くすべての割込みの禁止
 */
Inline uint8_t
TOPPERS_disint(void)
{
	uint32_t  TOPPERS_status;

	Asm("rdctl %0, status" : "=r" (TOPPERS_status));
	Asm("wrctl status, %0" :: "r" (TOPPERS_status & ~(0x0001)) : "memory");
    return((uint8_t)(TOPPERS_status & (0x0001)));
}

/*
 *  割込み優先度マスク（内部表現）の現在値の設定
 */
Inline void
TOPPERS_set_pie(uint8_t TOPPERS_pie_bit)
{
	uint32_t  TOPPERS_status;

	Asm("rdctl %0, status" : "=r" (TOPPERS_status));
	TOPPERS_status = TOPPERS_status & ~(0x0001);
	TOPPERS_status = TOPPERS_status | (TOPPERS_pie_bit & (0x0001));
	Asm("wrctl status, %0" :: "r" (TOPPERS_status) : "memory");
}

/*
 *  全割込みロック状態の制御
 */
#define SIL_PRE_LOC      uint8_t TOPPERS_pie_bit
#define SIL_LOC_INT()    ((void)(TOPPERS_pie_bit = TOPPERS_disint()))
#define SIL_UNL_INT()    (TOPPERS_set_pie(TOPPERS_pie_bit))

/*
 *  微少時間待ち（prc_support.S）
 */
extern void sil_dly_nse(ulong_t dlytim);


/*
 *  I/O空間アクセス関数郡
 *  Nios2はI/O空間とメモリ空間は区別しないが，キャッシュを無効にするため，
 *  I/Oアクセス時はこれらの関数を用いることとする．
 */

/*
 *  8ビット単位の読出し／書込み
 */
Inline uint8_t
sil_reb_iop(void *mem)
{
	return(__builtin_ldbuio(mem));
}

Inline void
sil_wrb_iop(void *mem, uint8_t data)
{
	__builtin_stbio(mem, data);
}

/*
 *  16ビット単位の読出し／書込み
 */
Inline uint16_t
sil_reh_iop(void *mem)
{
	return(__builtin_ldhuio(mem));
}

Inline void
sil_wrh_iop(void *mem, uint16_t data)
{
	__builtin_sthio(mem, data);
}

/*
 *  32ビット単位の読出し／書込み
 */
Inline uint32_t
sil_rew_iop(void *mem)
{
	return(__builtin_ldwio(mem));
}

Inline void
sil_wrw_iop(void *mem, uint32_t data)
{
	__builtin_stwio(mem, data);
}

/*
 *  プロセッサIDの取得
 */
Inline void
sil_get_pid(ID *p_prcid)
{
	uint32_t index;

	Asm("rdctl %0, cpuid" : "=r" (index));
	*p_prcid = index + 1;
}

#define TOPPERS_MUTEX_MUTEX_OFFSET 0x00
#define TOPPERS_MUTEX_RESET_OFFSET 0x04

#define TOPPERS_MUTEX_MUTEX_OWNER_MASK   0xffff0000
#define TOPPERS_MUTEX_MUTEX_VALUE_MASK   0x0000ffff
#define TOPPERS_MUTEX_MUTEX_OWNER_OFFSET (16U)

#define TOPPERS_MUTEX_RESET_RESET_MASK   0x01
#define TOPPERS_MUTEX_VALUE_DATA         0x1234

/*
 *  スピンロックの取得（silが使用できないので直接アクセス）
 */
Inline uint8_t
TOPPERS_sil_loc_spn(void)
{
	uint8_t pie_bit;
	uint32_t data;
	uint32_t check;
	ID       id;

  retry:
	/* 全割込みロック状態へ*/
	pie_bit = TOPPERS_disint();

	/* スピンロックの取得 */
	sil_get_pid(&id);

	data = (id << TOPPERS_MUTEX_MUTEX_OWNER_OFFSET) | TOPPERS_MUTEX_VALUE_DATA;
	sil_wrw_iop((void *)(TOPPERS_SIL_MUTEX_BASE + TOPPERS_MUTEX_MUTEX_OFFSET), data);
	check = sil_rew_iop((void *)(TOPPERS_SIL_MUTEX_BASE + TOPPERS_MUTEX_MUTEX_OFFSET));

	if (data != check) {
		/* スピンロックが取得できなかった場合 */
		/* 割込みロック状態を前の状態へ */
		TOPPERS_set_pie(pie_bit);
		goto retry;
	}

	Asm("":::"memory");

	return pie_bit;
}

/*
 *  スピンロックの返却
 */
Inline void
TOPPERS_sil_unl_spn(uint8_t pie_bit)
{
	ID       id;

	sil_get_pid(&id);

	sil_wrw_iop((void *)(TOPPERS_SIL_MUTEX_BASE + TOPPERS_MUTEX_MUTEX_OFFSET),
				(id << TOPPERS_MUTEX_MUTEX_OWNER_OFFSET));

	/* スピンロックの取得前の状態へ */
	TOPPERS_set_pie(pie_bit);
}

/*
 *  スピンロック
 */
#define SIL_LOC_SPN() ((void)(TOPPERS_pie_bit = TOPPERS_sil_loc_spn()))
#define SIL_UNL_SPN() (TOPPERS_sil_unl_spn(TOPPERS_pie_bit))

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_PRC_SIL_H */
