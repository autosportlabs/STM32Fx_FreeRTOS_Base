# The name of our project (and the associated artifacts created)
TARGET = f0_serialcom

# board specific config file
include board/stm32f0_discovery/config.mk

#Version of FreeRTOS we'll be using
FREERTOS = FreeRTOSV7.6.0

# Uncomment the following to enable STM32 Peripheral libraries
STM32F0XX_LIBS = 1

#Uncomment the following line to enable stm32 USB libraries
#STM32_USB_LIBS = 1

#Uncomment any of the following three to build in support for USB DEV/OTG/Host
#STM32_USB_DEV = 1
#STM32_USB_HOST = 1
#STM32_USB_OTG = 1

APP_C_FILES += $(sort $(wildcard $(APP_PATH)/*.c))

#Uncomment the following to enable newlib support
APP_INCLUDES += -Iutil
APP_C_FILES  += util/newlib.c

#Uncomment the following to enable shell support
COMMAND_SHELL = 1
APP_DEFINES = -DUSART1_ENABLE

# CPU is generally defined by the Board's config.mk file
ifeq ($(CPU),)
  $(error CPU is not defined, please define it in your CPU specific config.mk file)
endif
