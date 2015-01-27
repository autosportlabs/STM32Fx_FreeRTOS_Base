# FreeRTOS configuration file
#
# Jeff Ciesielski <jeff.ciesielski@gmail.com>

ifneq ($(FREERTOS),)

FREERTOS_SOURCE ?= $(LIB_PATH)/$(FREERTOS)/FreeRTOS/Source

ifeq ($(CPU_ARCH),ARMCM0)
  FREERTOS_PORT = ARM_CM0
endif
ifeq ($(CPU_ARCH),ARMCM3)
  FREERTOS_PORT = ARM_CM3
endif
ifeq ($(CPU_ARCH),ARMCM4)
  FREERTOS_PORT = ARM_CM4F
endif

ifeq ($(FREERTOS_PORT),)
  $(error CPU_ARCH not defined. This is required to build the FreeRTOS Port)
endif

#If no heap implementation has been defined, choose heap 1
# Note that this implementation does not allow free
ifeq ($(FREERTOS_HEAP),)
  $(warning No heap selected, defaulting to heap 1 (Memory cannot be freed!!))
  FREERTOS_HEAP = heap_1
endif

FREERTOS_C_FILES += $(FREERTOS_SOURCE)/portable/GCC/$(FREERTOS_PORT)/port.c
FREERTOS_C_FILES += $(FREERTOS_SOURCE)/portable/MemMang/$(FREERTOS_HEAP).c
FREERTOS_C_FILES += $(wildcard $(FREERTOS_SOURCE)/*.c)

#FreeRTOS Includes
LIB_INCLUDES += -I$(FREERTOS_SOURCE)/include
LIB_INCLUDES += -I$(FREERTOS_SOURCE)/portable/GCC/$(FREERTOS_PORT)

BASE_LIBS += freertos

endif
