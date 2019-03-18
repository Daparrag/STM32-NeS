#include "CPU_6502.h"
#include "cartridge.h"
#include "ppu.h"
#include "memlib.h"


M6502 cpu6502;

#define A  cpu6502.registers.a
#define X  cpu6502.registers.x
#define Y  cpu6502.registers.y
#define S  cpu6502.registers.s
#define P  cpu6502.registers.p
#define PC cpu6502.registers.pc
#define NMI_IRQ	cpu6502.flags
#define TICK   tick()
inline void tick(){PPU_step();PPU_step();PPU_step(); cpu6502.remainingCycles--;};

#define TOTAL_CYCLES  29781


                               /* 6502 status flags:         */
enum {
  flagN= (1<<7),	/* 1: Result is negative      */
  flagV= (1<<6),	/* 1: Overflow occured        */
  flagX= (1<<5),	/* unused   	 */
  flagB= (1<<4),	/* Break [0 on stk after int] */
  flagD= (1<<3),	/* 1: Decimal mode            */
  flagI= (1<<2),	/* 1: Interrupts disabled     */
  flagZ= (1<<1),	/* 1: Result is zero          */
  flagC= (1<<0)		/* 1: Carry occured           */
};

#define getN()	(P & flagN) 	/* get flagN      */
#define getV()	(P & flagV)		/* get flagV      */
#define getB()	(P & flagB)		/* get flagB      */
#define getD()	(P & flagD)		/* get flagD      */
#define getI()	(P & flagI)		/* get flagI      */
#define getZ()	(P & flagZ)		/* get flagZ      */
#define getC()	(P & flagC)		/* get flagC      */


#define irq_flag (1 << 1)
#define nmi_flag (1 << 0)



#define setNVZC(N,V,Z,C)	(P= (P & ~(flagN | flagV | flagZ | flagC)) | (N) | ((V)<<6) | ((Z)<<1) | (C)) /* Set FlagN, flagV, flagZ, flagC on register */
#define setNVZ(N,Z,V)		(P= (P & ~(flagN | flagV | flagZ 		)) | (N) | ((V)<<6) | ((Z)<<1)		) /* Set flagN, flagZ, flagC on Status register */
#define setNZC(N,Z,C)		(P= (P & ~(flagN |         flagZ | flagC)) | (N) |            ((Z)<<1) | (C)) /* Set flagN, flagZ, flagC on Status register */
//#define setNZ(N,Z)			(P= (P & ~(flagN |             flagZ    )) | (N) |            ((Z)<<1)      ) /* Set flagN, flagZ        on Status register */
#define setCV(C,V)    (P= (P& ~ (   		   flagV  |		   flagC ))|  	   ((V)<<6)			   | (C))
#define setZ(Z)			(P= (P & ~(                flagZ        )) |                  	  ((Z)<<1)      )	  /* Set flagZ,          on Status register */
#define setC(C)			(P= (P & ~(                        flagC)) |                             	 (C))	  /* Set flagC,          on Status register */
#define setV(V)     (P= (P & ~(                        flagV)) |               ((V)<<6)         )



inline void  setnz(uint8_t n, uint8_t z){

	(P = (P & ~(flagN | flagZ )) | (n) | ((z)<<1)); /* Set flagN, flagZ        on Status register */
}


#define NAND(P, Q)	(!((P) & (Q)))

inline void set_nmi(uint8_t v);
inline void set_irq(uint8_t v);

inline void dma_oam(uint8_t bank);

// Does adding I to A cross a page?
inline uint8_t cross(uint16_t a, uint8_t i) { return ((a+i) & 0xFF00) != ((a & 0xFF00)); }

/*memory access : implements cpu_6502 memory access. 
 * uint8_t mode  : 0:read operation 1:write operation
 * uint16_t addr : address to read/write
 * uint8_t v   	 : value to write in memory 
 */
