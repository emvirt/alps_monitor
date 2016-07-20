.phony: all

all: test.bin

test.bin: test.elf
	arm-none-eabi-objcopy -O binary test.elf test.bin

test.elf: start.S main.c uart.c linker.ld
	arm-none-eabi-gcc -c -g  -mcpu=cortex-a8 -mlittle-endian -msoft-float -Wall -O2  start.S
	arm-none-eabi-gcc -c -g -mcpu=cortex-a8 -mlittle-endian -msoft-float -Wall -O2  main.c
	arm-none-eabi-gcc -c -g -mcpu=cortex-a8 -mlittle-endian -msoft-float -Wall -O2  uart.c
	arm-none-eabi-gcc -g -nostdlib  -mcpu=cortex-a8 -N -mlittle-endian -msoft-float -T linker.ld -o test.elf start.o main.o uart.o

clean:
	$(RM) *.o *.i *.bin
	$(RM) *.elf *~
