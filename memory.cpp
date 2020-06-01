#pragma once
#include <stdint.h>

#ifndef FPGA
uint16_t _ram[10000];
#define START_MEM _ram
#else
#define START_MEM 0x00
#endif

#define JOYSTICK         uintptr_t(START_MEM + 0x01)
#define JOYSTICK_ONESHOT uintptr_t(START_MEM + 0x03)
#define START_STACK      uintptr_t(START_MEM + 0x05)


#define BTN_L (1 << 1)
#define BTN_U (1 << 2)
#define BTN_D (1 << 3)
#define BTN_R (1 << 4)
#define BTN_X (1 << 5)


namespace memory {
  uintptr_t _gabage_collector = START_STACK;
  uintptr_t malloc(int size_in_byte) {
    const uintptr_t res = _gabage_collector;
    _gabage_collector += size_in_byte;
    return res;
  }
  void reset_all_molloc() {
    _gabage_collector = START_STACK;
  }

  uint16_t* mem(uintptr_t pos) {
    return (uint16_t*) pos;
  }
  void set(uintptr_t pos, uint16_t mask, bool value) {
    if (value) {
      *mem(pos) |= mask;
    } else {
      *mem(pos) &= ~mask;
    }
  }
  bool is_set(uintptr_t pos, uint16_t mask) {
    return !!(*mem(pos) & mask);
  }
  void init() {
    *mem(JOYSTICK) = uint16_t(0);
  }
};