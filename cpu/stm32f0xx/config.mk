CPU_ARCH = ARMCM0
CPU_TYPE = cortex-m0
CPU_BASE = cpu/stm32f0xx

CPU_FLAGS = -mfloat-abi=soft
CPU_DEFINES = -D$(CPU_ARCH) -DSTM32F0XX
