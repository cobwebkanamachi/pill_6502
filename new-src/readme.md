NEW-SRC made 2025/02/24(pill_6502 is almost original).<BR>
<IMG src="https://github.com/cobwebkanamachi/pill_6502/blob/pio-and-uartbased-mod/new-src/running.jpg">running image</IMG><BR>
<PRE>
Newly made uart-based platform from https://github.com/cesanta/stm32-bluepill .
On top of this, I put pill_6502 assets (main.c, acia6850.c, rom.h, fake6502.c, etc).
Why did so, It is too complicated, but explain briefly bellow.

1) USB-CDC-ACM is too large and Enumeration not complete on Windows.
2) I test several implements, but only could use USB are cesanta/stm32-bluepill
   and https://github.com/ynsrc/stm32-platformio-libopencm3-freertos .
   Perhaps another work would acomplish perfect work, but limited time to test.
3) So I port pill_6502 on https://github.com/ynsrc/stm32-platformio-libopencm3-freertos
   first choice, but this is not completed because bss shortage heavily.
4) Next I port pill_6502 on https://github.com/cesanta/stm32-bluepill.
   This made success. So made this repo.

Changes(briefly):
a. USB CDC-ACM -> UART1
b. USB rel. codes on pill_6502 were changed to UART based code.
c. acia6850.c suffer slightly change from above b., but almost original.
d. ^V and other commands reduced (tick rel.were deleted for porting simplicity).
e. Makefile is adjust to https://github.com/cesanta/stm32-bluepill.
   osi_bas folder was abondoned, but flatten to equaly another sources.
   (Notice: ROM.HEX is same as pill_6502 original.)

Tested sequece:
a. boot -> OSI basic boot.
b. several ^V and other commands.

Hardware:
Bluepill(stm32f103)
stlink clone
FT232RL(red) for UART comm.(bluepill and pc teraterm).

Software:
WSL2 (ubuntu 22.04.3 LTS (Jammy Jellyfish))
(*sorry make only: PlatformIO(mainly pio run -v: PlatformIO Core, version 6.1.17) is not used. my mistake.*)
ST Programmer(ST32CubeProgrammer v2.18.0)
openocd, arm-none-eabi-gdb

Enjoy!
