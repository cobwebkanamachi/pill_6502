NEW-SRC made 2025/02/24(pill_6502 is almost original).
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

Software:
WLS2 (ubuntu)
PlatformIO(mainly pio run -v)
ST Programmer
openocd, arm-none-eabi-gdb

Enjoy!
