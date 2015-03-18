# This file is part of the Racecapture/Track project
#
# Copyright (C) 2013 Autosport Labs
#
# Author(s):
# 	Andrey Smirnov <andrew.smirnov@gmail.com>
# 	Jeff Ciesielski <jeffciesielski@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# This file can be used to autoconfigure your board for ease of
# building, otherwise it passes the BOARD environment variable to the
# rest of the makefile
-include config.mk

ifeq ($(APP),)
  ifeq ($(wildcard app.default),)
    $(error APP is not defined.  Pass it in as APP= or create a config.mk file)
  else
    APP = $(shell cat app.default)
  endif
else
  $(shell echo "$(APP)" > app.default)
endif

APP_PATH     := app/$(APP)
LIB_PATH     := libs
APP_INCLUDES += -I$(APP_PATH)

# load the board specific configuration
include $(APP_PATH)/config.mk

ifeq ($(CPU_TYPE),)
  $(error CPU_TYPE is not defined, please ensure it is defined in your cpu config.mk)
endif

PREFIX	?= arm-none-eabi

CC      := $(PREFIX)-gcc
AS      := $(PREFIX)-as
CXX     := $(PREFIX)-g++
AR      := $(PREFIX)-ar
NM      := $(PREFIX)-nm
OBJCOPY := $(PREFIX)-objcopy
OBJDUMP := $(PREFIX)-objdump
SIZE    := $(PREFIX)-size

OPENOCD := openocd
DDD     := ddd
GDB     := $(PREFIX)-gdb

#application libraries
include libs.mk

LIBS = -lnosys
LIBS += $(addprefix -l,$(BASE_LIBS))

INCLUDES += $(CPU_INCLUDES) $(BOARD_INCLUDES) $(LIB_INCLUDES) $(APP_INCLUDES)
CFLAGS    = $(INCLUDES) $(CPU_DEFINES) $(BOARD_DEFINES) $(APP_DEFINES) $(CPU_FLAGS) \
            -Os -g -Wall -fno-common -c -mthumb -mcpu=$(CPU_TYPE) -MD -std=gnu99
ASFLAGS   = -mcpu=$(CPU_TYPE) $(FPU) -g -Wa,--warn
ARFLAGS   = rv
LDFLAGS  ?= --specs=nano.specs -lc -lgcc $(LIBS) -mcpu=$(CPU_TYPE) -g -gdwarf-2 \
	 -L. -Lcpu/common -L$(CPU_BASE) -T$(CPU_LINK_MEM) -Tlink_sections.ld \
	 -nostartfiles -Wl,--gc-sections -mthumb -mcpu=$(CPU_TYPE) \
	 -msoft-float

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
  Q := @
  # Do not print "Entering directory ...".
  MAKEFLAGS += --no-print-directory
  # Redirect stdout/stderr for chatty tools
  NOOUT = 1> /dev/null 2> /dev/null
endif

STM32F0_DRIVERS_D_FILES = $(STM32F0_DRIVERS_C_FILES:.c=.d)
STM32F0_DRIVERS_O_FILES = $(STM32F0_DRIVERS_C_FILES:.c=.o)
FREERTOS_D_FILES        = $(FREERTOS_C_FILES:.c=.d)
FREERTOS_O_FILES        = $(FREERTOS_C_FILES:.c=.o)       $(FREERTOS_S_FILES:.s=.o)
STM32F0_PERIPH_D_FILES  = $(STM32F0_PERIPH_C_FILES:.c=.d)
STM32F0_PERIPH_O_FILES  = $(STM32F0_PERIPH_C_FILES:.c=.o) $(STM32F0_PERIPH_S_FILES:.s=.o)
STM32F4_PERIPH_D_FILES  = $(STM32F4_PERIPH_C_FILES:.c=.d)
STM32F4_PERIPH_O_FILES  = $(STM32F4_PERIPH_C_FILES:.c=.o) $(STM32F4_PERIPH_S_FILES:.s=.o)
STM32_USB_D_FILES       = $(STM32_USB_C_FILES:.c=.d)
STM32_USB_O_FILES       = $(STM32_USB_C_FILES:.c=.o)      $(STM32_USB_S_FILES:.s=.o)
APP_D_FILES             = $(APP_C_FILES:.c=.d)
APP_O_FILES             = $(APP_C_FILES:.c=.o)            $(APP_S_FILES:.s=.o)
SHELL_D_FILES           = $(SHELL_C_FILES:.c=.d)
SHELL_O_FILES           = $(SHELL_C_FILES:.c=.o)

