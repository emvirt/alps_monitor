.phony: all

all: test.bin

test.bin: test.elf
	arm-none-eabi-objcopy -O binary test.elf test.bin

test.elf: start.S main.c vic.c timer.c uart.c linker.ld
	arm-none-eabi-gcc -c -g -mcpu=arm1176jz-s -mlittle-endian -msoft-float -Wall -O2  start.S
	arm-none-eabi-gcc -c -g -mcpu=arm1176jz-s -mlittle-endian -msoft-float -Wall -O2  main.c
	arm-none-eabi-gcc -c -g -mcpu=arm1176jz-s -mlittle-endian -msoft-float -Wall -O2  vic.c
	arm-none-eabi-gcc -c -g -mcpu=arm1176jz-s -mlittle-endian -msoft-float -Wall -O2  timer.c
	arm-none-eabi-gcc -c -g -mcpu=arm1176jz-s -mlittle-endian -msoft-float -Wall -O2  uart.c
	arm-none-eabi-gcc -g -nostdlib  -mcpu=arm1176jz-s -N -mlittle-endian -msoft-float -T linker.ld -o test.elf start.o main.o vic.o timer.o uart.o

clean:
	$(RM) *.o *.i *.bin
	$(RM) *.elf *~
