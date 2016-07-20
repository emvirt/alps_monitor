#include "kernel_impl.h"
#include <sil.h>
#include "target_test.h"

#define AIC_ISET (0xfffff12C)

/*
 *  ティック用タイマディスエーブル変数
 */
static volatile bool_t target_timer_disable[TNUM_PRCID];

/*
 *  ティック用タイマワンショット実行
 */
static volatile bool_t target_timer_oneshot[TNUM_PRCID];

/*
 *  タイマハンドラ開始時に呼び出すフック
 *  (falseでsignal_time()を呼び出さない)
 */
bool_t
ttsp_timer_handler_begin_hook(void){
	return !target_timer_disable[x_prc_index()];
}

/*
 *  タイマハンドラ終了時に呼び出すフック
 */
void
ttsp_timer_handler_end_hook(void){
	uint_t prc_index = x_prc_index();
	if (target_timer_oneshot[prc_index]) {
		target_timer_disable[prc_index] = true;
		target_timer_oneshot[prc_index] = false;
	}
}

/*
 *  ティック更新の停止(全プロセッサ)
 */
void
ttsp_target_stop_tick(void)
{
	uint_t i;

	for(i = 1; i <= TNUM_PRCID; i++) {
		ttsp_target_stop_tick_pe(i);
	}
}

/*
 *  ティック更新の停止(特定プロセッサ)
 */
void
ttsp_target_stop_tick_pe(ID prcid)
{
	uint_t prc_index = prcid - 1;
	target_timer_disable[prc_index] = true;
}

/*
 *  ティック更新の再開（全プロセッサ）
 */
void
ttsp_target_start_tick(void)
{
	uint_t i;

	for(i = 1; i <= TNUM_PRCID; i++) {
		ttsp_target_start_tick_pe(i);
	}
}

/*
 *  ティック更新の再開（特定プロセッサ）
 */
void
ttsp_target_start_tick_pe(ID prcid)
{
	uint_t prc_index = prcid - 1;
	target_timer_disable[prc_index] = false;
}

/*
 *  ティックの更新（特定プロセッサ）
 */
void
ttsp_target_gain_tick_pe(ID prcid)
{
	int prc_index = prcid - 1;
	ID pid;

	/*
	 *  前回の更新が終わっていることを確認
	 *  自PEに対してはチェックする必要はないが，チェックしても問題ないため，
	 *  チェックする． 
	 */
	while(target_timer_oneshot[prc_index] == true);

	target_timer_oneshot[prc_index] = true;
	target_timer_disable[prc_index] = false;

	/* 自PEに対しては，ハンドラが終了するまで待ち合わせる */
	sil_get_pid(&pid);
	if (pid == prcid) {
		while(target_timer_oneshot[prc_index] == true);
	}
}

/*
 *  ティックの更新（全プロセッサ）
 */
void
ttsp_target_gain_tick(void)
{
	uint_t i;
	ID pid;

	/*
	 *  前回の更新が終わっていることを確認
	 *  自PEに対してはチェックする必要はないが，チェックしても問題ないため，
	 *  チェックする． 
	 */
	for(i = 0; i < TNUM_PRCID; i++) {
		while(target_timer_oneshot[i] == true);
	}

	for(i = 0; i < TNUM_PRCID; i++) {
		target_timer_oneshot[i] = true;
	}

	for(i = 0; i < TNUM_PRCID; i++) {
		target_timer_disable[i] = false;
	}

	/* 自PEに対しては，ハンドラが終了するまで待ち合わせる */
	sil_get_pid(&pid);
	while(target_timer_oneshot[pid-1] == true);
}

/*
 *  ティック用タイマ割込み要求の発生（自プロセッサ）
 */ 
void
ttsp_target_raise_tick_int(void)
{
	sil_wrw_mem((void *)AIC_ISET, (1 << IRQ_TC1));
}

/*
 *  高優先度割込みの発生   
 */
void
ttsp_highpri_int_raise(void)
{
	sil_wrw_mem((void *)AIC_ISET, (1 << TTSP_INTNO_HIGHPRI_INT));
}

/*
 *  中優先度割込みの発生   
 */
void
ttsp_midpri_int_raise(void)
{
	sil_wrw_mem((void *)AIC_ISET, (1 << TTSP_INTNO_MIDPRI_INT));
}

/*
 *  低優先度割込みの発生   
 */
void
ttsp_lowpri_int_raise(void)
{
	sil_wrw_mem((void *)AIC_ISET, (1 << TTSP_INTNO_LOWPRI_INT));
}

/*
 *  CPU例外の発生
 */
void
ttsp_cpuexc_raise(void)
{
	Asm("svc #0");
}
