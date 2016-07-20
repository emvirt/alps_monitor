#include "kernel_impl.h"
#include <sil.h>
#include "target_test.h"

#define AIC_ISET (0xfffff12C)

/*
 *  �ƥ��å��ѥ����ޥǥ��������֥��ѿ�
 */
static volatile bool_t target_timer_disable[TNUM_PRCID];

/*
 *  �ƥ��å��ѥ����ޥ�󥷥�åȼ¹�
 */
static volatile bool_t target_timer_oneshot[TNUM_PRCID];

/*
 *  �����ޥϥ�ɥ鳫�ϻ��˸ƤӽФ��եå�
 *  (false��signal_time()��ƤӽФ��ʤ�)
 */
bool_t
ttsp_timer_handler_begin_hook(void){
	return !target_timer_disable[x_prc_index()];
}

/*
 *  �����ޥϥ�ɥ齪λ���˸ƤӽФ��եå�
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
 *  �ƥ��å����������(���ץ��å�)
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
 *  �ƥ��å����������(����ץ��å�)
 */
void
ttsp_target_stop_tick_pe(ID prcid)
{
	uint_t prc_index = prcid - 1;
	target_timer_disable[prc_index] = true;
}

/*
 *  �ƥ��å������κƳ������ץ��å���
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
 *  �ƥ��å������κƳ�������ץ��å���
 */
void
ttsp_target_start_tick_pe(ID prcid)
{
	uint_t prc_index = prcid - 1;
	target_timer_disable[prc_index] = false;
}

/*
 *  �ƥ��å��ι���������ץ��å���
 */
void
ttsp_target_gain_tick_pe(ID prcid)
{
	int prc_index = prcid - 1;
	ID pid;

	/*
	 *  ����ι���������äƤ��뤳�Ȥ��ǧ
	 *  ��PE���Ф��Ƥϥ����å�����ɬ�פϤʤ����������å����Ƥ�����ʤ����ᡤ
	 *  �����å����롥 
	 */
	while(target_timer_oneshot[prc_index] == true);

	target_timer_oneshot[prc_index] = true;
	target_timer_disable[prc_index] = false;

	/* ��PE���Ф��Ƥϡ��ϥ�ɥ餬��λ����ޤ��Ԥ���碌�� */
	sil_get_pid(&pid);
	if (pid == prcid) {
		while(target_timer_oneshot[prc_index] == true);
	}
}

/*
 *  �ƥ��å��ι��������ץ��å���
 */
void
ttsp_target_gain_tick(void)
{
	uint_t i;
	ID pid;

	/*
	 *  ����ι���������äƤ��뤳�Ȥ��ǧ
	 *  ��PE���Ф��Ƥϥ����å�����ɬ�פϤʤ����������å����Ƥ�����ʤ����ᡤ
	 *  �����å����롥 
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

	/* ��PE���Ф��Ƥϡ��ϥ�ɥ餬��λ����ޤ��Ԥ���碌�� */
	sil_get_pid(&pid);
	while(target_timer_oneshot[pid-1] == true);
}

/*
 *  �ƥ��å��ѥ����޳�����׵��ȯ���ʼ��ץ��å���
 */ 
void
ttsp_target_raise_tick_int(void)
{
	sil_wrw_mem((void *)AIC_ISET, (1 << IRQ_TC1));
}

/*
 *  ��ͥ���ٳ���ߤ�ȯ��   
 */
void
ttsp_highpri_int_raise(void)
{
	sil_wrw_mem((void *)AIC_ISET, (1 << TTSP_INTNO_HIGHPRI_INT));
}

/*
 *  ��ͥ���ٳ���ߤ�ȯ��   
 */
void
ttsp_midpri_int_raise(void)
{
	sil_wrw_mem((void *)AIC_ISET, (1 << TTSP_INTNO_MIDPRI_INT));
}

/*
 *  ��ͥ���ٳ���ߤ�ȯ��   
 */
void
ttsp_lowpri_int_raise(void)
{
	sil_wrw_mem((void *)AIC_ISET, (1 << TTSP_INTNO_LOWPRI_INT));
}

/*
 *  CPU�㳰��ȯ��
 */
void
ttsp_cpuexc_raise(void)
{
	Asm("svc #0");
}
