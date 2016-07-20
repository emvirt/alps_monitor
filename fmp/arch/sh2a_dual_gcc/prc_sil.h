/*
 *	TOPPERS Software
 *		Toyohashi Open Platform for Embedded Real-Time Systems
 *	
 *	Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *				Graduate School of Information Science, Nagoya Univ., JAPAN
 *	Copyright (C) 2007-2009 by Industrial Technology Institute,
 *								Miyagi Prefectural Government, JAPAN
 *	
 *	�嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *	�����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *	�ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *	(1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *		��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *		����������˴ޤޤ�Ƥ��뤳�ȡ�
 *	(2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *		�ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *		�ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *		��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *	(3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *		�ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *		�ȡ�
 *	  (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *		  �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *	  (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ������Ȥ�
 *		  ��𤹤뤳�ȡ�
 *	(4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *		������⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ����դ��뤳�ȡ�
 *		�ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *		ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ�
 *		���դ��뤳�ȡ�
 *	
 *	�ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *	���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *	���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *	�������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *	����Ǥ�����ʤ���
 *	
 *  @(#) $Id$
 */

/*
 *	sil.h�Υץ��å���¸����SH2A-DUAL�ѡ�
 */

#ifndef TOPPERS_PRC_SIL_H
#define TOPPERS_PRC_SIL_H

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ����ǥ������ȿž
 */
Inline uint16_t toppers_sil_rev_endian_uint16(uint16_t src)
{
	uint16_t dst;
	Asm("swap.b %1, %0" : "=r"(dst) : "r"(src) );
	return dst;
}

#define	TOPPERS_SIL_REV_ENDIAN_UINT16(data) \
		toppers_sil_rev_endian_uint16(data)

Inline uint32_t toppers_sil_rev_endian_uint32(uint32_t src)
{
	uint32_t dst;
	Asm("swap.b %1, %0 \n"		/*  0x1234��0x1243  */
		"swap.w %0, %0 \n" 		/*  0x1243��0x4312  */
		"swap.b %0, %0"			/*  0x4312��0x4321  */
		: "=r"(dst) : "r"(src) );
	return dst;
}

#define	TOPPERS_SIL_REV_ENDIAN_UINT32(data) \
		toppers_sil_rev_endian_uint32(data)

/*
 *	�쥸���������������ؿ�
 *	�����������������η������å��Τ��ᡢ�������Ȥ��̤�����򤷤Ƥ��롣
 */

/*
 *	8�ӥåȡ��쥸�������ɽФ��������
 */
Inline uint8_t
sil_reb_reg(uint8_t *reg)
{
	uint8_t	data;

	data = *((volatile uint8_t *) reg);
	return(data);
}

Inline void
sil_wrb_reg(uint8_t *reg, uint8_t data)
{
	*((volatile uint8_t *) reg) = data;
}

/*
 *	16�ӥåȡ��쥸�������ɽФ��������
 */
Inline uint16_t
sil_reh_reg(uint16_t *reg)
{
	uint16_t	data;

	data = *((volatile uint16_t *) reg);
	return(data);
}

Inline void
sil_wrh_reg(uint16_t *reg, uint16_t data)
{
	*((volatile uint16_t *) reg) = data;
}

/*
 *	32�ӥåȡ��쥸�������ɽФ��������
 */
Inline uint32_t
sil_rew_reg(uint32_t *reg)
{
	uint32_t	data;

	data = *((volatile uint32_t *) reg);
	return(data);
}

Inline void
sil_wrw_reg(uint32_t *reg, uint32_t data)
{
	*((volatile uint32_t *) reg) = data;
}


/*
 *	16�ӥåȡ��쥸�����������黻
 */
Inline void
sil_orh_reg(uint16_t *reg, uint32_t bitptn)
{
	uint32_t reg_val = sil_reh_reg(reg);
	
	reg_val |= bitptn;
	sil_wrh_reg(reg, reg_val);
}

Inline void
sil_anh_reg(uint16_t *reg, uint32_t bitptn)
{
	uint32_t reg_val = sil_reh_reg(reg);
	
	reg_val &= bitptn;
	sil_wrh_reg(reg, reg_val);
}


/*
 *	16�ӥåȡ��쥸�����Υӥåȡ����åȡ����ꥢ
 */
Inline void
sil_seth_bit(uint16_t *reg, uint_t bit)
{
	uint32_t reg_val = sil_reh_reg(reg);
	
	/*
	 *	bit��7�ʲ�������Ǥ���С�
	 *	bset̿���Ÿ������뤳�Ȥ����
	 */
	reg_val |= (1U << bit);
	sil_wrh_reg(reg, reg_val);
}

