# This is a sort of meta package for all of the libraries we produce
# in the 'libs' folder.  To add a new library, see existing library
# config files for details on build requirements
include libs/stm32f4_periph.mk
include libs/stm32f0_periph.mk
include libs/freertos.mk
include libs/stm32_usb.mk
