; @1176CA-nowait.ARM11760-CA:ARM-ARM-MXS
load/r 'Z:\shared\safeg.elf' &s:0
settings,private "0,0,0,1,0,0,0,0x0"
load/np 'Z:\shared\trust.elf' &s:0
ce ((unsigned long *)S:0x10101810)[0]=0x3FFF
load/np 'Z:\shared\non-trust.elf' &n:0