ifeq ($(TARGET),)
  $(error TARGET is not defined, please define it in your applications config.mk)
endif

LIBS_ALL = $(addprefix lib,$(BASE_LIBS:=.a))

$(TARGET).bin: $(TARGET).elf
	@printf "  OBJCOPY $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(PREFIX)-objcopy -Obinary $< $@

$(TARGET).elf: $(LIBS_ALL) $(APP_O_FILES)
	@printf "  LD      $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(CC) -o $@ $(APP_O_FILES) $(LDFLAGS)
	$(Q)$(PREFIX)-size $(TARGET).elf

libfreertos.a: $(FREERTOS_O_FILES)
	@printf "  AR      $(@)\n"
	$(Q)$(AR) $(ARFLAGS) $@ $^ $(NOOUT)
ifneq ($(FREERTOS_WARNING),)
	@echo $(FREERTOS_WARNING)
endif

libstm32f0_drivers.a: $(STM32F0_DRIVERS_O_FILES)
	@printf "  AR      $(@)\n"
	$(Q)$(AR) $(ARFLAGS) $@ $^ $(NOOUT)

libstm32f0_periph.a: $(STM32F0_PERIPH_O_FILES)
	@printf "  AR      $(@)\n"
	$(Q)$(AR) $(ARFLAGS) $@ $^ $(NOOUT)

libstm32f4_periph.a: $(STM32F4_PERIPH_O_FILES)
	@printf "  AR      $(@)\n"
	$(Q)$(AR) $(ARFLAGS) $@ $^ $(NOOUT)

libstm32_usb.a: $(STM32_USB_O_FILES)
	@printf "  AR      $(@)\n"
	$(Q)$(AR) $(ARFLAGS) $@ $^ $(NOOUT)

libshell.a: $(SHELL_O_FILES)
	@printf " AR       $(@)\n"
	$(Q)$(AR) $(ARFLAGS) $@ $^ $(NOOUT)

%.o: %.c
	@printf "  CC      $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.s
	@printf "  AS      $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(CC) $(ASFLAGS) -c -o $@ $<

clean:
	$(Q)rm -f \
	$(SHELL_D_FILES)            \
	$(SHELL_O_FILES)            \
	$(STM32F0_DRIVERS_D_FILES)  \
	$(STM32F0_DRIVERS_O_FILES)  \
	$(FREERTOS_D_FILES)         \
	$(FREERTOS_O_FILES)         \
	$(STM32F0_PERIPH_D_FILES)   \
	$(STM32F0_PERIPH_O_FILES)   \
	$(STM32F4_PERIPH_D_FILES)   \
	$(STM32F4_PERIPH_O_FILES)   \
	$(STM32_USB_D_FILES)        \
	$(STM32_USB_O_FILES)        \
	$(APP_D_FILES)              \
	$(APP_O_FILES)              \
	$(TARGET).bin $(TARGET).elf \
	$(LIBS_ALL)

st-flash: $(TARGET).bin
	sudo st-flash write $(TARGET).bin 0x08000000

debug: $(TARGET).elf
	$(OPENOCD) -f $(APP_PATH)/debug.ocd

flash: $(TARGET).elf
	$(OPENOCD) -f $(APP_PATH)/debug.ocd -f $(APP_PATH)/flash.ocd

ddd: $(TARGET).elf
	$(DDD) --eval-command="target remote localhost:3333" --debugger $(GDB) $(TARGET).elf

gdb: $(TARGET).elf
	$(GDB) -ex "target ext localhost:3333" -ex "mon reset halt" -ex "mon arm semihosting enable" $(TARGET).elf

-include $(STM32F0_DRIVERS_D_FILES)	
-include $(FREERTOS_D_FILES)
-include $(STM32F0_PERIPH_D_FILES)
-include $(STM32F4_PERIPH_D_FILES)
-include $(APP_D_FILES)
-include $(SHELL_D_FILES)

.PHONY: clean st-flash debug flash ddd gdb
