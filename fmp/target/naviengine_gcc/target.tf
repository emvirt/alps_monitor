$ 
$     パス2のターゲット依存テンプレート（NaviEngine用）
$ 

$ 
$  有効な割込み番号，割込みハンドラ番号，CPU例外ハンドラ番号
$ 
$INTNO_VALID = { 
       0x10000,0x10001,...,0x1001f;
       0x20000,0x20001,...,0x2001f;
       0x30000,0x30001,...,0x3001f;
       0x40000,0x40001,...,0x4001f;
       0x00020,0x00021,...,0x0007f
}$
$INHNO_VALID = INTNO_VALID$

$ 
$ プロセッサに対する定義
$ 


$ 
$  クラスに対する定義
$ 

$ 
$  クラス数
$ 
$NUM_CLASS = 5$

$ 
$ クラス1に関する指定
$ 

$  初期割付けプロセッサ(ID指定)
$CLASS_AFFINITY_INI[1] = 1$

$  マイグレーション可能なプロセッサのマスク指定
$CLASS_AFFINITY_MASK[1] = 0x0000000f$

$  オブジェクトロック
& P_LOCKの場合に使用するオブジェクトロックを持つプロセッサのIDを指定
$CLASS_OBJ_LOCK[1] = 1$

$ 
$ クラス2に関する指定
$ 
$CLASS_AFFINITY_INI[2] = 2$
$CLASS_AFFINITY_MASK[2] = 0x0000000f$
$CLASS_OBJ_LOCK[2] = 2$

$ 
$ クラス3に関する指定
$ 
$CLASS_AFFINITY_INI[3] = 3$
$CLASS_AFFINITY_MASK[3] = 0x0000000f$
$CLASS_OBJ_LOCK[3] = 3$

$ 
$ クラス4に関する指定
$ 
$CLASS_AFFINITY_INI[4] = 4$
$CLASS_AFFINITY_MASK[4] = 0x0000000f$
$CLASS_OBJ_LOCK[4] = 4$

$ 
$ クラス TCL_ADMIN_PRC（クラスID 5） に関する指定
$ 
$CLASS_AFFINITY_INI[5] = 1$
$CLASS_AFFINITY_MASK[5] = 0x00000001$
$CLASS_OBJ_LOCK[5] = 1$

$ 
$  チップ依存テンプレートのインクルード
$ 
$INCLUDE"../../arch/arm_gcc/mpcore/arm11.tf"$
