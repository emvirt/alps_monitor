$ 
$		アーキテクチャ依存テンプレート（SH4A用）
$ 

$ 
$  ターゲット非依存部に含まれる標準の割込み管理機能の初期化処理を用いない
$ 
$OMIT_INITIALIZE_EXCEPTION = 1$

$  割込み番号，割り込みハンドラ番号のマスク
$ 
$INTNO_MASK = 0x0000FFFF$
$INHNO_MASK = INTNO_MASK$

$EXCNO_RANGE = { 0,0x20,...,0x820 }$

$  ATT_ISRで使用できる割込み番号とそれに対応する割込みハンドラ番号
$ 
$INTNO_ATTISR_VALID = INTNO_VALID$
$INHNO_ATTISR_VALID = INHNO_VALID$

$ 
$  DEF_INT／DEF_EXCで使用できる割込みハンドラ番号／CPU例外ハンドラ番号
$ 
$INHNO_DEFINH_VALID = INHNO_VALID$
$EXCNO_DEFEXC_VALID = EXCNO_VALID$

$ 
$  CFG_INTで使用できる割込み番号と割込み優先度
$ 
$INTNO_CFGINT_VALID = INTNO_VALID$
$INTPRI_CFGINT_VALID = { -1,-2,...,-15 }$

$ 
$  定数定義
$ 
$TA_POSEDGE = TA_EDGE$
$TA_NEGEDGE = 0x04$

$ 
$  割込み属性中のターゲット依存に用いるビット
$ 
$  TARGET_INTATR = TA_NEGEDGE$
$ 
$  ターゲット定義の割込み属性（プロセッサ番号）
$ 
$TARGET_INTATR = 0xff0000$



$ 
$  標準テンプレートファイルのインクルード
$ 
$INCLUDE "kernel/kernel.tf"$

/*$NL$
$SPC$*  Target-dependent Definitions (SH34)$NL$
$SPC$*/$NL$
$NL$


$ 
$  割込み優先度テーブル（内部表現）
$ 
const uint32_t _kernel_int_iipm_tbl[TNUM_INT] = {$NL$
$FOREACH intno INTNO_RANGE$
	$IF LENGTH(INT.INTNO[intno])$
		$intmask = 0x40000000 | (-INT.INTPRI[intno] << 4)$
	$ELSE$
$		// LSBを1にしているのは，割込み属性が設定されていないことを判
$		// 別するためである．
		$intmask = 0x400000F0 | 0x01 $
	$END$
	$TAB$$FORMAT("UINT32_C(0x%08x), /* 0x%03x */", intmask, +intno)$$NL$
$END$
$NL$};$NL$
$NL$

$ 
$  割込みハンドラテーブル
$ 
const FP _kernel_inh_tbl[TNUM_INH] = {$NL$
$FOREACH intno INTNO_RANGE$
	$IF LENGTH(INT.INTNO[intno])$
		$FOREACH inhno INHNO_RANGE$
			$IF (((inhno & 0x0000ffff) == intno) &&(LENGTH(INH.INHNO[inhno])))$
				$TAB$(FP)($INH.INTHDR[inhno]$),
			$END$
		$END$
	$ELSE$
		$TAB$(FP)(_kernel_default_int_handler),
	$END$
	$SPC$$FORMAT("/* 0x%03x */", +intno)$$NL$
$END$
$NL$};$NL$
$NL$

$ 
$  例外ハンドラテーブル
$ 
$FOREACH prcid RANGE(1, TNUM_PRCID)$

const FP _kernel_prc$prcid$_exch_table[] = {$NL$
$FOREACH excno EXCNO_RANGE$
    $number = excno|(prcid<<16)$
	$IF LENGTH(EXC.EXCNO[number])$
		$TAB$(FP)($EXC.EXCHDR[number]$),
	$ELSE$
		$TAB$(FP)(_kernel_default_exc_handler),
	$END$
	$SPC$$FORMAT("/* 0x%03x */", +excno)$$NL$
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