inline uint8_t access (uint8_t mode, uint16_t addr, uint8_t v)
{
	uint8_t * r;

	switch (addr){

		case 0x0000 ... 0x1FFF: r = &cpu6502.ram[addr % 0x800]; if (mode) *r= v; return *r;    //RAM
		case 0x2000 ... 0x3FFF: return PPU_access (mode, addr % 8, v); /*PPU:
																		0x2000-0x2001 : PPU-ctrl-register 1 ; PPU-ctrl-register 2;
		 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	*/

		//APU
		case 0x4000 ... 0x4013:
		//case 			0x4015:	return APU_access (eleapsed(), mode, addr, v); /*Fixme Audio*/
		//case 			0x4017: if (mode) return APU_access (eleapsed(), mode, addr, v);
		//						else return Joipad_read_state(1);

		case 			0x4014: if(mode) dma_oam(v); break;	//direct memory access for PPU   // OAM DMA.           0x4014:  if (wr) dma_oam(v); break;
		//case 			0x4016: if(mode) {Joypad_write_strobe(v & 1); break;}
		//						else return Joipad_read_state(0); /*Fixme joypad*/

		case 0x4018 ... 0xFFFF: return Cartridge_access(mode, addr, v);


	}

 return 0;

}
/*optimazing write/read fuctions*/
inline uint8_t  wr(uint16_t a, uint8_t v)      { TICK; return access(1, a, v);   }
inline uint8_t  rd(uint16_t a)            	  { TICK; return access(0, a, 0);      }
inline uint16_t rd16_d(uint16_t a, uint16_t b) { return rd(a) | (rd(b) << 8); }  // Read from A and B and merge.
inline uint16_t rd16(uint16_t a)          	  { return rd16_d(a, a+1);       }
inline uint8_t  push(uint8_t v)           	  { return wr(0x100 + (S--), v); }
inline uint8_t  pop()                		  { return rd(0x100 + (++S));    }
inline void dma_oam(uint8_t bank) { int i = 0; for (i = 0; i < 256; i++)  wr(0x2014, rd(bank*0x100 + i)); }


/* Addressing modes */

inline uint16_t imm()   { return PC++;                                       }
inline uint16_t imm16() { PC += 2; return PC - 2;                            }
inline uint16_t _abs()   { return rd16(imm16());                              }
inline uint16_t _abx()  { TICK; return _abs() + X;                               }  // Exception.
inline uint16_t abx()   { uint16_t a = _abs(); if (cross(a, X)) TICK; return a + X;   }
inline uint16_t aby()   { uint16_t a = _abs(); if (cross(a, Y)) TICK; return a + Y;   }
inline uint16_t zp()    { return rd(imm());                                  }
inline uint16_t zpx()   { TICK; return (zp() + X) % 0x100;                      }
inline uint16_t zpy()   { TICK; return (zp() + Y) % 0x100;                      }
inline uint16_t izx()   { uint8_t i = zpx(); return rd16_d(i, (i+1) % 0x100);     }
inline uint16_t _izy()  { uint8_t i = zp();  return rd16_d(i, (i+1) % 0x100) + Y; }  // Exception.
inline uint16_t izy()   { uint16_t a = _izy(); if (cross(a-Y, Y)) TICK; return a;    }

/*STx*/
inline void st(uint8_t  r, Mode m) {
										uint8_t a =  m();
										wr( a , r );
										}
inline void st_izy(){  wr(_izy(), A ); }	// Exceptions.
inline void st_abx(){  wr(_abs() + X, A ); }	// ...
inline void st_aby(){  wr(_abs() + Y, A ); } // ...


/* LD */
inline void ld(uint8_t * r, Mode adrmode) {

	uint16_t a = adrmode(); uint8_t p = rd(a);
	*r = p;
	setnz( ((*r) & 0x80), ((*r) == 0));
}

inline void cmp (uint8_t *r, Mode adrmode){

	uint16_t a = adrmode(); uint8_t p = rd(a);
	setnz( ((*r - p) & 0x80), ((*r - p) == 0));
	setC((*r >= p));
}


