$ 
$     パス2のアーキテクチャ依存テンプレート（Nios2用）
$ 

$ 
$  DEF_EXCで使用できるCPU例外ハンドラ番号
$ 
$EXCNO_DEFEXC_VALID = EXCNO_VALID$

$ 
$  ATT_ISRで使用できる割込み番号とそれに対応する割込みハンドラ番号
$ 
$INTNO_ATTISR_VALID = INTNO_VALID$
$INHNO_ATTISR_VALID = INHNO_VALID$

$ 
$  DEF_INTで使用できる割込みハンドラ番号
$ 
$INHNO_DEFINH_VALID = INHNO_VALID$

$ 
$  チェック方法の指定
$ 
$CHECK_STKSZ_ALIGN = 4$

$ 
$  CFG_INTで使用できる割込み番号と割込み優先度
$ 
$INTNO_CFGINT_VALID  = INTNO_VALID$
$INTPRI_CFGINT_VALID = { -1,-2,...,-7 }$

$ 
$  標準テンプレートファイルのインクルード
$ 
$INCLUDE "kernel/kernel.tf"$

/*$NL$
$SPC$*  Target Control Block$NL$
$SPC$*/$NL$
$NL$

$ 
$  例外ハンドラテーブル
$ 
$FOREACH prcid RANGE(1, TNUM_PRCID)$

const FP _kernel_prc$prcid$_exch_table[TNUM_EXCH] = {$NL$
$JOINEACH excno {0,1,...,17} ",\n"$ 
    $number = excno|(prcid<<16)$
	$IF LENGTH(EXC.EXCNO[number])$
		$TAB$(FP)($EXC.EXCHDR[number]$)
	$ELSE$
		$TAB$(FP)(_kernel_default_exc_handler)
	$END$
	$SPC$$FORMAT("/* %d */", +excno)$
$END$
$NL$};$NL$
$NL$

$END$

$ 
$  例外ハンドラアクセステーブル
$ 
const FP* const _kernel_p_exch_table[TNUM_PRCID] = {$NL$
$JOINEACH prcid RANGE(1, TNUM_PRCID) ",\n"$
	$TAB$_kernel_prc$prcid$_exch_table
$END$
$NL$};$NL$
$NL$

$ 
$  割込み優先度テーブル（内部表現）
$ 
$NL$

$FOREACH prcid RANGE(1, TNUM_PRCID)$

const uint8_t _kernel_prc$prcid$_inh_iipm_table[TNUM_INH] = {$NL$
$FOREACH inhno INHNO_VALID$
	$IF (inhno & 0xffff0000) == (prcid << 16) $
		$IF LENGTH(INH.INHNO[inhno]) && (CLASS_AFFINITY_INI[INH.CLASS[inhno]] == prcid)$
		  $TAB$UINT8_C($-INT.INTPRI[inhno]$),
		$ELSE$
		  $TAB$UINT8_C(0),
		$END$
		$SPC$$FORMAT("/* 0x%x */", +inhno)$$NL$
	$END$
$END$
};$NL$
$NL$

$END$

$ 
$  割込み優先度テーブルアクセステーブル
$ 
const uint8_t* const _kernel_p_inh_iipm_table[TNUM_PRCID] = {$NL$
$JOINEACH prcid RANGE(1, TNUM_PRCID) ",\n"$
	$TAB$_kernel_prc$prcid$_inh_iipm_table
$END$
$NL$};$NL$
$NL$
$NL$

$ 
$  割込みマスクテーブル
$ 
$FOREACH prcid RANGE(1, TNUM_PRCID)$

const uint32_t _kernel_prc$prcid$_iipm_mask_table[8]={$NL$
$FOREACH intpri { 0,-1,...,-7 }$
	$intmask = 0$
	$FOREACH intno (INT.ID_LIST)$
		$IF CLASS_AFFINITY_INI[INT.CLASS[intno]] == prcid $
			$IF INT.INTPRI[intno] >= intpri $
				$intmask = intmask | (1 << (INT.INTNO[intno] & 0x0000FFFF))$
			$END$
		$END$
	$END$
 $TAB$UINT32_C($FORMAT("0x%08x", intmask)$),/* Priority $+intpri$ */$NL$
$END$
};$NL$
$NL$
$END$

$ 
$  割込みマスクテーブルアクセステーブル
$ 
const uint32_t* const _kernel_p_iipm_mask_table[TNUM_PRCID] = {$NL$
$JOINEACH prcid RANGE(1, TNUM_PRCID) ",\n"$
	$TAB$_kernel_prc$prcid$_iipm_mask_table
$END$
$NL$};$NL$
$NL$

$ 
$  割込みハンドラテーブル
$ 
$NL$
$FOREACH prcid RANGE(1, TNUM_PRCID)$

const FP _kernel_prc$prcid$_inh_table[TNUM_INH] = {$NL$
$FOREACH inhno INHNO_VALID$
	$IF (inhno & 0xffff0000) == (prcid << 16) $
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
$END$

$ 
$  割込みハンドラテーブルアクセステーブル
$ 
const FP* const _kernel_p_inh_table[TNUM_PRCID] = {$NL$
$JOINEACH prcid RANGE(1, TNUM_PRCID) ",\n"$
	$TAB$_kernel_prc$prcid$_inh_table
$END$
$NL$};$NL$
$NL$
$NL$
