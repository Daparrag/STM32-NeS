#ifndef __apu_cpu_h
#define __apu_cpu_h

uint8_t APU_access(int elapsed, uint8_t wr ,uint16_t addr, uint8_t val);
void APU_runframe(int elapsed);
void APU_reset();
void APU_init();

#endif

