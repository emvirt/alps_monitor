/* This file is generated from kernel_rename.def by genrename. */

#ifndef TOPPERS_KERNEL_RENAME_H
#define TOPPERS_KERNEL_RENAME_H

/*
 *  startup.c
 */
#define exit_kernel					_kernel_exit_kernel

/*
 *  mp.c
 */
#define giant_lock					_kernel_giant_lock
#define ext_ker_reqflg				_kernel_ext_ker_reqflg
#define ipi_handler					_kernel_ipi_handler
#define dispatch_request			_kernel_dispatch_request
#define ext_ker_request				_kernel_ext_ker_request
#define initialize_pcb				_kernel_initialize_pcb
#define barrier_sync				_kernel_barrier_sync

/*
 *  task.c
 */
#define initialize_task				_kernel_initialize_task
#define search_schedtsk				_kernel_search_schedtsk
#define make_runnable				_kernel_make_runnable
#define make_non_runnable			_kernel_make_non_runnable
#define make_dormant				_kernel_make_dormant
#define make_active					_kernel_make_active
#define change_priority				_kernel_change_priority
#define rotate_ready_queue			_kernel_rotate_ready_queue
#define call_texrtn					_kernel_call_texrtn
#define dispatch_call_texrtn		_kernel_dispatch_call_texrtn
#define calltex						_kernel_calltex
#define migrate_self				_kernel_migrate_self
#define exit_and_migrate_self		_kernel_exit_and_migrate_self

/*
 *  wait.c
 */
#define make_wait_tmout				_kernel_make_wait_tmout
#define wait_complete				_kernel_wait_complete
#define wait_tmout					_kernel_wait_tmout
#define wait_tmout_ok				_kernel_wait_tmout_ok
#define wait_release				_kernel_wait_release
#define wobj_make_wait				_kernel_wobj_make_wait
#define wobj_make_wait_tmout		_kernel_wobj_make_wait_tmout
#define init_wait_queue				_kernel_init_wait_queue

/*
 *  time_event.c
 */
#define current_time				_kernel_current_time
#define next_time					_kernel_next_time
#define next_subtime				_kernel_next_subtime
#define pend_update					_kernel_pend_update
#define last_index					_kernel_last_index
#define initialize_tmevt			_kernel_initialize_tmevt
#define tmevt_up					_kernel_tmevt_up
#define tmevt_down					_kernel_tmevt_down
#define tmevtb_insert				_kernel_tmevtb_insert
#define tmevtb_delete				_kernel_tmevtb_delete
#define tmevt_lefttim				_kernel_tmevt_lefttim
#define signal_time					_kernel_signal_time
#define get_my_current_time			_kernel_get_my_current_time

/*
 *  semaphore.c
 */
#define initialize_semaphore		_kernel_initialize_semaphore

/*
 *  eventflag.c
 */
#define initialize_eventflag		_kernel_initialize_eventflag
#define check_flg_cond				_kernel_check_flg_cond

/*
 *  dataqueue.c
 */
#define initialize_dataqueue		_kernel_initialize_dataqueue
#define enqueue_data				_kernel_enqueue_data
#define force_enqueue_data			_kernel_force_enqueue_data
#define dequeue_data				_kernel_dequeue_data
#define send_data					_kernel_send_data
#define force_send_data				_kernel_force_send_data
#define receive_data				_kernel_receive_data

/*
 *  pridataq.c
 */
#define initialize_pridataq			_kernel_initialize_pridataq
#define enqueue_pridata				_kernel_enqueue_pridata
#define dequeue_pridata				_kernel_dequeue_pridata
#define send_pridata				_kernel_send_pridata
#define receive_pridata				_kernel_receive_pridata

/*
 *  mailbox.c
 */
#define initialize_mailbox			_kernel_initialize_mailbox

/*
 *  mempfix.c
 */
#define initialize_mempfix			_kernel_initialize_mempfix
#define get_mpf_block				_kernel_get_mpf_block

/*
 *  cyclic.c
 */
#define initialize_cyclic			_kernel_initialize_cyclic
#define call_cychdr					_kernel_call_cychdr

/*
 *  alarm.c
 */
#define initialize_alarm			_kernel_initialize_alarm
#define call_almhdr					_kernel_call_almhdr

/*
 *  spin_lock.c
 */
#define initialize_spin_lock		_kernel_initialize_spin_lock
#define force_unlock_spin			_kernel_force_unlock_spin

/*
 *  interrupt.c
 */
#define initialize_interrupt		_kernel_initialize_interrupt

/*
 *  exception.c
 */
#define initialize_exception		_kernel_initialize_exception

/*
 *  kernel_cfg.c
 */
