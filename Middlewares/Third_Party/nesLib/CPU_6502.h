#ifndef _cpu6502_h
#define _cpu6502_h

#include <stdio.h>
#include <stdint.h>


typedef struct _M6502		M6502; //M6502 vCPU define
typedef struct _M6502_Registers	M6502_Registers; //M6502 vRegister define
typedef uint16_t (*Mode)(void);              // Addressing mode.
typedef uint16_t cpu_addr_t; // 16-bit memory address

#define CPU6502_NMIVector  0xfffa
#define CPUM6502_RSTVector 0xfffc
#define CPUM6502_IRQVector 0xfffe


enum IntType {
	INT_NMI,	/* Non-maskable interrupt     */
	INT_RESET,	/* Reset interrupt     		  */
	INT_IRQ,	/* Standard IRQ interrupt     */
	INT_BRK		/* Break interrupt     		  */
};


struct _M6502_Registers
{
  uint8_t   a;	/* accumulator */
  uint8_t   x;	/* X index register */
  uint8_t   y;	/* Y index register */
  uint8_t   p;	/* processor status register */
  uint8_t   s;	/* stack pointer */
  uint16_t pc;	/* program counter */
};


struct _M6502
{
  M6502_Registers  registers;
  uint8_t	  		ram [0x0800];
  uint8_t	  		flags;
  int    			ticks;
  int 				remainingCycles;
};


void M6502_set_nmi();
int dmc_read(void*, cpu_addr_t addr);
void CPU_power();
void CPU_run_frame();

#endif /*__m6502_h*/














