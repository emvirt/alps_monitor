.phony: all

all: test.bin

test.bin: test.elf
	arm-none-eabi-objcopy -O binary test.elf test.bin

test.elf: start.S main.c uart.c linker.ld
	arm-none-eabi-gcc -c -g  -mcpu=cortex-a8 -mlittle-endian -msoft-float -Wall -O2  start.S
	arm-none-eabi-gcc -c -g -mcpu=cortex-a8 -mlittle-endian -msoft-float -Wall -O2  main.c
	arm-none-eabi-gcc -c -g -mcpu=cortex-a8 -mlittle-endian -msoft-float -Wall -O2  tzic.c
	arm-none-eabi-gcc -c -g -mcpu=cortex-a8 -mlittle-endian -msoft-float -Wall -O2  uart.c
	arm-none-eabi-gcc -g -nostdlib -mcpu=cortex-a8 -N -mlittle-endian -msoft-float -T linker.ld -o test.elf start.o main.o tzic.o uart.o ~/app/arm-none-eabi-2010.09.51/lib/gcc/arm-none-eabi/4.5.1/libgcc.a

clean:
	$(RM) *.o *.i *.bin
	$(RM) *.elf *~
