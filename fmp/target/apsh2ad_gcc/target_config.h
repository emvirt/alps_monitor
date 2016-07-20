/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2007-2010 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id$
 */

/*
 *  �������åȰ�¸�⥸�塼���APSH2AD�ѡ�
 *
 *  �����ͥ�Υ������åȰ�¸���Υ��󥯥롼�ɥե����롥kernel_impl.h�Υ���
 *  ���åȰ�¸���ΰ����դ��Ȥʤ롥
 */

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H

/*
 *  �������åȥ����ƥ�Υϡ��ɥ������񸻤����
 */
#include "apsh2ad.h"

/*
 *  �ȥ졼�����˴ؤ�������
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"

/*
 *  ���Ϥ�������̤����
 *  ��ɬ�פ˱�����asp/arch/logtrace/trace_dump.c��trace_print()��
 *  ���б���������̤��ɵ����롣
 */
#define LOG_INH_ENTER			/*   17(0x11)  */
#define LOG_INH_LEAVE			/*  145(0x91)  */
#define LOG_EXC_ENTER			/*   22(0x16)  */
#define LOG_EXC_LEAVE			/*  150(0x96)  */

#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  �������åȰ�¸��ʸ�����Ϥ˻��Ѥ���ݡ��Ȥ����
 *  (�����ƥ����ʣ���Υץ��å��Ƕ�ͭ�������
 */
#define TARGET_PUTC_PORTID		1

/*
 *  FMP�����ͥ�ư����Υ���ޥåפȴ�Ϣ�������
 */

/*
 *  �ǥե���Ȥ��󥿥�������ƥ������ѤΥ����å��ΰ�����
 */
#ifdef ROM_BOOT				/*  ROM��  */
#define DEFAULT_PRC1_ISTKSZ      (1024 * 8)   /* 8KB */
#define DEFAULT_PRC2_ISTKSZ      (1024 * 8)   /* 8KB */
#else	/*  ROM_BOOT  */	/*  �ǥХå���  */
#define DEFAULT_PRC1_ISTKSZ      (1024 * 8)   /* 8KB */
#define DEFAULT_PRC2_ISTKSZ      (1024 * 8)   /* 8KB */
#endif /*  ROM_BOOT  */

/*	�����å��ΰ���� */
#define DEFAULT_PRC1_ISTK_BOTTOM		\
		(INNER_RAM0_PAGE3_ADDRESS_START + INNER_RAM_PAGE_SIZE)
#define DEFAULT_PRC2_ISTK_BOTTOM		\
		(INNER_RAM1_PAGE1_ADDRESS_START + INNER_RAM_PAGE_SIZE)

/*  �����å��ΰ����Ƭ���ϡʥ����å��ΰ����ǤϤʤ��Τǡ���ա� */
#define DEFAULT_PRC1_ISTK      			\
		(void *)(DEFAULT_PRC1_ISTK_BOTTOM - DEFAULT_PRC1_ISTKSZ)
#define DEFAULT_PRC2_ISTK      			\
		(void *)(DEFAULT_PRC2_ISTK_BOTTOM - DEFAULT_PRC2_ISTKSZ)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  str_ker() �����ǥޥ����ץ��å��ǹԤ������
 */
extern void target_mprc_initialize(void);

/*
 *  �������åȥ����ƥ��¸�ν����
 */
extern void target_initialize(void);

/*
 *  �������åȥ����ƥ�ν�λ
 *
 *  �����ƥ��λ������˻Ȥ���
 */
extern void target_exit(void) NoReturn;

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  ���������Ԥ��Τ��������������SIL�Υ������åȰ�¸����
 *  �����׸�Ƥ���ͤ�Ĵ�����Ƥ��ʤ�
 */
#ifdef ROM_BOOT		/*  ROM���ξ��  */
#define SIL_DLY_TIM1     75
#define SIL_DLY_TIM2     21

#else				/*  RAM��ǥǥХå�������  */
#define SIL_DLY_TIM1     1670
#define SIL_DLY_TIM2      906

#endif

/*
 *  �ץ��å���¸�⥸�塼���SH7205�ѡ�
 */
#include "sh2a_dual_gcc/sh7205_config.h"

#endif  /* TOPPERS_TARGET_CONFIG_H */
