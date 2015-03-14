# Command shell configuration file
#
# Jeff Ciesielski <jeff.ciesielski@gmail.com>

ifneq ($(STM32F0_DRIVERS),)

STM32F0_DRIVERS_SOURCE ?= $(LIB_PATH)/stm32f0_drivers

STM32F0_DRIVERS_C_FILES = $(STM32F0_DRIVERS_SOURCE)/usart.c

# Shell includes
LIB_INCLUDES += -I$(STM32F0_DRIVERS_SOURCE)
BASE_LIBS += stm32f0_drivers

endif
