CPU_ARCH = ARMCM0
CPU_TYPE = cortex-m0
CPU_BASE = cpu/stm32f0xx

STM32F0_DRIVERS = 1

CPU_FLAGS = -mfloat-abi=soft
CPU_DEFINES = -D$(CPU_ARCH) -DSTM32F0XX
