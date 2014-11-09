include cpu/stm32f0xx/config.mk
CPU = stm32f072
CPU_LINK_MEM = f072_mem.ld

#this board uses the internal 8MHZ osc and has an stm32f072 onboard
BOARD_DEFINES += -DSTM32F072

# Uncomment the following line to use the high speed internal clock
BOARD_DEFINES += -DPLL_SOURCE_HSI -DHSI_VALUE=8000000 -DPLL_M=8

#Uncomment the following to use a high speed external clock
#BOARD_DEFINES += -DPLL_SOURCE_HSE=1 -DHSE_VALUE=8000000 -DPLL_M=8



