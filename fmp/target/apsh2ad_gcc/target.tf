$ 
$     �ѥ�2�Υ������åȰ�¸�ƥ�ץ졼�ȡ�APSH2AD�ѡ�
$ 

$ 
$ �ץ��å����Ф������
$ 

$ �����Х륿���޻��Υ����।�٥���ѥǡ����Υ�����������
$PRC_SECTION_TEVT[0] = ""$

$ 
$ �ͥ��ƥ��֥�å������ξ��Υ�å��κ��祵�ݡ��ȿ�
$ 
$MAX_NATIVE_SPIN_LOCK = 32 - 2$
$ 
$ �ץ��å�1�˴ؤ������
$ 
$ PCB�Υ�����������
$PRC_SECTION_PCB[1]  = ""$
$ �󥿥�������ƥ������ѥ����å��Υ�����������
$PRC_SECTION_ISTACK[1]  = ""$
$ �����।�٥�ȴ�Ϣ�ǡ����Υ�����������
$PRC_SECTION_TEVT[1] = ""$

$ �������åȰ�¸�����
$PRC_SECTION_TPCB[1] = ""$


$ 
$ �ץ��å�2�˴ؤ������
$ 
$PRC_SECTION_PCB[2] = ""$
$PRC_SECTION_ISTACK[2]  = ""$
$PRC_SECTION_TEVT[2] = ""$
$PRC_SECTION_TPCB[2] = ""$



$ 
$  ���饹���Ф������
$ 

$ 
$  ���饹��
$ 
$NUM_CLASS = 3$

$ 
$ ���饹1�˴ؤ������
$ 

$  �����°�ץ��å�(ID����)
$CLASS_AFFINITY_INI[1] = 1$

$  �ޥ����졼������ǽ�ʥץ��å��Υޥ�������
$CLASS_AFFINITY_MASK[1] = 0x00000003$
$ �ƥ���
$ $CLASS_AFFINITY_MASK[1] = 0x00000001$

$  ����������ȥ���֥�å��Υ�����������
$CLASS_SECTION_TSKCB[1] = ""$

$  ���֥������ȥ���ȥ���֥�å��Υ�����������
$CLASS_SECTION_OBJCB[1] = ""$

$  �����������å��Υ�����������
$CLASS_SECTION_TSTACK[1] = ""$


$ 
$ ���饹2�˴ؤ������
$ 
$CLASS_AFFINITY_INI[2] = 2$
$CLASS_AFFINITY_MASK[2] = 0x00000003$
$ �ƥ���
$ $CLASS_AFFINITY_MASK[2] = 0x00000002$

$CLASS_SECTION_TSKCB[2] = ""$
$CLASS_SECTION_OBJCB[2] = ""$
$CLASS_SECTION_TSTACK[2] = ""$

$ 
$ ���饹 TCL_ADMIN_PRC�ʥ��饹ID 3�� �˴ؤ������
$ 
$CLASS_AFFINITY_INI[3] = 1$
$CLASS_AFFINITY_MASK[3] = 0x00000001$

$CLASS_SECTION_TSKCB[3] = ""$
$CLASS_SECTION_OBJCB[3] = ""$
$CLASS_SECTION_TSTACK[3] = ""$


$ 
$  SH7205��¸�Υƥ�ץ졼�ȥե�����Υ��󥯥롼��
$ 
$INCLUDE "sh2a_dual_gcc/sh7205.tf"$
