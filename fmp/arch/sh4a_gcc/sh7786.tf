$ 
$		�������ƥ������¸�ƥ�ץ졼�ȡ�SH7786�ѡ�
$ 

$ 
$ ͭ����CPU�㳰�ϥ�ɥ��ֹ�
$ 
$EXCNO_VALID = {
					0x101E0;0x100E0;0x10040;0x100A0;0x10180;0x101A0;0x10800;0x10820;0x100E0;0x10100;
					0x10040;0x10060;0x100A0;0x100C0;0x10120;0x10080;0x10160;0x101E0;0x101C0;
					0x201E0;0x200E0;0x20040;0x200A0;0x20180;0x201A0;0x20800;0x20820;0x200E0;0x20100;
					0x20040;0x20060;0x200A0;0x200C0;0x20120;0x20080;0x20160;0x201E0;0x201C0
				}$

$ 
$  ͭ���ʳ�����ֹ桤����ߥϥ�ɥ��ֹ桤CPU�㳰�ϥ�ɥ��ֹ�
$ 
$INTNO_VALID = { 
                    0x00400,0x00420,0x00440,0x00460,0x00480;
					0x00720,0x00740,0x00760,0x00780;
					0x00F00,0x00F20 
				}$

$INHNO_VALID = { 
                    0x00400,0x00420,0x00440,0x00460,0x00480;
					0x10720,0x10740,0x10760,0x20780;
					0x10F00,0x20F20
}$



$ 
$  ������ֹ桤�����ߥϥ�ɥ��ֹ�Υޥ���
$ 
$INTNO_MASK = 0x00000FFF$
$INHNO_MASK = INTNO_MASK$



$INTNO_RANGE = { 0x00000,0x00020,...,0x00FE0 }$

$  $INHNO_RANGE = INTNO_RANGE$

$INHNO_RANGE = { 0x00000,0x00020,...,0x020FE0 }$

$ 
$  �ץ����å���¸�Υƥ�ץ졼�ȥե�����Υ��󥯥롼��
$ 
$INCLUDE"sh4a_gcc/prc.tf"$