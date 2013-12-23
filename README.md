STM32Fx_FreeRTOS_Base
=====================

FreeRTOS starter project for STM32 F1 and F4 microcontrollers.

## Prerequisites
1. GNU Make
2. [gcc-arm-embedded](https://launchpad.net/gcc-arm-embedded)

## Creating a new project:
1. Create (if necessary) board and CPU definitions (see existing examples for details).
2. Create an application folder in the apps directory
3. Create a config.mk file for your application (see existing examples)
4. Store your application code in the newly created directory and edit your config.mk file appropriately

## Building
1. 'cd' to the root of base project directory
2. type 'make APP=<your_app_name>'
