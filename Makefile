build.hex: build.out
	arm-none-eabi-objcopy -O ihex build.out build.hex
	arm-none-eabi-objcopy -O binary build.out build.bin

build.out: main.o stm32_startup.o
	arm-none-eabi-ld -T stm32_ls.ld -Map=build_example.map -o build.out main.o stm32_startup.o

main.o: main.c
	arm-none-eabi-gcc -c -g -I. -Iinclude -O0 -mcpu=cortex-m3 -mthumb -nostartfiles -mhard-float -mfpu=vfpv2 main.c -o main.o

stm32_startup.o: stm32_startup.c
	arm-none-eabi-gcc -c -g -I. -Iinclude -O0 -mcpu=cortex-m3 -mthumb -nostartfiles -mhard-float -mfpu=vfpv2 stm32_startup.c -o stm32_startup.o

.PHONY: clean
clean:
	del /q *.o *.out