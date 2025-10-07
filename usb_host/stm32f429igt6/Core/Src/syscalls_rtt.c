#include "stm32f4xx.h"

int __io_putchar(int ch) {
  return SEGGER_RTT_PutChar(0, ch);
}