/* Arithmetic and bitwise */
inline void adc(Mode adrmode)									
	{													
		uint16_t a = adrmode(); uint8_t p = rd(a);			
		int16_t r = A + p + getC();							
/*update flags*/										
		setCV((r > 0xFF), (( ~(A^p) ) & (A^r)) & 0x80  );	
		A = r;												
		setnz((A & 0x80),(A == 0));
	}													


inline void sbc(Mode adrmode)
	{					
		uint16_t a = adrmode(); uint8_t p = rd(a) ^ 0xFF;		
		int16_t r = A + p + getC();								
		setCV((r > 0xFF), (( ~(A^p) ) & (A^r)) & 0x80  );	
		A = r;												
		setnz((A & 0x80),(A == 0));
	}														

inline void bit(Mode adrmode)
	{
		uint16_t a = adrmode(); uint8_t p = rd(a);
		setNVZ((p & 0x80),(p & 0x40),(!(A & p)));
	}															

inline void and(Mode adrmode)
		{
			uint16_t a = adrmode(); uint8_t p = rd(a);			
			A &= p;												
			setnz( ( A  & 0x80),( A == 0 ) );
		}														

inline void eor(Mode adrmode)
		{					
			uint16_t a = adrmode(); uint8_t p = rd(a);			
			A ^= p;									
			setnz( (A  & 0x80),( A  == 0 ) );
		}														

inline void ora(Mode adrmode)
		{
			uint16_t a = adrmode(); uint8_t p = rd(a);			
			A |= p;											
			setnz( ( A & 0x80),( A  == 0 ) );
		}


/*read-Modify-write*/
inline void asl(Mode adrmode)												
		{
			uint16_t a = adrmode(); uint8_t p = rd(a);
			setC(p & 0x80);											
			TICK;													
			setnz((wr(a, (p << 1) ) & 0x80),(wr(a, (p << 1)) == 0));
		}

inline void lsr(Mode adrmode)
		{	
			uint16_t a = adrmode(); uint8_t p = rd(a);
			setC(p & 0x01);							
			TICK;									
			setnz((wr(a, (p << 1) ) & 0x80),(wr(a, (p << 1)) == 0));
		}

inline void rol(Mode adrmode)
		{			
			uint16_t a = adrmode(); uint8_t p = rd(a);				
			uint8_t c = getC();										
			setC(p & 0x80);											
			TICK;													
			setnz( (wr(a, (p << 1)  | c ) & 0x80 ) , (wr(a, (p << 1)  | c ) == 0 ) );
		}

inline void ror(Mode adrmode)
		{					
			uint16_t a = adrmode(); uint8_t p = rd(a);				
			uint8_t c = (getC() << 7 );								
			setC(p & 0x01);											
			TICK;													
			setnz( (wr(a, (p >> 1)  | c ) & 0x80 ) , (wr(a, (p >> 1)  | c ) == 0 ) );
		}


inline void DEC(Mode adrmode)
		{					
			uint16_t a = adrmode(); uint8_t p = rd(a);				
			TICK;
			--p;
			setnz( (wr(a, p ) & 0x80 ) , (wr(a, p ) == 0 ) );
		}


inline void INC(Mode adrmode)												
		{
			uint16_t a = adrmode(); uint8_t p = rd(a);				
			TICK;
			++p;
			setnz( (wr(a, p ) & 0x80 ) , (wr(a, p ) == 0 ) );
		}

/*DEx INx */
inline void dec(uint8_t r)
		{		
			--r;
			setnz( ((r)  & 0x80 ), ((r)  == 0 ) );
			TICK;													
		}


inline void inc(uint8_t r)
		{
			++r;
			setnz( ((r)  & 0x80 ), ((r)  == 0 ) );
			TICK;													
		}

/* Bit shifting on the accumulator */

inline void ASL_A()
				{	
					setC(A & 0X80);	
					A <<= 1;	
					setnz( (A & 0x80), ( A  == 0 ) );
					TICK;							

				}

