// Copyright (c) 2018-2021 Cesanta Software Limited
// All rights reserved

#include "stm32f1.h"
#include "rom.h"
#include "acia6850.h"

static int blink_period = 450000;
static paused = 0;

// 6502 processor memory, 16KB (< 20KB)
static uint8_t ram[0x4000];
uint8_t read6502(uint16_t address) {
        // RAM
        if (address < sizeof(ram)) {
                return ram[address];
        }

        // ROM
        if (address >= 0xc000) {
		//nm : _binary_obj_ROM_o_bin
                const uint8_t *rom = &_binary_obj_ROM_o_bin_start;

                return rom[address - 0xc000];
        }

        // ACIA
        if (address >= 0xa000 && address <= 0xbfff) {
                return read6850(address);
        }

        return 0xff;
}

void write6502(uint16_t address, uint8_t value) {
        // RAM
        if (address < sizeof(ram)) {
                ram[address] = value;
        }

        // ACIA
        if (address >= 0xa000 && address <= 0xbfff) {
                write6850(address, value);
        }
}

static void setup(void) {
  INIT_MEMORY;

  RCC->APB2ENR |= BIT(4);                // GPIOC, for LED on PC13
  SET_PIN_MODE(GPIOC, LED_PIN, 0b0110);  // open drain, output 2

  RCC->APB2ENR |= BIT(2);   // GPIOA, for USART1 TX/RX on PA9 and PA10
  RCC->APB2ENR |= BIT(14);  // enable clock on USART1
  SET_PIN_MODE(GPIOA, TX_PIN, 0b1010);     // TX pin mode = AF, push/pull
  SET_PIN_MODE(GPIOA, RX_PIN, 0b0100);     // RX pin mode = input, floating
  UART1->BRR = 0x45;                       // Set baud rate, TRM 27.3.4
  UART1->CR1 = BIT(13) | BIT(2) | BIT(3);  // Enable USART1
}

void uart_putc(struct uart *uart, uint8_t byte) {
  uart->DR = byte;                  // Write next byte
  do {
  } while ((uart->SR & BIT(7)) == 0);  // Wait until byte is scheduled
}

char *process_serial_command(char b) {
        if (b == '\x16') { // ^V
                return "Pill 6502 version TEST Ver TEST01"; //FIRMWARE_VERSION;
        } else if (b == '\x10') { // ^P
                paused = !paused;
                return paused ? "paused" : "resumed";
        } else if (b == '\x06') { // ^F
        } else if (b == '\x12') { // ^R
                reset6502();
                paused = 0;
                return "reset";
        } else if (b == '\x07') { // ^G
                return "^V=version ^R=reset ^P=pause ^T=timing ^G=help";
        }

        return NULL;
}

int main(void) {
  setup();
  volatile int count = 0, led_on = 0;
  reset6502();
  for (;;) {
    if (UART_HAS_DATA(UART1)) {
      int ch = UART_READ(UART1);
      //uart_putc(UART1, 'i');
      //uart_putc(UART1, ':');
      //uart_putc(UART1, ch);
      char *a=process_serial_command(ch);
      while((*a++)!='\0'){
            uart_putc(UART1, *a);
      }
      process_serial_input_byte(ch);
    }
    if (++count > blink_period) {
      count = 0;
      led_on = !led_on;
      GPIOC->BSRR |= BIT(LED_PIN + (led_on ? 0 : 16));
      //uart_putc(UART1, led_on ? '1' : '0');
    }
    step6502();
  }
  return 0;
}

void *_sbrk() {
  return 0;
}
