/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: wait.c 532 2010-02-16 10:09:15Z ertl-honda $
 */

/*
 *		�Ԥ����ִ����⥸�塼��
 */

#include "kernel_impl.h"
#include "wait.h"
#include "mp.h"

/*
 *  �Ԥ����֤ؤΰܹԡʥ����ॢ���Ȼ����
 */
#ifdef TOPPERS_waimake

void
make_wait_tmout(TMO tmout, PCB *p_pcb)
{
	TCB *p_runtsk = p_pcb->p_runtsk;

	(void) make_non_runnable(p_runtsk);
	if (tmout > 0) {
		tmevtb_enqueue(p_pcb->p_tevtcb, &(p_runtsk->tmevtb), (RELTIM) tmout,
						(CBACK) wait_tmout, (void *) p_runtsk);
	}
	else {
		assert(tmout == TMO_FEVR);
		(p_runtsk->tmevtb).callback = NULL;
	}
}

#endif /* TOPPERS_waimake */

/*
 *  �Ԥ�����Τ���Υ��������֤ι���
 *
 *  p_tcb�ǻ��ꤵ��륿�������Ԥ��������褦���������֤򹹿����롥
 *  �Ԥ�������륿�������¹ԤǤ�����֤ˤʤ���ϡ���ǥ����塼�ˤĤ�
 *  �����ޤ����ǥ����ѥå���ɬ�פʾ��ˤ�true���֤���
 *    
 *  �Ԥ����֤���������ϡ�ɬ�����δؿ���ƤӽФ����ᡤ�ǥåɥ�å�
 *  ����Τ���Υڥ�ǥ��󥰥ե饰�Υ����å���ޤȤ�ƹԤ���
 */
Inline bool_t
make_non_wait(TCB *p_tcb)
{
	assert(TSTAT_WAITING(p_tcb->tstat));

#if TTYPE_KLOCK != G_KLOCK
	/* ͥ�����ѹ��ե饰�����å� */
	if (p_tcb->pend_chgpri) {
		p_tcb->priority = p_tcb->pend_newpri;
		/* ͥ�����ѹ��ե饰�Υ��ꥢ */
		p_tcb->pend_chgpri = false;
	}

	/* �����������Ԥ������α���ꥢ */
	p_tcb->pend_relwai = false;
#endif /* TTYPE_KLOCK != G_KLOCK */

	if (!TSTAT_SUSPENDED(p_tcb->tstat)) {
		/*
		 *  �Ԥ����֤���¹ԤǤ�����֤ؤ�����
		 */
		p_tcb->tstat = TS_RUNNABLE;
		LOG_TSKSTAT(p_tcb);
		return(make_runnable(p_tcb));
	}
	else {
		/*
		 *  ����Ԥ����֤��鶯���Ԥ����֤ؤ�����
		 */
		p_tcb->tstat = TS_SUSPENDED;
		LOG_TSKSTAT(p_tcb);
		return(false);
	}
}

/*
 *  �Ԥ����
 */
#ifdef TOPPERS_waicmp

bool_t
wait_complete(TCB *p_tcb)
{
	wait_dequeue_tmevtb(p_tcb);
	p_tcb->wercd = E_OK;
	return(make_non_wait(p_tcb));
}

#endif /* TOPPERS_waicmp */

/*
 *  �����ॢ���Ȥ�ȼ���Ԥ����
 *
 *  �ܴؿ��� make_wait_tmout() �ˤ�꥿����ҡ��פ���Ͽ���쥿���ॢ����
 *  ���˥�����Хå��ؿ��Ȥ��ƸƤӽФ���롥���Τ��ᡤCPU��å����֤��ġ�
 *  ��������å��������֤ǸƤӽФ���롥 
 */
#ifdef TOPPERS_waitmo

void
wait_tmout(TCB *p_tcb)
{
	PCB		*my_p_pcb;

#if TTYPE_KLOCK == G_KLOCK
	/*
	 *  ���㥤����ȥ�å��ξ���ñ��wait���塼�������������
	 */
	wait_dequeue_wobj(p_tcb);
#else /* TTYPE_KLOCK != G_KLOCK */
	/*
	 *  ����¾�Υ�å�����
	 *
	 *  ���֥�������å������������ϡ��ǥåɥ�å������Ԥ�
	 */
	if (TSTAT_WAIT_WOBJ(p_tcb->tstat)) {
		/* ���֥��������Ԥ��ξ�� */
		WOBJCB* volatile p_wobjcb = p_tcb->p_wobjcb;
		p_tcb->pend_relwai = true;
		release_tsk_lock(p_tcb->p_pcb);

		/*
		 * ���餿���
		 * ���֥������ȥ�å� -> ��������å��ν�ǥ�å������
		 */
	  retry:
		i_acquire_obj_lock(&GET_OBJLOCK(p_wobjcb));
		if (i_acquire_nested_tsk_lock(p_tcb, &GET_OBJLOCK(p_wobjcb)) == NULL){
			goto retry;
		}

		/* �������ξ��֤��Ѳ����Ƥ��ʤ��������å� */
		if (!(p_tcb->pend_relwai)) {
			/* ����¾�βս���Ԥ�����������ʤ��줿 */
			release_obj_lock(&GET_OBJLOCK(p_wobjcb));
			return;
		}
		p_tcb->pend_relwai = false;
		wait_dequeue_wobj(p_tcb);
		release_obj_lock(&GET_OBJLOCK(p_wobjcb));
	}
#endif /* TTYPE_KLOCK != G_KLOCK */

	p_tcb->wercd = E_TMOUT;
	if (make_non_wait(p_tcb)) {
		if (dispatch_request(p_tcb->p_pcb)) {
			p_tcb->p_pcb->reqflg = true;
		}
	}

	/*
	 *  ������ͥ���٤ι⤤����ߤ�����դ��롥
	 */
	my_p_pcb = get_my_p_pcb();
	release_tsk_lock(my_p_pcb);
	i_unlock_cpu();
	i_lock_cpu();
	i_acquire_tsk_lock_self();
}

