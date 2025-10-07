#include "stm32f4xx.h"

int __io_putchar(int ch) {
  // fcking bloccking here?!
  while (ITM->PORT[0].u32 == 0) {
  }
  ITM->PORT[0].u8 = ch;
  return 1;
}