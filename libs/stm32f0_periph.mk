# STM32f0xx standard peripheral library makefile
#
# Jeff Ciesielski <jeff.ciesielski@gmail.com>

ifneq ($(STM32F0XX_LIBS),)

F0_PERIPH_BASE ?= $(LIB_PATH)/STM32F0xx_StdPeriph_LibV1.4.0/Libraries
F0_CMSIS_BASE = $(F0_PERIPH_BASE)/CMSIS
F0_CMSIS_DEVICE_BASE = $(F0_CMSIS_BASE)/Device/ST/STM32F0xx
F0_LIB_BASE = $(F0_PERIPH_BASE)/STM32F0xx_StdPeriph_Driver

STM32F0_PERIPH_C_FILES += $(F0_CMSIS_DEVICE_BASE)/Source/Templates/system_stm32f0xx.c
STM32F0_PERIPH_C_FILES += $(wildcard $(F0_LIB_BASE)/src/*.c)

ifeq ($(CPU), stm32f030)
  STM32F0_PERIPH_S_FILES = $(F0_CMSIS_DEVICE_BASE)/Source/Templates/gcc_ride7/startup_stm32f030.s
endif
ifeq ($(CPU), stm32f031)
  STM32F0_PERIPH_S_FILES = $(F0_CMSIS_DEVICE_BASE)/Source/Templates/gcc_ride7/startup_stm32f031.s
endif
ifeq ($(CPU), stm32f042)
  STM32F0_PERIPH_S_FILES = $(F0_CMSIS_DEVICE_BASE)/Source/Templates/gcc_ride7/startup_stm32f042.s
endif
ifeq ($(CPU), stm32f051)
  STM32F0_PERIPH_S_FILES = $(F0_CMSIS_DEVICE_BASE)/Source/Templates/gcc_ride7/startup_stm32f051.s
endif
ifeq ($(CPU), stm32f072)
  STM32F0_PERIPH_S_FILES = $(F0_CMSIS_DEVICE_BASE)/Source/Templates/gcc_ride7/startup_stm32f072.s
endif

# Std periph includes
LIB_INCLUDES += -I$(F0_LIB_BASE)/inc

# CMSIS Includes
LIB_INCLUDES += -I$(F0_CMSIS_BASE)/Include
LIB_INCLUDES += -I$(F0_CMSIS_BASE)/Device/ST/STM32F0xx/Include

BASE_LIBS += stm32f0_periph

endif
