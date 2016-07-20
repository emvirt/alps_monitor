$ 
$     ターゲット依存テンプレート（CM922T用）
$ 

$ 
$  有効な割込み番号，割込みハンドラ番号，CPU例外ハンドラ番号
$ 
$INTNO_VALID = { 0,1,...,63 }$
$INHNO_VALID = INTNO_VALID$

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
$  CFG_INTで使用できる割込み番号と割込み優先度
$ 
$INTNO_CFGINT_VALID  = INTNO_VALID$
$INTPRI_CFGINT_VALID = { -1,-2,...,-31 }$

$ 
$  アーキテクチャ依存テンプレートのインクルード
$ 
$INCLUDE"../../arch/arm_gcc/common/core.tf"$

$ 
$  割込みハンドラテーブル
$ 
$NL$
const FP _kernel_inh_tbl[TNUM_INH] = {$NL$
$FOREACH inhno INHNO_VALID$ 
	$IF LENGTH(INH.INHNO[inhno])$
		$TAB$(FP)($INH.INTHDR[inhno]$),
	$ELSE$
		$TAB$(FP)(_kernel_default_int_handler),
	$END$
	$SPC$$FORMAT("/* %d */", +inhno)$$NL$
$END$
$NL$};$NL$
$NL$


$ 
$  割込み属性テーブル
$ 
$NL$
const uint8_t _kernel_cfgint_tbl[TNUM_INH] = {$NL$
$FOREACH inhno INHNO_VALID$ 
	$IF LENGTH(INH.INHNO[inhno])$
		$TAB$1U,
	$ELSE$
		$TAB$0U,
	$END$
	$SPC$$FORMAT("/* 0x%03x */", inhno)$$NL$
$END$
$NL$};$NL$
$NL$
