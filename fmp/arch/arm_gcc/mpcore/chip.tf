$ 
$     �ѥ�2�Υ��åװ�¸�ƥ�ץ졼�ȡ�MPCORE�ѡ�
$ 

$ 
$ ͭ����CPU�㳰�ϥ�ɥ��ֹ�
$ 
$EXCNO_VALID = { 
	0x10001,0x10002,0x10003,0x10004,0x10006;
	0x20001,0x20002,0x20003,0x20004,0x20006;
	0x30001,0x30002,0x30003,0x30004,0x30006;
	0x40001,0x40002,0x40003,0x40004,0x40006
}$

$ 
$  ATT_ISR�ǻ��ѤǤ��������ֹ�Ȥ�����б��������ߥϥ�ɥ��ֹ�
$ 
$INTNO_ATTISR_VALID = INTNO_VALID$
$INHNO_ATTISR_VALID = INHNO_VALID$

$ 
$  DEF_INT�ǻ��ѤǤ������ߥϥ�ɥ��ֹ�
$ 
$INHNO_DEFINH_VALID = INHNO_VALID$

$ 
$  CFG_INT�ǻ��ѤǤ��������ֹ�ȳ����ͥ����
$ 
$INTNO_CFGINT_VALID  = INTNO_VALID$

$ 
$  ARM��¸�ƥ�ץ졼�ȤΥ��󥯥롼��
$ 
$INCLUDE"../common/core.tf"$

$ 
$  ����ߥϥ�ɥ�ơ��֥�
$ 
$FOREACH prcid RANGE(1, TNUM_PRCID)$

const FP _kernel_prc$prcid$_inh_table[TNUM_INH] = {$NL$
$FOREACH inhno INHNO_VALID$
	$IF ((inhno & 0xffff0000) == (prcid << 16)) ||  ((inhno & 0xffff0000) == 0)$
		$IF LENGTH(INH.INHNO[inhno]) && (CLASS_AFFINITY_INI[INH.CLASS[inhno]] == prcid)$
			$TAB$(FP)($INH.INTHDR[inhno]$),
		$ELSE$
			$TAB$(FP)(_kernel_default_int_handler),
		$END$
		$SPC$$FORMAT("/* 0x%x */", +inhno)$$NL$
	$END$
$END$
};$NL$
$NL$

$END$$NL$

$ 
$  ����ߥϥ�ɥ饢�������ơ��֥�
$ 
const FP* const _kernel_p_inh_table[TNUM_PRCID] = {$NL$
$JOINEACH prcid RANGE(1, TNUM_PRCID) ",\n"$
	$TAB$_kernel_prc$prcid$_inh_table
$END$
$NL$};$NL$
$NL$

$ 
$  �����°���ơ��֥�
$ 
$NL$
$FOREACH prcid RANGE(1, TNUM_PRCID)$

const uint8_t _kernel_prc$prcid$_cfgint_table[TNUM_INH] = {$NL$
$FOREACH inhno INHNO_VALID$
	$IF ((inhno & 0xffff0000) == (prcid << 16)) ||  ((inhno & 0xffff0000) == 0)$
		$IF LENGTH(INH.INHNO[inhno]) && (CLASS_AFFINITY_INI[INH.CLASS[inhno]] == prcid)$
			$TAB$1U,
		$ELSE$
			$TAB$0U,
		$END$
		$SPC$$FORMAT("/* 0x%x */", +inhno)$$NL$
	$END$
$END$
};$NL$
$NL$

$END$$NL$

$ 
$  �����°�����������ơ��֥�
$ 
const uint8_t* const _kernel_p_cfgint_table[TNUM_PRCID] = {$NL$
$JOINEACH prcid RANGE(1, TNUM_PRCID) ",\n"$
	$TAB$_kernel_prc$prcid$_cfgint_table
$END$
$NL$};$NL$
$NL$
