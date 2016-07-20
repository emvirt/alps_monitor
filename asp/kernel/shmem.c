/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2008 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: shmem.c 748 2010-10-27 17:18:06Z dsl $
 */

#include "kernel_impl.h"
#include "check.h"
#include "task.h"
#include "shmem.h"

/*
 *  トレースログマクロのデフォルト定義
 */
#ifndef LOG_GET_SHM_ENTER
#define LOG_GET_SHM_ENTER(shmid, p_shmadd)
#endif /* LOG_GET_SHM_ENTER */

#ifndef LOG_GET_SHM_LEAVE
#define LOG_GET_SHM_LEAVE(ercd)
#endif /* LOG_GET_SHM_LEAVE */

/*
 *  number of shared memory regions
 */
#define tnum_shm	((uint_t)(tmax_shmid - TMIN_SHMID + 1))

/*
 *  shared memory ID to control block macros
 */
#define INDEX_SHM(shmid)	((uint_t)((shmid) - TMIN_SHMID))
#define get_shmcb(shmid)	(&(shmcb_table[INDEX_SHM(shmid)]))

/*
 *  shared memory module initialization
 */
#ifdef TOPPERS_shmini

void
initialize_shmem(void)
{
	uint_t	i;
	SHMCB	*p_shmcb;

	for (p_shmcb = shmcb_table, i = 0; i < tnum_shm; p_shmcb++, i++) {
		p_shmcb->p_shminib = &(shminib_table[i]);
	}
}

#endif /* TOPPERS_shmini */

/*
 *  get a shared memory region
 */
#ifdef TOPPERS_get_shm

ER
get_shm(ID shmid, intptr_t *p_shmadd, uint_t *p_shmsz)
{
	SHMCB	*p_shmcb;
	ER		ercd;

	LOG_GET_SHM_ENTER(shmid, p_shmadd);
	CHECK_DISPATCH();
	CHECK_SHMID(shmid);
	p_shmcb = get_shmcb(shmid);

	*p_shmadd = p_shmcb->p_shminib->shmadd;
	*p_shmsz  = p_shmcb->p_shminib->shmsz;

	ercd = E_OK;

  error_exit:
	LOG_GET_SHM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_get_shm */
