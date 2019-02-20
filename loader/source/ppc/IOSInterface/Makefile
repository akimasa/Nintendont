#---------------------------------------------------------------------------------
.SUFFIXES:
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif
include $(DEVKITPPC)/wii_rules
#---------------------------------------------------------------------------------
TARGET	:= IOSInterface
BUILD	:= ../../../data
OUTPUT	:= $(BUILD)/$(TARGET).bin
#---------------------------------------------------------------------------------
CFLAGS	:= -O1 -Wall -s -nostartfiles -nostdlib -mhard-float
LDFLAGS	:= -T link.ld
OCFLAGS	:= -S -O binary
#---------------------------------------------------------------------------------
.PHONY: all clean

SRCS := _crt0.S cache.c debug.c sock.c usb.c
OBJS := _crt0.o cache.o debug.o sock.o usb.o

all: $(OUTPUT)

%.o: %.S
	@echo " ASSEMBLE    $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	@echo " COMPILE     $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	@echo " LINK        $@"
	@$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OUTPUT): $(TARGET).elf
	@echo " OBJCOPY     $(TARGET).bin"
	@$(OBJCOPY) $(OCFLAGS) $^ $@

clean:
	rm -fr *.o $(TARGET).elf $(BUILD)/$(TARGET).bin
