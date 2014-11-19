# STM32f0xx standard peripheral library makefile
#
# Jeff Ciesielski <jeff.ciesielski@gmail.com>

ifneq ($(STM32F0XX_LIBS),)

F0_PERIPH_BASE ?= libs/STM32F0xx_StdPeriph_LibV1.4.0/Libraries
F0_CMSIS_BASE = $(F0_PERIPH_BASE)/CMSIS
F0_CMSIS_DEVICE_BASE = $(F0_CMSIS_BASE)/Device/ST/STM32F0xx
F0_LIB_BASE = $(F0_PERIPH_BASE)/STM32F0xx_StdPeriph_Driver

F0_PERIPH_SOURCES := $(wildcard $(F0_LIB_BASE)/src/*.c)
F0_CMSIS_SOURCES  = $(F0_CMSIS_DEVICE_BASE)/Source/Templates/system_stm32f0xx.c

ifeq ($(CPU), stm32f030)
F0_CMSIS_STARTUP_SOURCES = $(F0_CMSIS_DEVICE_BASE)/Source/Templates/gcc_ride7/startup_stm32f030.s
endif
ifeq ($(CPU), stm32f031)
F0_CMSIS_STARTUP_SOURCES = $(F0_CMSIS_DEVICE_BASE)/Source/Templates/gcc_ride7/startup_stm32f031.s
endif
ifeq ($(CPU), stm32f042)
F0_CMSIS_STARTUP_SOURCES = $(F0_CMSIS_DEVICE_BASE)/Source/Templates/gcc_ride7/startup_stm32f042.s
endif
ifeq ($(CPU), stm32f051)
F0_CMSIS_STARTUP_SOURCES = $(F0_CMSIS_DEVICE_BASE)/Source/Templates/gcc_ride7/startup_stm32f051.s
endif
ifeq ($(CPU), stm32f072)
F0_CMSIS_STARTUP_SOURCES = $(F0_CMSIS_DEVICE_BASE)/Source/Templates/gcc_ride7/startup_stm32f072.s
endif

# Std periph includes
LIB_INCLUDES += -I$(F0_LIB_BASE)/inc

# CMSIS Includes
LIB_INCLUDES += -I$(F0_CMSIS_BASE)/Include
LIB_INCLUDES += -I$(F0_CMSIS_BASE)/Device/ST/STM32F0xx/Include

STM32F0_PERIPH_SRCS = $(F0_CMSIS_SOURCES) $(F0_PERIPH_SOURCES)

STM32F0_PERIPH_OBJS = $(STM32F0_PERIPH_SRCS:.c=.o) $(F0_CMSIS_STARTUP_SOURCES:.s=.o)

BASE_LIBS += stm32f0_periph

endif