#define p_pcb_table					_kernel_p_pcb_table
#define initialize_object			_kernel_initialize_object
#define call_global_inirtn			_kernel_call_global_inirtn
#define call_local_inirtn			_kernel_call_local_inirtn
#define call_global_terrtn			_kernel_call_global_terrtn
#define call_local_terrtn			_kernel_call_local_terrtn
#define tmax_tskid					_kernel_tmax_tskid
#define tinib_table					_kernel_tinib_table
#define torder_table				_kernel_torder_table
#define tcb_table					_kernel_tcb_table
#define p_tcb_table					_kernel_p_tcb_table
#define tmax_semid					_kernel_tmax_semid
#define seminib_table				_kernel_seminib_table
#define semcb_table					_kernel_semcb_table
#define p_semcb_table				_kernel_p_semcb_table
#define tmax_flgid					_kernel_tmax_flgid
#define flginib_table				_kernel_flginib_table
#define flgcb_table					_kernel_flgcb_table
#define p_flgcb_table				_kernel_p_flgcb_table
#define tmax_dtqid					_kernel_tmax_dtqid
#define dtqcb_table					_kernel_dtqcb_table
#define p_dtqcb_table				_kernel_p_dtqcb_table
#define dtqinib_table				_kernel_dtqinib_table
#define tmax_pdqid					_kernel_tmax_pdqid
#define pdqcb_table					_kernel_pdqcb_table
#define p_pdqcb_table				_kernel_p_pdqcb_table
#define pdqinib_table				_kernel_pdqinib_table
#define tmax_mbxid					_kernel_tmax_mbxid
#define mbxcb_table					_kernel_mbxcb_table
#define p_mbxcb_table				_kernel_p_mbxcb_table
#define mbxinib_table				_kernel_mbxinib_table
#define tmax_mpfid					_kernel_tmax_mpfid
#define mpfinib_table				_kernel_mpfinib_table
#define mpfcb_table					_kernel_mpfcb_table
#define p_mpfcb_table				_kernel_p_mpfcb_table
#define tmax_cycid					_kernel_tmax_cycid
#define cycinib_table				_kernel_cycinib_table
#define cyccb_table					_kernel_cyccb_table
#define p_cyccb_table				_kernel_p_cyccb_table
#define tmax_almid					_kernel_tmax_almid
#define alminib_table				_kernel_alminib_table
#define almcb_table					_kernel_almcb_table
#define p_almcb_table				_kernel_p_almcb_table
#define tmax_spnid					_kernel_tmax_spnid
#define spninib_table				_kernel_spninib_table
#define spncb_table					_kernel_spncb_table
#define p_spncb_table				_kernel_p_spncb_table
#define tnum_inhno					_kernel_tnum_inhno
#define inhinib_table				_kernel_inhinib_table
#define tnum_intno					_kernel_tnum_intno
#define intinib_table				_kernel_intinib_table
#define tnum_excno					_kernel_tnum_excno
#define excinib_table				_kernel_excinib_table
#define tmevt_heap					_kernel_tmevt_heap
#define p_tevtcb_table				_kernel_p_tevtcb_table
#define p_tmevt_heap_table			_kernel_p_tmevt_heap_table
#define tevtcb						_kernel_tevtcb
#define istksz_table				_kernel_istksz_table
#define istk_table					_kernel_istk_table
#define istkpt_table				_kernel_istkpt_table

/*
 *  target_config.c, target_support.S
 */
#define dispatch					_kernel_dispatch
#define start_dispatch				_kernel_start_dispatch
#define exit_and_dispatch			_kernel_exit_and_dispatch
#define dispatch_and_migrate		_kernel_dispatch_and_migrate
#define exit_and_migrate			_kernel_exit_and_migrate
#define call_exit_kernel			_kernel_call_exit_kernel
#define target_initialize			_kernel_target_initialize
#define target_exit					_kernel_target_exit
#define default_int_handler			_kernel_default_int_handler
#define default_exc_handler			_kernel_default_exc_handler
#define log_dsp_enter				_kernel_log_dsp_enter
#define log_dsp_leave				_kernel_log_dsp_leave
#define log_inh_enter				_kernel_log_inh_enter
#define log_inh_leave				_kernel_log_inh_leave
#define log_exc_enter				_kernel_log_exc_enter
#define log_exc_leave				_kernel_log_exc_leave

/*
 *  syslog.c
 */
#define p_syslogcb_table			_kernel_p_syslogcb_table


#ifdef TOPPERS_LABEL_ASM

/*
 *  startup.c
 */
#define _exit_kernel				__kernel_exit_kernel

/*
 *  mp.c
 */