Inline void
sil_clrh_bit(uint16_t *reg, uint_t bit)
{
	uint32_t reg_val = sil_reh_reg(reg);
	
	/*
	 *	bit��7�ʲ�������Ǥ���С�
	 *	bclr̿���Ÿ������뤳�Ȥ����
	 */
	reg_val &= ~(1U << bit);
	sil_wrh_reg(reg, reg_val);
}


/*
 *	NMI��������٤Ƥγ���ߤζػ�
 */
Inline uint32_t
TOPPERS_disint(void)
{
	uint32_t  TOPPERS_sr;

	Asm("stc  sr,%0" : "=r"(TOPPERS_sr));
	Asm("ldc %0, sr" : : "r"(TOPPERS_sr | 0x000000f0U) : "memory", "t");
	return(TOPPERS_sr & 0x000000f0U);
}

/*
 *	�����ͥ���٥ޥ���������ɽ���ˤθ����ͤ�����
 */
Inline void
TOPPERS_set_iipm(uint32_t TOPPERS_iipm)
{
	uint32_t  TOPPERS_sr;

	Asm("stc  sr,%0" : "=r"(TOPPERS_sr));
	TOPPERS_sr = (TOPPERS_sr & ~0x000000f0U) | TOPPERS_iipm;
	Asm("ldc %0, sr" : : "r"(TOPPERS_sr) : "memory", "t");
}


/*
 *	������ߥ�å����֤�����
 *	
 *	����ǥޥ����졼����󤷡����Υץ��å��Ȱۤʤ�ץ��å���
 *	SR�쥸�����ؤν񤭹��ߤ���������ιͻ�
 *	
 *	�ޥ����졼�����ȯ������Τϡ��ޥ����졼����󸵥ץ��å���
 *	��ץ��å�����
 *	������������ߥ�å�������֡�����
 *	�����������ͥ���٥ޥ������������
 *	�ξ��˸¤���Τǡ��ޥ����졼�������Ǽ¹ԺƳ������ݤˤϡ�
 *	�嵭�ξ��֤ˤʤäƤ��ꡢSR�쥸�������񤭤��Ƥ�����ʤ���
 */
#define SIL_PRE_LOC 	 uint32_t TOPPERS_saved_iipm
#define SIL_LOC_INT()	 ((void)(TOPPERS_saved_iipm = TOPPERS_disint()))
#define SIL_UNL_INT()	 (TOPPERS_set_iipm(TOPPERS_saved_iipm))

/*
 *	���ԥ��å��μ���
 */
Inline uint32_t
TOPPERS_sil_loc_spn(void)
{
	uint32_t saved_iipm;
	uint8_t semr0;
	uint8_t *p_semr0 = (uint8_t *)SEMR0;

  retry:
	/* ������ߥ�å����֤�*/
	saved_iipm = TOPPERS_disint();
	
	semr0 = sil_reb_reg(p_semr0);
		/*
		 *	SEMR0 == 0 ��������
		 *	SEMR0 == 1 ������
		 */
	if (semr0 == 0) {
		/* ���ԥ��å��������Ǥ��ʤ��ä���� */
		/* ������ߥ�å����֤����ξ��֤� */
		TOPPERS_set_iipm(saved_iipm);
		goto retry;
	}

	/* ��������Ƥ��񤭴�����ǽ�������� */
	Asm("":::"memory");
	return saved_iipm;
}

/*
 *	���ԥ��å��β���
 */
Inline void
TOPPERS_sil_unl_spn(uint32_t saved_iipm)
{
	/* ��������Ƥ��񤭴�����ǽ�������� */
	Asm("":::"memory");
	sil_wrb_reg((uint8_t *)SEMR0, SEMR_SEMF_BIT);

	/* ������ߥ�å����֤����ξ��֤� */
	TOPPERS_set_iipm(saved_iipm);
}

/*
 *	���ԥ��å�
 */
#define SIL_LOC_SPN() ((void)(TOPPERS_saved_iipm = TOPPERS_sil_loc_spn()))
#define SIL_UNL_SPN() (TOPPERS_sil_unl_spn(TOPPERS_saved_iipm))


/*
 *	�ץ��å�INDEX��0���ꥸ��ˤμ���
 *	�������
 *	����CPU0��0
 *	����CPU1��1
 */
Inline uint_t
sil_get_pindex(void)
{
	uint_t index;
	uint32_t cpuidr = sil_rew_reg((uint32_t *)CPUIDR);
	
	if ((cpuidr & CPUIDR_ID_BIT) == 0U) {
		index = 0U;
	} else {
		index = 1U;
	}
	return index;
}

/*
 *	�ץ��å�ID�μ���
 *	�������
 *	����CPU0��1
 *	����CPU1��2
 */
Inline void
sil_get_pid(ID *p_id)
{
	*p_id = sil_get_pindex() + 1U;
}

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_PRC_SIL_H */
