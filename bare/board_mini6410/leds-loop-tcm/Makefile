.phony: all

all: test.bin

test.bin: test.elf
	arm-none-eabi-objcopy -O binary test.elf test.bin

test.elf: start.S main.c tcm.c uart.c linker.ld
	arm-none-eabi-gcc -c  -mcpu=arm1176jz-s -mlittle-endian -msoft-float -Wall -O2  start.S
	arm-none-eabi-gcc -c  -mcpu=arm1176jz-s -mlittle-endian -msoft-float -Wall -O2  main.c
	arm-none-eabi-gcc -c  -mcpu=arm1176jz-s -mlittle-endian -msoft-float -Wall -O2  tcm.c
	arm-none-eabi-gcc -c  -mcpu=arm1176jz-s -mlittle-endian -msoft-float -Wall -O2  uart.c
	arm-none-eabi-gcc -nostdlib  -mcpu=arm1176jz-s -N -mlittle-endian -msoft-float -T linker.ld -o test.elf start.o main.o tcm.o uart.o

clean:
	$(RM) *.o *.i *.bin
	$(RM) *.elf *~