#endif /* TOPPERS_waitmo */

#ifdef TOPPERS_waitmook

void
wait_tmout_ok(TCB *p_tcb)
{
	PCB *p_pcb = p_tcb->p_pcb;
	PCB *my_p_pcb;

	p_tcb->wercd = E_OK;
	if (make_non_wait(p_tcb)) {
		if (dispatch_request(p_tcb->p_pcb)) {
			p_pcb->reqflg = true;
		}
	}

	/*
	 *  ������ͥ���٤ι⤤����ߤ�����դ��롥
	 */
	my_p_pcb = get_my_p_pcb();
	release_tsk_lock(my_p_pcb);
	i_unlock_cpu();
	i_lock_cpu();
	i_acquire_tsk_lock_self();
}

#endif /* TOPPERS_waitmook */

/*
 *  �Ԥ����֤ζ������
 */
#ifdef TOPPERS_wairel

bool_t
wait_release(TCB *p_tcb)
{
	wait_dequeue_wobj(p_tcb);
	wait_dequeue_tmevtb(p_tcb);
	p_tcb->wercd = E_RLWAI;
	return(make_non_wait(p_tcb));
}

#endif /* TOPPERS_wairel */

/*
 *  �¹���Υ�������Ʊ�����̿����֥������Ȥ��Ԥ����塼�ؤ�����
 *
 *  �¹���Υ�������Ʊ�����̿����֥������Ȥ��Ԥ����塼���������롥��
 *  �֥������Ȥ�°���˱����ơ�FIFO��ޤ��ϥ�����ͥ���ٽ���������롥
 */
Inline void
wobj_queue_insert(WOBJCB *p_wobjcb, TCB *p_runtsk)
{
	if ((p_wobjcb->p_wobjinib->wobjatr & TA_TPRI) != 0U) {
		queue_insert_tpri(&(p_wobjcb->wait_queue), p_runtsk);
	}
	else {
		queue_insert_prev(&(p_wobjcb->wait_queue), &(p_runtsk->task_queue));
	}
}

/*
 *  Ʊ�����̿����֥������Ȥ��Ф����Ԥ����֤ؤΰܹ�
 */
#ifdef TOPPERS_wobjwai

void
wobj_make_wait(WOBJCB *p_wobjcb, TCB *p_runtsk)
{
	make_wait(p_runtsk);
	wobj_queue_insert(p_wobjcb, p_runtsk);
	p_runtsk->p_wobjcb = p_wobjcb;
	LOG_TSKSTAT(p_runtsk);
}

#endif /* TOPPERS_wobjwai */
#ifdef TOPPERS_wobjwaitmo

void
wobj_make_wait_tmout(WOBJCB *p_wobjcb, TMO tmout, TCB *p_runtsk)
{
	make_wait_tmout(tmout, p_runtsk->p_pcb);
	wobj_queue_insert(p_wobjcb, p_runtsk);
	p_runtsk->p_wobjcb = p_wobjcb;
	LOG_TSKSTAT(p_runtsk);
}

#endif /* TOPPERS_wobjwaitmo */

/*
 *  �Ԥ����塼�ν����
 */
#ifdef TOPPERS_iniwque

bool_t
init_wait_queue(QUEUE *p_wait_queue)
{
	TCB		*p_tcb;
	bool_t	dspreq = false;
	PCB		*p_pcb;     

	while (!queue_empty(p_wait_queue)) {
		p_tcb = (TCB *) queue_delete_next(p_wait_queue);
		/* ��������å��μ��� */
		p_pcb = acquire_nested_tsk_lock_without_preemption(p_tcb);
		wait_dequeue_tmevtb(p_tcb);
		p_tcb->wercd = E_DLT;
		if (make_non_wait(p_tcb)) {
			if (dispatch_request(p_pcb)) {
				dspreq = true;
			}
		}
		release_tsk_lock(p_pcb);
	}

	return(dspreq);
}

#endif /* TOPPERS_iniwque */
