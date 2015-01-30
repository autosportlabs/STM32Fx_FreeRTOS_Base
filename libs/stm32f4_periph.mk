# STM32f4xx standard peripheral library makefile
#
# Jeff Ciesielski <jeff.ciesielski@gmail.com>

# TODO: Add support for DSP library

ifneq ($(STM32F4XX_LIBS),)

F4_PERIPH_BASE ?= $(LIB_PATH)/STM32F4xx_DSP_StdPeriph_Lib_V1.0.1/Libraries
F4_CMSIS_BASE = $(F4_PERIPH_BASE)/CMSIS
F4_CMSIS_DEVICE_BASE = $(F4_CMSIS_BASE)/Device/ST/STM32F4xx
F4_LIB_BASE = $(F4_PERIPH_BASE)/STM32F4xx_StdPeriph_Driver

STM32F4_PERIPH_C_FILES += $(F4_CMSIS_DEVICE_BASE)/Source/Templates/system_stm32f4xx.c
STM32F4_PERIPH_C_FILES += $(sort $(wildcard $(F4_LIB_BASE)/src/*.c))
STM32F4_PERIPH_S_FILES += $(F4_CMSIS_DEVICE_BASE)/Source/Templates/gcc_ride7/startup_stm32f4xx.s

# Std periph includes
LIB_INCLUDES += -I$(F4_LIB_BASE)/inc

# CMSIS Includes
LIB_INCLUDES += -I$(F4_CMSIS_BASE)/Include
LIB_INCLUDES += -I$(F4_CMSIS_BASE)/Device/ST/STM32F4xx/Include

BASE_LIBS += stm32f4_periph

endif
