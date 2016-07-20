/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel  
 *
 *  Copyright (C) 2008-2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: nios2.h 89 2009-01-25 16:02:00Z ertl-honda $
 */

/*
 *  Nios2のハードウェア資源の定義
 */

#ifndef TOPPERS_NIOS2_H
#define TOPPERS_NIOS2_H

#include <sil.h>

/*
 *  割込みビット
 */
#define STATUS_U   0x02
#define STATUS_PIE 0x01

/*
 * 割込みの数
 */
#define MAX_INT_NUM 32

/*
 * 例外の個数
 */
#define MAX_EXC_NUM 32

/*
 * 割込みハンドラ番号から，IRC操作のためのビットパターンを求めるマクロ
 */
#define INTNO_BITPAT(intno) (1U << intno)

/*
 * Mutex
 */ 
#define MUTEX_MUTEX_OFFSET 0x00
#define MUTEX_RESET_OFFSET 0x04

#define MUTEX_MUTEX_OWNER_MASK   0xffff0000
#define MUTEX_MUTEX_VALUE_MASK   0x0000ffff
#define MUTEX_MUTEX_OWNER_OFFSET (16U)

#define MUTEX_RESET_RESET_MASK   0x01
#define MUTEX_VALUE_DATA         0x1234

#ifndef TOPPERS_MACRO_ONLY
#include "prc_insn.h"

/*
 *  プロセッサINDEX（0オリジン）の取得
 *  合成時に各プロセッサへ0から連続した値を設定しておく． 
 */
Inline uint32_t
x_prc_index(void)
{
	return(current_cpuid());
}

/*
 *  Mutex関連
 */

/*
 *  Mutexの取得
 *  取得できればtrueを取得できなければfalseを返す 
 */ 
Inline int
mutex_try_lock(uint32_t addr, uint32_t id)
{
	uint32_t data;
	uint32_t check;
    
	data = (id << MUTEX_MUTEX_OWNER_OFFSET) | MUTEX_VALUE_DATA;

	sil_wrw_mem((void *)(addr + MUTEX_MUTEX_OFFSET), data);
	check = sil_rew_mem((void *)(addr + MUTEX_MUTEX_OFFSET));

	return (data == check)? true : false;
}

/*
 *  Mutexの解放 
 */ 
Inline void
mutex_unlock(uint32_t addr, uint32_t id)
{
	sil_wrw_mem((void *)(addr + MUTEX_MUTEX_OFFSET), (id << MUTEX_MUTEX_OWNER_OFFSET));
}

/*
 *  Mutexの初期化
 */
Inline void
mutex_init(uint32_t addr)
{
	uint32_t data;
	uint32_t check;

	check = sil_rew_mem((void *)(addr + MUTEX_RESET_OFFSET));

	if ( check == MUTEX_RESET_RESET_MASK ){
		sil_wrw_mem((void *)(addr + MUTEX_RESET_OFFSET), MUTEX_RESET_RESET_MASK);
	}
	else {
		data = sil_rew_mem((void *)(addr + MUTEX_MUTEX_OFFSET));
		sil_wrw_mem((void *)(addr + MUTEX_MUTEX_OFFSET), (data & MUTEX_MUTEX_OWNER_MASK));
	}
}
#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_MICROBLAZE_H */