inline void LSR_A()	
				{	
					setC(A & 0X01);	
					A >>= 1;		
					setnz( ( A   & 0x80), ( A == 0 ) );
					TICK;								
				}


inline void ROL_A()
				{	
					uint8_t c = getC();
					setC(A & 0x80);		
					A = ((A << 1) | c);	
					setnz( (A & 0x80), (A == 0) );
					TICK;							
				}


inline void ROR_A()
				{	
					uint8_t c = (getC() << 7);	
					setC(A & 0x01);		
					A = (c | (A >> 1) );	
					setnz( (A & 0x80), (A == 0) );
					TICK;						
												
				}

/**Txx moving values between registers */
inline void tr(uint8_t s, uint8_t d)
				{				
					d = s; 		
					setnz((d & 0x80),( d == 0));
					TICK;										
				}
/*FIXME: TX exeption*/
inline void  tr_excep()	{ S = X; 	TICK; }

/*stack operations*/
inline void PLP()
				{
					TICK;
					TICK;
					P = pop();
				}


inline void PHP()	
				{
					TICK;										
					push( P | (1 << 4) );	/* B flag set*/		
				}


inline void PLA()
				{
					TICK;
					TICK;
					A=pop();
					setnz((A & 0x80),( A == 0));
				}

inline void PHA()	{ TICK; push(A); }

/*flow control branches and jumps*/
inline void  br(uint8_t f , uint8_t v)
		{
			int8_t j = rd(imm());
			if((P & f) == v)	
			{					
				TICK;
				PC+=j;
			}					
		}

inline void JMP_IND()												
		{														
			uint16_t i = rd16(imm16());							
			PC = rd16_d(i, (i&0xFF00) | ((i+1) % 0x100));		
		}

inline void JMP() { PC = rd16(imm16()); }

inline void JSR()
		{
			uint16_t t = PC+1;
			TICK;			
			push(t >> 8);	
			push(t);		
			PC = rd16(imm16());
		}

/*Return instructions*/
inline void  RTS() {TICK; TICK; PC = (pop() |  pop() << 8) + 1; TICK; }
inline void   RTI() {PLP(); PC = (pop() |  pop() << 8); }
inline void  flag(uint8_t f, uint8_t v ) { P = (P & ~( 1 << f)) | (v << f);  TICK; }

inline void  INT(uint8_t t)
		{
			TICK;
			if (t != INT_BRK) TICK;
			if (t != INT_RESET)
			{
				push(PC >> 8); push(PC & 0xFF);					
				push(P | ((t == INT_BRK) << 4)); /*set B if INT_BRK*/				
			}				
			else{			
					S-=3;	
					TICK; TICK; TICK;
			}									
			P = (P  & ~(flagI)  ) | (1 << 2);	
									/*   NMI    Reset    IRQ     BRK  */
			const uint16_t vect[] = { CPU6502_NMIVector, CPUM6502_RSTVector, CPUM6502_IRQVector, CPUM6502_IRQVector };
			PC = rd16(vect[t]);			
			if(t == INT_NMI) set_nmi(0);
		}

inline void NOP() {TICK;}