#define _giant_lock					__kernel_giant_lock
#define _ext_ker_reqflg				__kernel_ext_ker_reqflg
#define _ipi_handler				__kernel_ipi_handler
#define _dispatch_request			__kernel_dispatch_request
#define _ext_ker_request			__kernel_ext_ker_request
#define _initialize_pcb				__kernel_initialize_pcb
#define _barrier_sync				__kernel_barrier_sync

/*
 *  task.c
 */
#define _initialize_task			__kernel_initialize_task
#define _search_schedtsk			__kernel_search_schedtsk
#define _make_runnable				__kernel_make_runnable
#define _make_non_runnable			__kernel_make_non_runnable
#define _make_dormant				__kernel_make_dormant
#define _make_active				__kernel_make_active
#define _change_priority			__kernel_change_priority
#define _rotate_ready_queue			__kernel_rotate_ready_queue
#define _call_texrtn				__kernel_call_texrtn
#define _dispatch_call_texrtn		__kernel_dispatch_call_texrtn
#define _calltex					__kernel_calltex
#define _migrate_self				__kernel_migrate_self
#define _exit_and_migrate_self		__kernel_exit_and_migrate_self

/*
 *  wait.c
 */
#define _make_wait_tmout			__kernel_make_wait_tmout
#define _wait_complete				__kernel_wait_complete
#define _wait_tmout					__kernel_wait_tmout
#define _wait_tmout_ok				__kernel_wait_tmout_ok
#define _wait_release				__kernel_wait_release
#define _wobj_make_wait				__kernel_wobj_make_wait
#define _wobj_make_wait_tmout		__kernel_wobj_make_wait_tmout
#define _init_wait_queue			__kernel_init_wait_queue

/*
 *  time_event.c
 */
#define _current_time				__kernel_current_time
#define _next_time					__kernel_next_time
#define _next_subtime				__kernel_next_subtime
#define _pend_update				__kernel_pend_update
#define _last_index					__kernel_last_index
#define _initialize_tmevt			__kernel_initialize_tmevt
#define _tmevt_up					__kernel_tmevt_up
#define _tmevt_down					__kernel_tmevt_down
#define _tmevtb_insert				__kernel_tmevtb_insert
#define _tmevtb_delete				__kernel_tmevtb_delete
#define _tmevt_lefttim				__kernel_tmevt_lefttim
#define _signal_time				__kernel_signal_time
#define _get_my_current_time		__kernel_get_my_current_time

/*
 *  semaphore.c
 */
#define _initialize_semaphore		__kernel_initialize_semaphore

/*
 *  eventflag.c
 */
#define _initialize_eventflag		__kernel_initialize_eventflag
#define _check_flg_cond				__kernel_check_flg_cond

/*
 *  dataqueue.c
 */
#define _initialize_dataqueue		__kernel_initialize_dataqueue
#define _enqueue_data				__kernel_enqueue_data
#define _force_enqueue_data			__kernel_force_enqueue_data
#define _dequeue_data				__kernel_dequeue_data
#define _send_data					__kernel_send_data
#define _force_send_data			__kernel_force_send_data
#define _receive_data				__kernel_receive_data

/*
 *  pridataq.c
 */
#define _initialize_pridataq		__kernel_initialize_pridataq
#define _enqueue_pridata			__kernel_enqueue_pridata
#define _dequeue_pridata			__kernel_dequeue_pridata
#define _send_pridata				__kernel_send_pridata
#define _receive_pridata			__kernel_receive_pridata

/*
 *  mailbox.c
 */
#define _initialize_mailbox			__kernel_initialize_mailbox

/*
 *  mempfix.c
 */
#define _initialize_mempfix			__kernel_initialize_mempfix
#define _get_mpf_block				__kernel_get_mpf_block

/*
 *  cyclic.c
 */
#define _initialize_cyclic			__kernel_initialize_cyclic
#define _call_cychdr				__kernel_call_cychdr

/*
 *  alarm.c
 */
#define _initialize_alarm			__kernel_initialize_alarm
#define _call_almhdr				__kernel_call_almhdr

/*
 *  spin_lock.c
 */
#define _initialize_spin_lock		__kernel_initialize_spin_lock
#define _force_unlock_spin			__kernel_force_unlock_spin

/*
 *  interrupt.c
 */
#define _initialize_interrupt		__kernel_initialize_interrupt

/*
 *  exception.c
 */
#define _initialize_exception		__kernel_initialize_exception

/*
 *  kernel_cfg.c
 */
