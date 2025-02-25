TARGET  = bluepill
ARCH    = arm-none-eabi
CFLAGS  = -W -Wall -g -Os -mcpu=cortex-m3 -mthumb -mabi=aapcs -mfloat-abi=soft $(CFLAGS_EXTRA)
LDFLAGS = -Wl,--gc-sections -march=armv7-m -mabi=aapcs  -nostartfiles -nostdlib -lc -lnosys -lgcc
AFLAGS  = --warn --fatal-warnings -mcpu=cortex-m3
ROM     = ./osi_bas/ROM.HEX
OBJS    = obj/bootstrap.o obj/main.o obj/acia6850.o obj/fake6502.o obj/ROM.o
OBJCOPY = $(ARCH)-objcopy

all: $(TARGET).hex

$(TARGET).bin: obj/$(TARGET).elf
	$(ARCH)-objcopy -O binary $< $@

$(TARGET).hex: $(TARGET).bin
	$(ARCH)-objcopy -I binary -O ihex --change-address 0x8000000 $< $@

obj/$(TARGET).elf: $(OBJS)
	$(ARCH)-gcc $^ $(LDFLAGS) -T link.ld -o $@ -Wl,-Map=$@.map

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(ARCH)-gcc $(CFLAGS) -c $< -fdata-sections -ffunction-sections -o $@

obj/%.o: %.s
	@mkdir -p $(dir $@)
	$(ARCH)-as $(AFLAGS) $< -o $@

obj/%.o: %.HEX
	@echo $(OBJCOPY $@.bin)
	$(OBJCOPY) -I ihex -O binary $^ $@.bin
	@echo $(OBJCOPY $@)
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm --rename-section .data=.rodata,alloc,load,readonly,data,contents $@.bin $@

clean:
	@rm -rf *.{bin,elf,map,lst,tgz,zip,hex} obj
