$ 
$     �ѥ�2�Υ������åȰ�¸�ƥ�ץ졼�ȡ�NaviEngine�ѡ�
$ 

$ 
$  ͭ���ʳ�����ֹ桤����ߥϥ�ɥ��ֹ桤CPU�㳰�ϥ�ɥ��ֹ�
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
$ �ץ��å����Ф������
$ 


$ 
$  ���饹���Ф������
$ 

$ 
$  ���饹��
$ 
$NUM_CLASS = 5$

$ 
$ ���饹1�˴ؤ������
$ 

$  ������դ��ץ��å�(ID����)
$CLASS_AFFINITY_INI[1] = 1$

$  �ޥ����졼������ǽ�ʥץ��å��Υޥ�������
$CLASS_AFFINITY_MASK[1] = 0x0000000f$

$  ���֥������ȥ�å�
& P_LOCK�ξ��˻��Ѥ��륪�֥������ȥ�å�����ĥץ��å���ID�����
$CLASS_OBJ_LOCK[1] = 1$

$ 
$ ���饹2�˴ؤ������
$ 
$CLASS_AFFINITY_INI[2] = 2$
$CLASS_AFFINITY_MASK[2] = 0x0000000f$
$CLASS_OBJ_LOCK[2] = 2$

$ 
$ ���饹3�˴ؤ������
$ 
$CLASS_AFFINITY_INI[3] = 3$
$CLASS_AFFINITY_MASK[3] = 0x0000000f$
$CLASS_OBJ_LOCK[3] = 3$

$ 
$ ���饹4�˴ؤ������
$ 
$CLASS_AFFINITY_INI[4] = 4$
$CLASS_AFFINITY_MASK[4] = 0x0000000f$
$CLASS_OBJ_LOCK[4] = 4$

$ 
$ ���饹 TCL_ADMIN_PRC�ʥ��饹ID 5�� �˴ؤ������
$ 
$CLASS_AFFINITY_INI[5] = 1$
$CLASS_AFFINITY_MASK[5] = 0x00000001$
$CLASS_OBJ_LOCK[5] = 1$

$ 
$  ���åװ�¸�ƥ�ץ졼�ȤΥ��󥯥롼��
$ 
$INCLUDE"../../arch/arm_gcc/mpcore/arm11.tf"$