/* Execute a CPU instruction */
static void exec(){

	switch ( rd(PC++) ) // Fetch the opcode.
	{
		// Select the right function to emulate the instruction:
		case 0x00: return INT(INT_BRK)	; case 0x01: return ora(izx)	;
		case 0x05: return ora(zp)		; case 0x06: return asl(zp)		;
		case 0x08: return PHP()			; case 0x09: return ora(imm)	;
		case 0x0A: return ASL_A()		; case 0x0D: return ora(_abs)	;
		case 0x0E: return asl(_abs)		; case 0x10: return br(flagN,0) ;
		case 0x11: return ora(izy)		; case 0x15: return ora(zpx)	;
		case 0x16: return asl(zpx)		; case 0x18: return flag(0, 0b1);
		case 0x19: return ora(aby)  	; case 0x1D: return ora(abx)  	;
		case 0x1E: return asl(_abx) 	; case 0x20: return JSR()      	;
		case 0x21: return and(izx)  	; case 0x24: return bit(zp)   	;
		case 0x25: return and(zp)   	; case 0x26: return rol(zp)   	;
		case 0x28: return PLP()       	; case 0x29: return and(imm)  	;
		case 0x2A: return ROL_A()     	; case 0x2C: return bit(_abs)   	;
		case 0x2D: return and(_abs)  	; case 0x2E: return rol(_abs) 	;
		case 0x30: return br(flagN,1)   ; case 0x31: return and(izy)  	;
		case 0x35: return and(zpx)  	; case 0x36: return rol(zpx)	;
		case 0x38: return flag(0,0b1) 	; case 0x39: return and(aby)	;
        case 0x3D: return and(abx)  	; case 0x3E: return rol(_abx) 	;
        case 0x40: return RTI()       	; case 0x41: return eor(izx)  	;
        case 0x45: return eor(zp)   	; case 0x46: return lsr(zp)   	;
        case 0x48: return PHA()       	; case 0x49: return eor(imm)  	;
        case 0x4A: return LSR_A()     	; case 0x4C: return JMP()       ;
        case 0x4D: return eor(_abs)  	; case 0x4E: return lsr(_abs)  	;
        case 0x50: return br(flagV,0)   ; case 0x51: return eor(izy)  	;
        case 0x55: return eor(zpx)  	; case 0x56: return lsr(zpx)  	;
        case 0x58: return flag(2,0) 	; case 0x59: return eor(aby)  	;
        case 0x5D: return eor(abx)  	; case 0x5E: return lsr(_abx) 	;
        case 0x60: return RTS()       	; case 0x61: return adc(izx)	;
        case 0x65: return adc(zp)   	; case 0x66: return ror(zp)   	;
        case 0x68: return PLA()       	; case 0x69: return adc(imm)  	;
        case 0x6A: return ROR_A()     	; case 0x6C: return JMP_IND()  	;
        case 0x6D: return adc(_abs)  	; case 0x6E: return ror(_abs)  	;
        case 0x70: return br(flagV,1)   ; case 0x71: return adc(izy)  	;
        case 0x75: return adc(zpx)  	; case 0x76: return ror(zpx)  	;
        case 0x78: return flag(2,1) 	; case 0x79: return adc(aby)  	;
        case 0x7D: return adc(abx)  	; case 0x7E: return ror(_abx) 	;
        case 0x81: return st(A,izx) 	; case 0x84: return st(Y,zp)  	;
        case 0x85: return st(A,zp)  	; case 0x86: return st(X,zp) 	;
        case 0x88: return dec(Y)    	; case 0x8A: return tr(X,A)	   	;
        case 0x8C: return st(Y,_abs) 	; case 0x8D: return st(A,_abs) 	;
        case 0x8E: return st(X,_abs) 	; case 0x90: return br(flagC,0) ;
        case 0x91: return st_izy() 		; case 0x94: return st(Y,zpx) 	;
        case 0x95: return st(A,zpx) 	; case 0x96: return st(X,zpy) 	;
        case 0x98: return tr(Y,A)   	; case 0x99: return st_aby() 	;
        case 0x9A: return tr_excep()   	; case 0x9D: return st_abx() 	;
        case 0xA0: return ld(&Y,imm) 	; case 0xA1: return ld(&A,izx) 	;
        case 0xA2: return ld(&X,imm) 	; case 0xA4: return ld(&Y,zp)  	;
        case 0xA5: return ld(&A,zp)  	; case 0xA6: return ld(&X,zp)  	;
        case 0xA8: return tr(A,Y)   	; case 0xA9: return ld(&A,imm) 	;
        case 0xAA: return tr(A,X)   	; case 0xAC: return ld(&Y,_abs) 	;
        case 0xAD: return ld(&A,_abs) 	; case 0xAE: return ld(&X,_abs) 	;
        case 0xB0: return br(flagC,1)   ; case 0xB1: return ld(&A,izy) 	;
        case 0xB4: return ld(&Y,zpx) 	; case 0xB5: return ld(&A,zpx) 	;
        case 0xB6: return ld(&X,zpy) 	; case 0xB8: return flag(6,0) 	;
        case 0xB9: return ld(&A,aby) 	; case 0xBA: return tr(S,X)  	;
        case 0xBC: return ld(&Y,abx) 	; case 0xBD: return ld(&A,abx) 	;
        case 0xBE: return ld(&X,aby) 	; case 0xC0: return cmp(&Y,imm)	;
        case 0xC1: return cmp(&A,izx)	; case 0xC4: return cmp(&Y,zp) 	;
        case 0xC5: return cmp(&A,zp) 	; case 0xC6: return DEC(zp)		;
        case 0xC8: return inc(Y)    	; case 0xC9: return cmp(&A,imm)	;
        case 0xCA: return dec(X)    	; case 0xCC: return cmp(&Y,_abs)	;
        case 0xCD: return cmp(&A,_abs)	; case 0xCE: return DEC(_abs)  	;
        case 0xD0: return br(flagZ,0)   ; case 0xD1: return cmp(&A,izy)	;
        case 0xD5: return cmp(&A,zpx)	; case 0xD6: return DEC(zpx)  	;
        case 0xD8: return flag(3,0) 	; case 0xD9: return cmp(&A,aby)	;
        case 0xDD: return cmp(&A,abx)	; case 0xDE: return DEC(_abx) 	;
        case 0xE0: return cmp(&X,imm)	; case 0xE1: return sbc(izx)  	;
        case 0xE4: return cmp(&X,zp) 	; case 0xE5: return sbc(zp)   	;
        case 0xE6: return INC(zp)   	; case 0xE8: return inc(X)    	;
        case 0xE9: return sbc(imm)  	; case 0xEA: return NOP()       ;
        case 0xEC: return cmp(&X,_abs)	; case 0xED: return sbc(_abs)  	;
        case 0xEE: return INC(_abs)  	; case 0xF0: return br(flagZ,1)	;
        case 0xF1: return sbc(izy)  	; case 0xF5: return sbc(zpx)  	;
        case 0xF6: return INC(zpx) 		; case 0xF8: return flag(3,1) 	;
        case 0xF9: return sbc(aby)  	; case 0xFD: return sbc(abx)  	;
        case 0xFE: return INC(_abx) 	;
        default:
        {
        	/* Invalid OPcode! PC */
        	return NOP();
        }
	}
} 

