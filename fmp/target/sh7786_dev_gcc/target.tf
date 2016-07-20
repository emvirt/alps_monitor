$ 
$     ターゲット依存テンプレート（SH7786_DEV用）
$ 

$ 
$ プロセッサに対する定義
$ 

$ グローバルタイマ時のタイムイベント用データのセクション指定
$PRC_SECTION_TEVT[0] = ""$

$ 
$ ネイティブロック方式の場合のロックの最大サポート数
$ 
$MAX_NATIVE_SPIN_LOCK = 4$
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
$ プロセッサ3に関する指定
$ 
$PRC_SECTION_PCB[3] = ""$
$PRC_SECTION_ISTACK[3]  = ""$
$PRC_SECTION_TEVT[3] = ""$
$PRC_SECTION_TPCB[3] = ""$

$ 
$ プロセッサ4に関する指定
$ 
$PRC_SECTION_PCB[4] = ""$
$PRC_SECTION_ISTACK[4]  = ""$
$PRC_SECTION_TEVT[4] = ""$
$PRC_SECTION_TPCB[4] = ""$


$ 
$  クラスに対する定義
$ 

$ 
$  クラス数
$ 
$NUM_CLASS = 4$

$ 
$ クラス1に関する指定
$ 

$  初期所属プロセッサ(ID指定)
$CLASS_AFFINITY_INI[1] = 1$

$  マイグレーション可能なプロセッサのマスク指定
$CLASS_AFFINITY_MASK[1] = 0x0000000F$

$  タスクコントロールブロックのセクション指定
$CLASS_SECTION_TSKCB[1] = ""$

$  オブジェクトコントロールブロックのセクション指定
$CLASS_SECTION_OBJCB[1] = ""$

$  タスクスタックのセクション指定
$CLASS_SECTION_TSTACK[1] = ""$

$  オブジェクトロック
& P_LOCKの場合に使用するオブジェクトロックを持つプロセッサのIDを指定
$CLASS_OBJ_LOCK[1] = 1$

$ 
$ クラス2に関する指定
$ 
$CLASS_AFFINITY_INI[2] = 2$
$CLASS_AFFINITY_MASK[2] = 0x0000000F$
$CLASS_SECTION_TSKCB[2] = ""$
$CLASS_SECTION_OBJCB[2] = ""$
$CLASS_SECTION_TSTACK[2] = ""$
$CLASS_OBJ_LOCK[2] = 2$

$ 
$ クラス3に関する指定
$ 
$CLASS_AFFINITY_INI[3] = 3$
$CLASS_AFFINITY_MASK[3] = 0x0000000F$
$CLASS_SECTION_TSKCB[3] = ""$
$CLASS_SECTION_OBJCB[3] = ""$
$CLASS_SECTION_TSTACK[3] = ""$
$CLASS_OBJ_LOCK[3] = 3$

$ 
$ クラス4に関する指定
$ 
$CLASS_AFFINITY_INI[4] = 4$
$CLASS_AFFINITY_MASK[4] = 0x0000000F$
$CLASS_SECTION_TSKCB[4] = ""$
$CLASS_SECTION_OBJCB[4] = ""$
$CLASS_SECTION_TSTACK[4] = ""$
$CLASS_OBJ_LOCK[4] = 4$

$ 
$  SH7786依存のテンプレートファイルのインクルード
$ 

$INCLUDE"sh4a_gcc/sh7786.tf"$
