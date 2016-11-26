# Makefile to create a simple demo app using FreeRTOS on XMEGA-B1 Xplained board
#
# ©2015 kcs
#
# based on FreeRTOS demo projects

# avoid displaying commands
export MAKEFLAGS += --silent

# name of the project
APP = freertos-xplained

#===============================================================================
# Paths
#===============================================================================

BIN_PATH = bin
OBJ_PATH = obj
SRC_PATH = src

FREERTOS_PATH = src/FreeRTOS
FREERTOS_SRC_PATH = $(FREERTOS_PATH)/source
FREERTOS_INC_PATH = $(FREERTOS_PATH)/include
FREERTOS_PORT_PATH = $(FREERTOS_PATH)/portable

# insert source directories into the search path
vpath %.c   $(SRC_PATH) \
            $(FREERTOS_SRC_PATH) \
            $(FREERTOS_PORT_PATH) \

#===============================================================================
# Source files
#===============================================================================

FREERTOS_SRC = \
               tasks.c \
               queue.c \
               list.c \
               croutine.c \
               MemMang/heap_1.c \
               port.c \

APP_SRC =      \
               main.c \
               xplained.c \


SRC = $(APP_SRC) $(FREERTOS_SRC)

#===============================================================================
# Includes
#===============================================================================

INC_PATH = \
           $(SRC_PATH) \
           $(FREERTOS_INC_PATH) \
           $(FREERTOS_PORT_PATH) \

INCLUDES = $(addprefix -I,$(INC_PATH))


#===============================================================================
# Configuration
#===============================================================================

# mcu = ATxmega128B1
MCU = atxmega128b1

# debug level: only affects ELF, not translated into the HEX, so can be left
# to highest level for easier debugging
DEBUG = -g3

# optimization: either optimize for size or for speed, when optimizing
# for speed level 2 is usually better overall than level 3 for the size
# constraints of AVR microconrollers
OPT = -Os

# warnings
WARNINGS = -Wall -Wextra -Werror \
           -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align \
           -Wsign-compare -Waggregate-return -Wstrict-prototypes \
           -Wmissing-prototypes -Wmissing-declarations -Wunused


# extra flags
# recommended for AVR microcontrollers
EXTRA_FLAGS = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums

# language standard
# use latest standard with GNU extensions
C_STD = gnu99

#===============================================================================
# Defines
#===============================================================================

DEFS = \
       GCC_XMEGA \

#===============================================================================
# Compiler flags
#===============================================================================

CFLAGS = \
         -mmcu=$(MCU) \
         $(DEBUG) \
         $(OPT) \
         $(WARNINGS) \
         $(EXTRA_FLAGS) \
         -std=$(C_STD) \
         $(addprefix -D,$(DEFS)) \
         -Wa,-adhlns=$(@:.o=.lst) \

ASFLAGS = \
         -Wa,-adhlns=$(@:.o=.lst),-gstabs \

LDFLAGS = \
         -mmcu=$(MCU) \
          $(DEBUG) \
          $(OPT) \
          $(WARNINGS) \
          -Wl,-Map=$(OBJ_PATH)/$(APP).map,--cref \

# additional libraries like math
# be aware that floating point mathematics are only available as software
# implementation and are extremely costly to run on AVR
LIBS = #-lm


# ISP programmer - going for Xmega Xplained board which uses the new flip
# protocol to program it
ISP = flip2

# if other programmer is needed extra flags (like -P port) can be set here
ISP_FLAGS = \
            # -P /dev/ttyACM0

#====================================================================
# Tools
#====================================================================

CROSS := avr-
CC    := $(CROSS)gcc
LD    := $(CROSS)gcc
OBJCOPY := $(CROSS)objcopy
OBJDUMP := $(CROSS)objdump

AVRDUDE := avrdude

RM      := rm -f
RMDIR   := rm -rf
MKDIR   := mkdir -p
CP      := cp

#====================================================================
# Compiling and programming commands
#====================================================================

TARGET = $(BIN_PATH)/$(APP)
TARGET_ELF = $(TARGET).elf
TARGET_HEX = $(TARGET).hex
TARGET_EEP = $(TARGET).eep
TARGET_LSS = $(OBJ_PATH)/$(APP).lss


# program into the flash
#AVRDUDE_FLASH = -U flash:w:$(TARGET_HEX)
AVRDUDE_FLASH = -U application:w:$(TARGET_HEX)
#AVRDUDE_EEPROM = -U eeprom:w:$(TARGET_EEP)

INSTALL = $(AVRDUDE) -p $(MCU) -c $(ISP) $(ISP_FLAGS) \
                     $(AVRDUDE_FLASH) \
                     $(AVRDUDE_WRITE_EEPROM)


# Define all object files.
OBJ = $(addprefix $(OBJ_PATH)/, $(SRC:.c=.o) $(ASRC:.S=.o))

# compiling C files
CCRUN = $(CC) -c $(CFLAGS) $(INCLUDES) -MMD -MP -MT $@ -o $@ $<

#====================================================================
# Rules
#====================================================================

# Default target.
all: directories $(TARGET_ELF) $(TARGET_HEX) $(TARGET_EEP) $(TARGET_LSS)


# main binary in elf format
$(TARGET_ELF): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) $(LIBS) -o $@


# other output from the elf
$(TARGET_HEX): $(TARGET_ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(TARGET_EEP): $(TARGET_ELF)
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O ihex $< $@

$(TARGET_LSS): $(TARGET_ELF)
	$(OBJDUMP) -h -S $< > $@


# Program the device.
install: $(TARGET).hex $(TARGET).eep
	$(INSTALL)


# create directories needed for project
directories:
	$(MKDIR) $(sort $(dir $(TARGET_ELF) $(TARGET_HEX) $(TARGET_EEP) $(TARGET_LST) $(OBJ)))

# Target: clean project.
clean:
	$(RMDIR) $(BIN_PATH)/*
	$(RMDIR) $(OBJ_PATH)/*


# Phony targets
.PHONY: all clean directories


# Implicit rules

$(foreach path, $(sort $(dir $(OBJ))), $(path)%.o): %.c
	@echo "(CC) $<"
	$(CCRUN)

#$(foreach path, $(sort $(dir $(OBJ))), $(path)%.o): %.S
#	@echo (AS) $<
#	$(ASRUN)


# Dependencies
ifneq "$(MAKECMDGOALS)" "clean"
-include $(OBJ:.o=.d)
endif
