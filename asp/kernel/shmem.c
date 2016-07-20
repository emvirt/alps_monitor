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
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ�
 *      ���դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 * 
 *  @(#) $Id: shmem.c 748 2010-10-27 17:18:06Z dsl $
 */

#include "kernel_impl.h"
#include "check.h"
#include "task.h"
#include "shmem.h"

/*
 *  �ȥ졼�����ޥ���Υǥե�������
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