inline void set_nmi(uint8_t v){

	NMI_IRQ = (NMI_IRQ & ~ ( nmi_flag )) | (v);
}

inline void set_irq(uint8_t v){
	(NMI_IRQ = (NMI_IRQ & ~ ( irq_flag )) | (v << 1));
}

inline uint8_t get_nmi(){
	return (NMI_IRQ & ~ ( nmi_flag ));

}

inline uint8_t get_irq(){
	return (NMI_IRQ & ~ ( irq_flag ));

}



int dmc_read(void* ptr , cpu_addr_t addr) { uint8_t ret=access(0, addr, 0); return ret; }

/* Turn on the CPU */
void CPU_power()
{
	cpu6502.remainingCycles+=TOTAL_CYCLES;
	P = 0x04;
	A = X = Y = S = 0x00;
	memset(cpu6502.ram,0xFF,sizeof(cpu6502.ram));
	set_nmi(0);
	set_irq(0);
	INT(INT_RESET);


}

/* Run the CPU for roughly a frame */
void run_frame()
{
	cpu6502.remainingCycles+= TOTAL_CYCLES;
	while (cpu6502.remainingCycles > 0){
		if (get_nmi())INT(INT_NMI);
		else if (get_irq() && !getI()) INT(INT_IRQ); 
		exec();
	}
}

void  M6502_set_nmi(void){

	set_nmi(TRUE);

}
		
