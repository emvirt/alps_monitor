$ 
$     パス2のターゲット依存テンプレート（APSH2AD用）
$ 

$ 
$ プロセッサに対する定義
$ 

$ グローバルタイマ時のタイムイベント用データのセクション指定
$PRC_SECTION_TEVT[0] = ""$

$ 
$ ネイティブロック方式の場合のロックの最大サポート数
$ 
$MAX_NATIVE_SPIN_LOCK = 32 - 2$
$ 
$ プロセッサ1に関する指定
$ 
$ PCBのセクション指定
$PRC_SECTION_PCB[1]  = ""$
$ 非タスクコンテキスト用スタックのセクション指定
$PRC_SECTION_ISTACK[1]  = ""$
$ タイムイベント関連データのセクション指定
$PRC_SECTION_TEVT[1] = ""$

$ ターゲット依存の定義
$PRC_SECTION_TPCB[1] = ""$


$ 
$ プロセッサ2に関する指定
$ 
$PRC_SECTION_PCB[2] = ""$
$PRC_SECTION_ISTACK[2]  = ""$
$PRC_SECTION_TEVT[2] = ""$
$PRC_SECTION_TPCB[2] = ""$



$ 
$  クラスに対する定義
$ 

$ 
$  クラス数
$ 
$NUM_CLASS = 3$

$ 
$ クラス1に関する指定
$ 

$  初期所属プロセッサ(ID指定)
$CLASS_AFFINITY_INI[1] = 1$

$  マイグレーション可能なプロセッサのマスク指定
$CLASS_AFFINITY_MASK[1] = 0x00000003$
$ テスト
$ $CLASS_AFFINITY_MASK[1] = 0x00000001$

$  タスクコントロールブロックのセクション指定
$CLASS_SECTION_TSKCB[1] = ""$

$  オブジェクトコントロールブロックのセクション指定
$CLASS_SECTION_OBJCB[1] = ""$

$  タスクスタックのセクション指定
$CLASS_SECTION_TSTACK[1] = ""$


$ 
$ クラス2に関する指定
$ 
$CLASS_AFFINITY_INI[2] = 2$
$CLASS_AFFINITY_MASK[2] = 0x00000003$
$ テスト
$ $CLASS_AFFINITY_MASK[2] = 0x00000002$

$CLASS_SECTION_TSKCB[2] = ""$
$CLASS_SECTION_OBJCB[2] = ""$
$CLASS_SECTION_TSTACK[2] = ""$

$ 
$ クラス TCL_ADMIN_PRC（クラスID 3） に関する指定
$ 
$CLASS_AFFINITY_INI[3] = 1$
$CLASS_AFFINITY_MASK[3] = 0x00000001$

$CLASS_SECTION_TSKCB[3] = ""$
$CLASS_SECTION_OBJCB[3] = ""$
$CLASS_SECTION_TSTACK[3] = ""$


$ 
$  SH7205依存のテンプレートファイルのインクルード
$ 
$INCLUDE "sh2a_dual_gcc/sh7205.tf"$
