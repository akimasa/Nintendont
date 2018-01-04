#---------------------------------------------------------------------------------
.SUFFIXES:
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif
include $(DEVKITPPC)/wii_rules
#---------------------------------------------------------------------------------
TARGET	:= PADReadGC
BUILD	:= ../../../data
OUTPUT	:= $(BUILD)/$(TARGET).bin
#---------------------------------------------------------------------------------
CFLAGS	:= -Os -Wall -s -nostartfiles -mhard-float -T openstub.ld
OCFLAGS	:= -S -O binary
#---------------------------------------------------------------------------------
.PHONY: all clean

all: $(OUTPUT)

$(TARGET).elf: $(TARGET).c
	@echo " COMPILE     $<"
	@$(CC) $(CFLAGS) $^ -o $@

$(OUTPUT): $(TARGET).elf
	@echo " OBJCOPY     $(TARGET).bin"
	@$(OBJCOPY) $(OCFLAGS) $^ $@

clean:
	rm -fr $(TARGET).elf $(BUILD)/$(TARGET).bin