#define _p_pcb_table				__kernel_p_pcb_table
#define _initialize_object			__kernel_initialize_object
#define _call_global_inirtn			__kernel_call_global_inirtn
#define _call_local_inirtn			__kernel_call_local_inirtn
#define _call_global_terrtn			__kernel_call_global_terrtn
#define _call_local_terrtn			__kernel_call_local_terrtn
#define _tmax_tskid					__kernel_tmax_tskid
#define _tinib_table				__kernel_tinib_table
#define _torder_table				__kernel_torder_table
#define _tcb_table					__kernel_tcb_table
#define _p_tcb_table				__kernel_p_tcb_table
#define _tmax_semid					__kernel_tmax_semid
#define _seminib_table				__kernel_seminib_table
#define _semcb_table				__kernel_semcb_table
#define _p_semcb_table				__kernel_p_semcb_table
#define _tmax_flgid					__kernel_tmax_flgid
#define _flginib_table				__kernel_flginib_table
#define _flgcb_table				__kernel_flgcb_table
#define _p_flgcb_table				__kernel_p_flgcb_table
#define _tmax_dtqid					__kernel_tmax_dtqid
#define _dtqcb_table				__kernel_dtqcb_table
#define _p_dtqcb_table				__kernel_p_dtqcb_table
#define _dtqinib_table				__kernel_dtqinib_table
#define _tmax_pdqid					__kernel_tmax_pdqid
#define _pdqcb_table				__kernel_pdqcb_table
#define _p_pdqcb_table				__kernel_p_pdqcb_table
#define _pdqinib_table				__kernel_pdqinib_table
#define _tmax_mbxid					__kernel_tmax_mbxid
#define _mbxcb_table				__kernel_mbxcb_table
#define _p_mbxcb_table				__kernel_p_mbxcb_table
#define _mbxinib_table				__kernel_mbxinib_table
#define _tmax_mpfid					__kernel_tmax_mpfid
#define _mpfinib_table				__kernel_mpfinib_table
#define _mpfcb_table				__kernel_mpfcb_table
#define _p_mpfcb_table				__kernel_p_mpfcb_table
#define _tmax_cycid					__kernel_tmax_cycid
#define _cycinib_table				__kernel_cycinib_table
#define _cyccb_table				__kernel_cyccb_table
#define _p_cyccb_table				__kernel_p_cyccb_table
#define _tmax_almid					__kernel_tmax_almid
#define _alminib_table				__kernel_alminib_table
#define _almcb_table				__kernel_almcb_table
#define _p_almcb_table				__kernel_p_almcb_table
#define _tmax_spnid					__kernel_tmax_spnid
#define _spninib_table				__kernel_spninib_table
#define _spncb_table				__kernel_spncb_table
#define _p_spncb_table				__kernel_p_spncb_table
#define _tnum_inhno					__kernel_tnum_inhno
#define _inhinib_table				__kernel_inhinib_table
#define _tnum_intno					__kernel_tnum_intno
#define _intinib_table				__kernel_intinib_table
#define _tnum_excno					__kernel_tnum_excno
#define _excinib_table				__kernel_excinib_table
#define _tmevt_heap					__kernel_tmevt_heap
#define _p_tevtcb_table				__kernel_p_tevtcb_table
#define _p_tmevt_heap_table			__kernel_p_tmevt_heap_table
#define _tevtcb						__kernel_tevtcb
#define _istksz_table				__kernel_istksz_table
#define _istk_table					__kernel_istk_table
#define _istkpt_table				__kernel_istkpt_table

/*
 *  target_config.c, target_support.S
 */
#define _dispatch					__kernel_dispatch
#define _start_dispatch				__kernel_start_dispatch
#define _exit_and_dispatch			__kernel_exit_and_dispatch
#define _dispatch_and_migrate		__kernel_dispatch_and_migrate
#define _exit_and_migrate			__kernel_exit_and_migrate
#define _call_exit_kernel			__kernel_call_exit_kernel
#define _target_initialize			__kernel_target_initialize
#define _target_exit				__kernel_target_exit
#define _default_int_handler		__kernel_default_int_handler
#define _default_exc_handler		__kernel_default_exc_handler
#define _log_dsp_enter				__kernel_log_dsp_enter
#define _log_dsp_leave				__kernel_log_dsp_leave
#define _log_inh_enter				__kernel_log_inh_enter
#define _log_inh_leave				__kernel_log_inh_leave
#define _log_exc_enter				__kernel_log_exc_enter
#define _log_exc_leave				__kernel_log_exc_leave

/*
 *  syslog.c
 */
#define _p_syslogcb_table			__kernel_p_syslogcb_table


#endif /* TOPPERS_LABEL_ASM */

#include "target_rename.h"

#endif /* TOPPERS_KERNEL_RENAME_H */
