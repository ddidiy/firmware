
RM = rm -f
RMDIR = rm -f -r
MKDIR = mkdir -p

# GCC_ARM_PATH can be defined to be the path of the GCC ARM compiler,
# It must include a final slash! Default is empty
# GCC_PREFIX can be set to the prefix added to GCC ARM names.
# Default is arm-none-eabi-

CC = $(GCC_ARM_PATH)$(GCC_PREFIX)gcc
CPP = $(GCC_ARM_PATH)$(GCC_PREFIX)g++
AR = $(GCC_ARM_PATH)$(GCC_PREFIX)ar
OBJCOPY = $(GCC_ARM_PATH)$(GCC_PREFIX)objcopy
OBJDUMP = $(GCC_ARM_PATH)$(GCC_PREFIX)objdump
SIZE = $(GCC_ARM_PATH)$(GCC_PREFIX)size
DFU = dfu-util
DFUSUFFIX = dfu-suffix
CURL = curl
CRC = crc32
XXD = xxd
ESP_TOOL = $(PROJECT_ROOT)/tools/esptool/esptool
ST-FLASH = $(PROJECT_ROOT)/tools/stlink/st-flash
ESP_TOOL_PY = python $(PROJECT_ROOT)/tools/esptool/esptool.py

CPPFLAGS +=

