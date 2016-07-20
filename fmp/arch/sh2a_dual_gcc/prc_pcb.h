/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2009 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN  
 *	Copyright (C) 2009-2010 by Industrial Technology Institute,
 *								Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id$
 */

/*
 *  プロセッサ依存プロセッサコントロールブロックモジュール（SH2A-DUAL用）
 */
#ifndef TOPPERS_PRC_PCB_H
#define TOPPERS_PRC_PCB_H

/*
 *  カーネルロックの方式
 */
#define TTYPE_KLOCK 	G_KLOCK

#ifndef TOPPERS_MACRO_ONLY

#include "prc_insn.h"	/*  current_gbr()  */
#include <sil.h>

/*
 *  ロックの型
 */
typedef uint8_t *LOCK;		/*  SEMR1へのポインタ  */

/*
 *  前方参照
 */
typedef struct processor_control_block PCB;

/*
 *  SH2A-DUAL依存プロセッサコントロールブロック
 */
typedef struct target_processor_control_block{
	/*
	 *  コンテキストの管理
	 */
	uint32_t excnest_count;	/*  割込み・CPU例外のネスト回数  */
	STK_T* istkpt;			/*  非タスクコンテキスト用のスタックの初期値  */
	
	/*
	 *  CPUロック機能
	 */
	bool_t lock_flag;		/*  CPUロックフラグ  */
	uint32_t saved_iipm;	/*  CPUロック中にIPMの値を保持する変数（内部表現）*/

	/*
	 *  ベクタテーブルへのポインタ（要検討：必要ない？）
	 */
	const FP* p_vector_table;
}TPCB;

/*  非依存部のget_my_p_pcb()を使用しない  */
#define OMIT_GET_MY_P_PCB

/*
 *  my_p_pcbの取得
 *  　gccでは、gbrを使用しないので、my_p_pcbをgbrに割り当て、
 *  　CPUIDRやmy_p_pcb_tableへの無駄なアクセスを減らす。
 *  　（要検討）
 */
Inline PCB*
get_my_p_pcb(void)
{
	PCB* my_p_pcb;
	
	my_p_pcb = current_gbr();
	return my_p_pcb;
}

/*
 *	プロセッサINDEX（0オリジン）の取得
 *	　戻り値
 *	　　CPU0：0
 *	　　CPU1：1
 */
Inline uint_t
x_prc_index(void)
{
	uint32_t cpuidr, index;
	
	cpuidr = sil_rew_reg((uint32_t *)CPUIDR);
	if ((cpuidr & CPUIDR_ID_BIT) == 0) {
		index = 0U;
	} else {
		index = 1U;
	}
	return index;
}


#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_PRC_PCB_H */
