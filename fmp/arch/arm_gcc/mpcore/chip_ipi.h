/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: chip_ipi.h 447 2009-10-16 08:51:47Z ertl-honda $
 */

/*
 *  �ץ��å��ֳ���ߥɥ饤�С�MPCORE�ѡ�
 */

#ifndef TOPPERS_CHIP_IPI_H
#define TOPPERS_CHIP_IPI_H

#include "mpcore.h"


#ifdef NONTRUST /* �Υ�ȥ饹��¦������ */
/*
 *  �ǥ����ѥå��׵��ѥץ��å��ֳ���ߤγ�����ֹ�
 */
#define INTNO_IPI_DIS_PRC1   (0x00010000 | DIC_IRQNO_IPI0)
#define INTNO_IPI_DIS_PRC2   (0x00020000 | DIC_IRQNO_IPI0)
#define INTNO_IPI_DIS_PRC3   (0x00030000 | DIC_IRQNO_IPI0)
#define INTNO_IPI_DIS_PRC4   (0x00040000 | DIC_IRQNO_IPI0)

/*
 *  �����ͥ뽪λ�ѤΥץ��å��ֳ���ߤγ�����ֹ�
 */
#define INTNO_IPI_EXT_PRC1   (0x00010000 | DIC_IRQNO_IPI1)
#define INTNO_IPI_EXT_PRC2   (0x00020000 | DIC_IRQNO_IPI1)
#define INTNO_IPI_EXT_PRC3   (0x00030000 | DIC_IRQNO_IPI1)
#define INTNO_IPI_EXT_PRC4   (0x00040000 | DIC_IRQNO_IPI1)

/*
 *  �ǥ����ѥå��׵��ѤΥץ��å��ֳ���ߤγ���ߥϥ�ɥ��ֹ�
 */
#define INHNO_IPI_DIS_PRC1   (0x00010000 | DIC_IRQNO_IPI0)
#define INHNO_IPI_DIS_PRC2   (0x00020000 | DIC_IRQNO_IPI0)
#define INHNO_IPI_DIS_PRC3   (0x00030000 | DIC_IRQNO_IPI0)
#define INHNO_IPI_DIS_PRC4   (0x00040000 | DIC_IRQNO_IPI0)

/*
 *  �����ͥ뽪λ�ѤΥץ��å��ֳ���ߤγ���ߥϥ�ɥ��ֹ�
 */
#define INHNO_IPI_EXT_PRC1   (0x00010000 | DIC_IRQNO_IPI1)
#define INHNO_IPI_EXT_PRC2   (0x00020000 | DIC_IRQNO_IPI1)
#define INHNO_IPI_EXT_PRC3   (0x00030000 | DIC_IRQNO_IPI1)
#define INHNO_IPI_EXT_PRC4   (0x00040000 | DIC_IRQNO_IPI1)



#else /* �ȥ饹��¦������ */

/*
 *  �ǥ����ѥå��׵��ѥץ��å��ֳ���ߤγ�����ֹ�
 */
#define INTNO_IPI_DIS_PRC1   (0x00010000 | DIC_IRQNO_IPI2)
#define INTNO_IPI_DIS_PRC2   (0x00020000 | DIC_IRQNO_IPI2)
#define INTNO_IPI_DIS_PRC3   (0x00030000 | DIC_IRQNO_IPI2)
#define INTNO_IPI_DIS_PRC4   (0x00040000 | DIC_IRQNO_IPI2)

/*
 *  �����ͥ뽪λ�ѤΥץ��å��ֳ���ߤγ�����ֹ�
 */
#define INTNO_IPI_EXT_PRC1   (0x00010000 | DIC_IRQNO_IPI3)
#define INTNO_IPI_EXT_PRC2   (0x00020000 | DIC_IRQNO_IPI3)
#define INTNO_IPI_EXT_PRC3   (0x00030000 | DIC_IRQNO_IPI3)
#define INTNO_IPI_EXT_PRC4   (0x00040000 | DIC_IRQNO_IPI3)

/*
 *  �ǥ����ѥå��׵��ѤΥץ��å��ֳ���ߤγ���ߥϥ�ɥ��ֹ�
 */
#define INHNO_IPI_DIS_PRC1   (0x00010000 | DIC_IRQNO_IPI2)
#define INHNO_IPI_DIS_PRC2   (0x00020000 | DIC_IRQNO_IPI2)
#define INHNO_IPI_DIS_PRC3   (0x00030000 | DIC_IRQNO_IPI2)
#define INHNO_IPI_DIS_PRC4   (0x00040000 | DIC_IRQNO_IPI2)

/*
 *  �����ͥ뽪λ�ѤΥץ��å��ֳ���ߤγ���ߥϥ�ɥ��ֹ�
 */
#define INHNO_IPI_EXT_PRC1   (0x00010000 | DIC_IRQNO_IPI3)
#define INHNO_IPI_EXT_PRC2   (0x00020000 | DIC_IRQNO_IPI3)
#define INHNO_IPI_EXT_PRC3   (0x00030000 | DIC_IRQNO_IPI3)
#define INHNO_IPI_EXT_PRC4   (0x00040000 | DIC_IRQNO_IPI3)

#endif /* NONTRUST */




/*
 *  �ǥ����ѥå��ѥץ��å��ֳ���ߤ�ȯ��
 */
Inline void
target_ipi_raise(uint_t prcid)
{
#ifdef NONTRUST
	dic_swi((1<<(prcid-1)), DIC_IRQNO_IPI0);
#else
	dic_swi((1<<(prcid-1)), DIC_IRQNO_IPI2);
#endif
}

/*
 *  �ǥ����ѥå��ѥץ��å��ֳ���ߤΥ��ꥢ
 */
Inline void
target_ipi_clear(void)
{

}

#ifdef USE_IPI_DIS_HANDER_BYPASS
/*
 *  �����ͥ뽪λ�����ѤΥץ��å��ֳ���ߥϥ�ɥ�
 */
extern void _kernel_ipi_ext_handler(void);
#endif /* USE_IPI_DIS_HANDER_BYPASS */

#endif /* TOPPERS_CHIP_IPI_H */
