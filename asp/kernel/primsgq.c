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
 *  @(#) $Id: primsgq.c 748 2010-10-26 17:18:06Z dsl $
 */

/*
 *		優先度データキュー機能
 */

#include "kernel_impl.h"
#include "check.h"
#include "task.h"
#include "wait.h"
#include "primsgq.h"

/*
 *  トレースログマクロのデフォルト定義
 */
#ifndef LOG_SND_PMQ_ENTER
#define LOG_SND_PMQ_ENTER(pmqid, data, msgsz, datapri)
#endif /* LOG_SND_PMQ_ENTER */

#ifndef LOG_SND_PMQ_LEAVE
#define LOG_SND_PMQ_LEAVE(ercd)
#endif /* LOG_SND_PMQ_LEAVE */

#ifndef LOG_PSND_PMQ_ENTER
#define LOG_PSND_PMQ_ENTER(pmqid, data, msgsz, datapri)
#endif /* LOG_PSND_PMQ_ENTER */

#ifndef LOG_PSND_PMQ_LEAVE
#define LOG_PSND_PMQ_LEAVE(ercd)
#endif /* LOG_PSND_PMQ_LEAVE */

#ifndef LOG_IPSND_PMQ_ENTER
#define LOG_IPSND_PMQ_ENTER(pmqid, data, msgsz, datapri)
#endif /* LOG_IPSND_PMQ_ENTER */

#ifndef LOG_IPSND_PMQ_LEAVE
#define LOG_IPSND_PMQ_LEAVE(ercd)
#endif /* LOG_IPSND_PMQ_LEAVE */

#ifndef LOG_TSND_PMQ_ENTER
#define LOG_TSND_PMQ_ENTER(pmqid, data, msgsz, datapri, tmout)
#endif /* LOG_TSND_PMQ_ENTER */

#ifndef LOG_TSND_PMQ_LEAVE
#define LOG_TSND_PMQ_LEAVE(ercd)
#endif /* LOG_TSND_PMQ_LEAVE */

#ifndef LOG_RCV_PMQ_ENTER
#define LOG_RCV_PMQ_ENTER(pmqid, buffer, p_msgsz, p_datapri)
#endif /* LOG_RCV_PMQ_ENTER */

#ifndef LOG_RCV_PMQ_LEAVE
#define LOG_RCV_PMQ_LEAVE(ercd, buffer, msgsz, datapri)
#endif /* LOG_RCV_PMQ_LEAVE */

#ifndef LOG_PRCV_PMQ_ENTER
#define LOG_PRCV_PMQ_ENTER(pmqid, buffer, p_msgsz, p_datapri)
#endif /* LOG_PRCV_PMQ_ENTER */

#ifndef LOG_PRCV_PMQ_LEAVE
#define LOG_PRCV_PMQ_LEAVE(ercd, buffer, msgsz, datapri)
#endif /* LOG_PRCV_PMQ_LEAVE */

#ifndef LOG_TRCV_PMQ_ENTER
#define LOG_TRCV_PMQ_ENTER(pmqid, buffer, p_msgsz, p_datapri, tmout)
#endif /* LOG_TRCV_PMQ_ENTER */

#ifndef LOG_TRCV_PMQ_LEAVE
#define LOG_TRCV_PMQ_LEAVE(ercd, buffer, msgsz, datapri)
#endif /* LOG_TRCV_PMQ_LEAVE */

#ifndef LOG_INI_PMQ_ENTER
#define LOG_INI_PMQ_ENTER(pmqid)
#endif /* LOG_INI_PMQ_ENTER */

#ifndef LOG_INI_PMQ_LEAVE
#define LOG_INI_PMQ_LEAVE(ercd)
#endif /* LOG_INI_PMQ_LEAVE */

#ifndef LOG_REF_PMQ_ENTER
#define LOG_REF_PMQ_ENTER(pmqid, pk_rpmq)
#endif /* LOG_REF_PMQ_ENTER */

#ifndef LOG_REF_PMQ_LEAVE
#define LOG_REF_PMQ_LEAVE(ercd, pk_rpmq)
#endif /* LOG_REF_PMQ_LEAVE */

/*
 *  優先度データキューの数
 */
#define tnum_pmq	((uint_t)(tmax_pmqid - TMIN_PMQID + 1))

/*
 *  優先度データキューIDから優先度データキュー管理ブロックを取り出すた
 *  めのマクロ
 */
#define INDEX_PMQ(pmqid)	((uint_t)((pmqid) - TMIN_PMQID))
#define get_pmqcb(pmqid)	(&(pmqcb_table[INDEX_PMQ(pmqid)]))

static inline void pmq_memcpy(intptr_t dest, intptr_t src, uint_t count) {
	char *dst8 = (char *)dest;
	char *src8 = (char *)src;
	while (count--) *dst8++ = *src8++;
}

/*
 *  優先度データキュー機能の初期化
 */
#ifdef TOPPERS_pmqini

void
initialize_primsgq(void)
{
	uint_t	i, j;
	PMQCB	*p_pmqcb;
	PMQINIB	*p_pmqinib;

	// for each Priority Message Queue initialize its Control Block
	for (p_pmqcb = pmqcb_table, i = 0; i < tnum_pmq; p_pmqcb++, i++) {
		p_pmqinib = (PMQINIB *)&(pmqinib_table[i]);
		// initialize the memory buffers
		for (j = 0; j < p_pmqinib->pmqcnt; j++) {
			p_pmqinib->p_pmqmb[j].data = (intptr_t)
					(p_pmqinib->p_pmqbuff + (j * p_pmqinib->maxmsgsz));
		}
		queue_initialize(&(p_pmqcb->swait_queue));
		p_pmqcb->p_pmqinib = p_pmqinib;
		queue_initialize(&(p_pmqcb->rwait_queue));
		p_pmqcb->count = 0U;	// no messages
		p_pmqcb->p_head = NULL; // no messages
		p_pmqcb->unused = 0U;   // index of the next free block
		p_pmqcb->p_freelist = NULL; // freelist is empty at the beginning
	}
}

#endif /* TOPPERS_pmqini */

/*
 *  優先度データキュー管理領域へのデータの格納
 */
#ifdef TOPPERS_pmqenq

void
enqueue_primsg(PMQCB *p_pmqcb, intptr_t data, uint_t msgsz, PRI datapri)
{
	PMQMB	*p_pmqmb;
	PMQMB	**pp_prev_next, *p_next;

	// get a free message management block
	if (p_pmqcb->p_freelist != NULL) {
		// if there are free blocks in the freelist, take one
		p_pmqmb = p_pmqcb->p_freelist;
		p_pmqcb->p_freelist = p_pmqmb->p_next;
	}
	else {
		// if no free blocks in freelist, take one more from the initial region
		p_pmqmb = p_pmqcb->p_pmqinib->p_pmqmb + p_pmqcb->unused;
		p_pmqcb->unused++;
	}

	// copy the data, size and priority to the message block
	pmq_memcpy(p_pmqmb->data, data, msgsz);
	p_pmqmb->msgsz = msgsz;
	p_pmqmb->datapri = datapri;

	// insert the message block in the queue at the appropriate priority
	pp_prev_next = &(p_pmqcb->p_head);
	while ((p_next = *pp_prev_next) != NULL) {
		if (p_next->datapri > datapri) {
			break;
		}
		pp_prev_next = &(p_next->p_next);
	}
	p_pmqmb->p_next = p_next;
	*pp_prev_next = p_pmqmb;
	p_pmqcb->count++;
}

#endif /* TOPPERS_pmqenq */

/*
 *  優先度データキュー管理領域からのデータの取出し
 */
#ifdef TOPPERS_pmqdeq

void
dequeue_primsg(PMQCB *p_pmqcb, intptr_t buffer, uint_t *p_msgsz,
			   PRI *p_datapri)
{
	PMQMB	*p_pmqmb;

	// extract the message block at the head of the queue (p_head)
	// Note: messages are inserted in priority order in "enqueue_primsg"
	p_pmqmb = p_pmqcb->p_head;
	p_pmqcb->p_head = p_pmqmb->p_next;
	p_pmqcb->count--;

	// copy the values into the user variables
	pmq_memcpy(buffer, p_pmqmb->data, p_pmqmb->msgsz);
	*p_msgsz = p_pmqmb->msgsz;
	*p_datapri = p_pmqmb->datapri;

	// add the dequeued block to the freelist (in the head) of message blocks
	p_pmqmb->p_next = p_pmqcb->p_freelist;
	p_pmqcb->p_freelist = p_pmqmb;
}

#endif /* TOPPERS_pmqdeq */

/*
 *  優先度データキューへのデータ送信
 */
#ifdef TOPPERS_pmqsnd

bool_t
send_primsg(PMQCB *p_pmqcb, intptr_t data, uint_t msgsz,
			 PRI datapri, bool_t *p_reqdsp)
{
	TCB		*p_tcb;

	if (!queue_empty(&(p_pmqcb->rwait_queue))) {
		// if somebody is waiting for our message, give it to him
		p_tcb = (TCB *) queue_delete_next(&(p_pmqcb->rwait_queue));
		pmq_memcpy(((WINFO_PMQ *)(p_tcb->p_winfo))->data, data, msgsz);
		((WINFO_PMQ *)(p_tcb->p_winfo))->msgsz = msgsz;
		((WINFO_PMQ *)(p_tcb->p_winfo))->datapri = datapri;
		*p_reqdsp = wait_complete(p_tcb);
		return(true);
	}
	else if (p_pmqcb->count < p_pmqcb->p_pmqinib->pmqcnt) {
		// otherwise we put it in the queue
		enqueue_primsg(p_pmqcb, data, msgsz, datapri);
		*p_reqdsp = false;
		return(true);
	}
	else {
		// if there is no space in the queue we return false
		return(false);
	}
}

#endif /* TOPPERS_pmqsnd */

/*
 *  優先度データキューからのデータ受信
 */
#ifdef TOPPERS_pmqrcv

bool_t
receive_primsg(PMQCB *p_pmqcb, intptr_t buffer, uint_t *p_msgsz,
				PRI *p_datapri, bool_t *p_reqdsp)
{
	TCB		*p_tcb;
	intptr_t data;
	uint_t	msgsz;
	PRI		datapri;

	if (p_pmqcb->count > 0U) {
		// if there are messages in the queue, we extract one of them
		dequeue_primsg(p_pmqcb, buffer, p_msgsz, p_datapri);
		if (!queue_empty(&(p_pmqcb->swait_queue))) {
			// if a sender was waiting (because he had no space) let him 
			// send the message now and wake it up
			p_tcb = (TCB *) queue_delete_next(&(p_pmqcb->swait_queue));
			data = ((WINFO_PMQ *)(p_tcb->p_winfo))->data;
			msgsz = ((WINFO_PMQ *)(p_tcb->p_winfo))->msgsz;
			datapri = ((WINFO_PMQ *)(p_tcb->p_winfo))->datapri;
			enqueue_primsg(p_pmqcb, data, msgsz, datapri);
			*p_reqdsp = wait_complete(p_tcb);
		}
		else {
			*p_reqdsp = false;
		}
		return(true);
	}
	else if (!queue_empty(&(p_pmqcb->swait_queue))) {
		// if no messages but a sender is waiting (it can happen) let him send
		p_tcb = (TCB *) queue_delete_next(&(p_pmqcb->swait_queue));
		*p_msgsz = ((WINFO_PMQ *)(p_tcb->p_winfo))->msgsz;
		*p_datapri = ((WINFO_PMQ *)(p_tcb->p_winfo))->datapri;
		pmq_memcpy(buffer, ((WINFO_PMQ *)(p_tcb->p_winfo))->data, *p_msgsz);
		*p_reqdsp = wait_complete(p_tcb);
		return(true);
	}
	else {
		// if no messages return false
		return(false);
	}
}

#endif /* TOPPERS_pmqrcv */

/*
 *  Send to a priority message queue
 */
#ifdef TOPPERS_snd_pmq

ER
snd_pmq(ID pmqid, intptr_t data, uint_t msgsz, PRI datapri)
{
	PMQCB	*p_pmqcb;
	WINFO_PMQ winfo_pmq;
	bool_t	reqdsp;
	ER		ercd;

	LOG_SND_PMQ_ENTER(pmqid, data, msgsz, datapri);
	CHECK_DISPATCH();
	CHECK_PMQID(pmqid); // 1..num msg queues (tmax_pmqid, by configurator)
	
	p_pmqcb = get_pmqcb(pmqid); // get control block (pmqcb_table[id-1])
	CHECK_PAR(TMIN_MQPRI <= datapri && datapri <= p_pmqcb->p_pmqinib->maxmpri);
	CHECK_PAR(msgsz <= p_pmqcb->p_pmqinib->maxmsgsz);

	t_lock_cpu();
	if (send_primsg(p_pmqcb, data, msgsz, datapri, &reqdsp)) {
		// the data was sent
		if (reqdsp) {
			// a waiting thread was woken up so we need to dispatch
			dispatch();
		}
		ercd = E_OK;
	}
	else {
		// the data was not sent (no space) so the thread gets blocked until
		// another thread obtains one of the messages (Note: we dont copy
		// the buffered data yet, only the pointer)
		winfo_pmq.data = data;
		winfo_pmq.msgsz = msgsz;
		winfo_pmq.datapri = datapri;
		p_runtsk->tstat = (TS_WAITING | TS_WAIT_SPMQ);
		wobj_make_wait((WOBJCB *) p_pmqcb, (WINFO_WOBJ *) &winfo_pmq);
		dispatch();
		ercd = winfo_pmq.winfo.wercd;
	}
	t_unlock_cpu();

  error_exit:
	LOG_SND_PMQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_snd_pmq */

/*
 *  優先度データキューへの送信（ポーリング）
 */
#ifdef TOPPERS_psnd_pmq

ER
psnd_pmq(ID pmqid, intptr_t data, uint_t msgsz, PRI datapri)
{
	PMQCB	*p_pmqcb;
	bool_t	reqdsp;
	ER		ercd;

	LOG_PSND_PMQ_ENTER(pmqid, data, msgsz, datapri);
	CHECK_TSKCTX_UNL();
	CHECK_PMQID(pmqid);
	p_pmqcb = get_pmqcb(pmqid);
	CHECK_PAR(TMIN_MQPRI <= datapri && datapri <= p_pmqcb->p_pmqinib->maxmpri);
	CHECK_PAR(msgsz <= p_pmqcb->p_pmqinib->maxmsgsz);

	t_lock_cpu();
	if (send_primsg(p_pmqcb, data, msgsz, datapri, &reqdsp)) {
		if (reqdsp) {
			dispatch();
		}
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	t_unlock_cpu();

  error_exit:
	LOG_PSND_PMQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_psnd_pmq */

/*
 *  優先度データキューへの送信（ポーリング，非タスクコンテキスト用）
 */
#ifdef TOPPERS_ipsnd_pmq

ER
ipsnd_pmq(ID pmqid, intptr_t data, uint_t msgsz, PRI datapri)
{
	PMQCB	*p_pmqcb;
	bool_t	reqdsp;
	ER		ercd;

	LOG_IPSND_PMQ_ENTER(pmqid, data, msgsz, datapri);
	CHECK_INTCTX_UNL();
	CHECK_PMQID(pmqid);
	p_pmqcb = get_pmqcb(pmqid);
	CHECK_PAR(TMIN_MQPRI <= datapri && datapri <= p_pmqcb->p_pmqinib->maxmpri);
	CHECK_PAR(msgsz <= p_pmqcb->p_pmqinib->maxmsgsz);

	i_lock_cpu();
	if (send_primsg(p_pmqcb, data, msgsz, datapri, &reqdsp)) {
		if (reqdsp) {
			reqflg = true;
		}
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	i_unlock_cpu();

  error_exit:
	LOG_IPSND_PMQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_ipsnd_pmq */

/*
 *  優先度データキューへの送信（タイムアウトあり）
 */
#ifdef TOPPERS_tsnd_pmq

ER
tsnd_pmq(ID pmqid, intptr_t data, uint_t msgsz, PRI datapri, TMO tmout)
{
	PMQCB	*p_pmqcb;
	WINFO_PMQ winfo_pmq;
	TMEVTB	tmevtb;
	bool_t	reqdsp;
	ER		ercd;

	LOG_TSND_PMQ_ENTER(pmqid, data, msgsz, datapri, tmout);
	CHECK_DISPATCH();
	CHECK_PMQID(pmqid);
	CHECK_TMOUT(tmout);
	p_pmqcb = get_pmqcb(pmqid);
	CHECK_PAR(TMIN_MQPRI <= datapri && datapri <= p_pmqcb->p_pmqinib->maxmpri);
	CHECK_PAR(msgsz <= p_pmqcb->p_pmqinib->maxmsgsz);

	t_lock_cpu();
	if (send_primsg(p_pmqcb, data, msgsz, datapri, &reqdsp)) {
		if (reqdsp) {
			dispatch();
		}
		ercd = E_OK;
	}
	else if (tmout == TMO_POL) {
		ercd = E_TMOUT;
	}
	else {
		winfo_pmq.data = data;
		winfo_pmq.msgsz = msgsz;
		winfo_pmq.datapri = datapri;
		p_runtsk->tstat = (TS_WAITING | TS_WAIT_SPMQ);
		wobj_make_wait_tmout((WOBJCB *) p_pmqcb, (WINFO_WOBJ *) &winfo_pmq,
														&tmevtb, tmout);
		dispatch();
		ercd = winfo_pmq.winfo.wercd;
	}
	t_unlock_cpu();

  error_exit:
	LOG_TSND_PMQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_tsnd_pmq */

/*
 *  優先度データキューからの受信
 */
#ifdef TOPPERS_rcv_pmq

ER
rcv_pmq(ID pmqid, intptr_t buffer, uint_t *p_msgsz, PRI *p_datapri)
{
	PMQCB	*p_pmqcb;
	WINFO_PMQ winfo_pmq;
	bool_t	reqdsp;
	ER		ercd;

	LOG_RCV_PMQ_ENTER(pmqid, buffer, p_msgsz, p_datapri);
	CHECK_DISPATCH();
	CHECK_PMQID(pmqid);
	p_pmqcb = get_pmqcb(pmqid);

	t_lock_cpu();
	if (receive_primsg(p_pmqcb, buffer, p_msgsz, p_datapri, &reqdsp)) {
		// we received a message
		if (reqdsp) {
			// we received it from a thread that was blocked before and now
			// it was awaken so we need to dispatch
			dispatch();
		}
		ercd = E_OK;
	}
	else {
		// no message was available so we block until we receive one
		p_runtsk->tstat = (TS_WAITING | TS_WAIT_RPMQ);
		make_wait(&(winfo_pmq.winfo));
		queue_insert_prev(&(p_pmqcb->rwait_queue), &(p_runtsk->task_queue));
		winfo_pmq.p_pmqcb = p_pmqcb;
		LOG_TSKSTAT(p_runtsk);
		dispatch();
		ercd = winfo_pmq.winfo.wercd;
		if (ercd == E_OK) {
			pmq_memcpy(buffer, winfo_pmq.data, winfo_pmq.msgsz) ;
			*p_msgsz = winfo_pmq.msgsz;
			*p_datapri = winfo_pmq.datapri;
		}
	}
	t_unlock_cpu();

  error_exit:
	LOG_RCV_PMQ_LEAVE(ercd, buffer, *p_msgsz, *p_datapri);
	return(ercd);
}

#endif /* TOPPERS_rcv_pmq */

/*
 *  優先度データキューからの受信（ポーリング）
 */
#ifdef TOPPERS_prcv_pmq

ER
prcv_pmq(ID pmqid, intptr_t buffer, uint_t *p_msgsz, PRI *p_datapri)
{
	PMQCB	*p_pmqcb;
	bool_t	reqdsp;
	ER		ercd;

	LOG_PRCV_PMQ_ENTER(pmqid, buffer, p_msgsz, p_datapri);
	CHECK_TSKCTX_UNL();
	CHECK_PMQID(pmqid);
	p_pmqcb = get_pmqcb(pmqid);

	t_lock_cpu();
	if (receive_primsg(p_pmqcb, buffer, p_msgsz, p_datapri, &reqdsp)) {
		if (reqdsp) {
			dispatch();
		}
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	t_unlock_cpu();

  error_exit:
	LOG_PRCV_PMQ_LEAVE(ercd, buffer, *p_msgsz, *p_datapri);
	return(ercd);
}

#endif /* TOPPERS_prcv_pmq */

/*
 *  優先度データキューからの受信（タイムアウトあり）
 */
#ifdef TOPPERS_trcv_pmq

ER
trcv_pmq(ID pmqid, intptr_t buffer, uint_t *p_msgsz, PRI *p_datapri, TMO tmout)
{
	PMQCB	*p_pmqcb;
	WINFO_PMQ winfo_pmq;
	TMEVTB	tmevtb;
	bool_t	reqdsp;
	ER		ercd;

	LOG_TRCV_PMQ_ENTER(pmqid, buffer, p_msgsz, p_datapri, tmout);
	CHECK_DISPATCH();
	CHECK_PMQID(pmqid);
	CHECK_TMOUT(tmout);
	p_pmqcb = get_pmqcb(pmqid);

	t_lock_cpu();
	if (receive_primsg(p_pmqcb, buffer, p_msgsz, p_datapri, &reqdsp)) {
		if (reqdsp) {
			dispatch();
		}
		ercd = E_OK;
	}
	else if (tmout == TMO_POL) {
		ercd = E_TMOUT;
	}
	else {
		p_runtsk->tstat = (TS_WAITING | TS_WAIT_RPMQ);
		make_wait_tmout(&(winfo_pmq.winfo), &tmevtb, tmout);
		queue_insert_prev(&(p_pmqcb->rwait_queue), &(p_runtsk->task_queue));
		winfo_pmq.p_pmqcb = p_pmqcb;
		LOG_TSKSTAT(p_runtsk);
		dispatch();
		ercd = winfo_pmq.winfo.wercd;
		if (ercd == E_OK) {
			pmq_memcpy(buffer, winfo_pmq.data, winfo_pmq.msgsz) ;
			*p_msgsz = winfo_pmq.msgsz;
			*p_datapri = winfo_pmq.datapri;
		}
	}
	t_unlock_cpu();

  error_exit:
	LOG_TRCV_PMQ_LEAVE(ercd, buffer, *p_msgsz, *p_datapri);
	return(ercd);
}

#endif /* TOPPERS_trcv_pmq */

/*
 *  優先度データキューの再初期化
 */
#ifdef TOPPERS_ini_pmq

ER
ini_pmq(ID pmqid)
{
	PMQCB	*p_pmqcb;
	bool_t	dspreq;
	ER		ercd;

	LOG_INI_PMQ_ENTER(pmqid);
	CHECK_TSKCTX_UNL();
	CHECK_PMQID(pmqid);
	p_pmqcb = get_pmqcb(pmqid);

	t_lock_cpu();
	dspreq = init_wait_queue(&(p_pmqcb->swait_queue));
	if (init_wait_queue(&(p_pmqcb->rwait_queue))) {
		dspreq = true;
	}
	p_pmqcb->count = 0U;
	p_pmqcb->p_head = NULL;
	p_pmqcb->unused = 0U;
	p_pmqcb->p_freelist = NULL;
	if (dspreq) {
		dispatch();
	}
	ercd = E_OK;
	t_unlock_cpu();

  error_exit:
	LOG_INI_PMQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_ini_pmq */

/*
 *  優先度データキューの状態参照
 */
#ifdef TOPPERS_ref_pmq

ER
ref_pmq(ID pmqid, T_RPMQ *pk_rpmq)
{
	PMQCB	*p_pmqcb;
	ER		ercd;

	LOG_REF_PMQ_ENTER(pmqid, pk_rpmq);
	CHECK_TSKCTX_UNL();
	CHECK_PMQID(pmqid);
	p_pmqcb = get_pmqcb(pmqid);

	t_lock_cpu();
	pk_rpmq->stskid = wait_tskid(&(p_pmqcb->swait_queue));
	pk_rpmq->rtskid = wait_tskid(&(p_pmqcb->rwait_queue));
	pk_rpmq->spmqcnt = p_pmqcb->count;
	ercd = E_OK;
	t_unlock_cpu();

  error_exit:
	LOG_REF_PMQ_LEAVE(ercd, pk_rpmq);
	return(ercd);
}

#endif /* TOPPERS_ref_pmq */
