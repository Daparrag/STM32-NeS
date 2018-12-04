#include "ppu.h"

PPU nes_ppu;

static inline uint8_t rendering () { return (nes_ppu.mask.bg || nes_ppu.mask.spr); }


/*sprite size : return the size of the sprite. 
 	@param : None;
 	@return: uint8_t size of the sprite : 8x8 = 0  16x16 = 1;  
 */
static inline uint8_t spr_height() { return nes_ppu.ctrl.sprSz ? 1 : 0; }




/*read directly from PPU memory 
 * Access PPU memory 
 * @param: uint16_t addr
 *
 */

static uint8_t rd (uint16_t addr){
	switch(addr){
		case 0x0000 ... 0x1FFF: return Cartridge_chr_access(0, addr, 0);//pattern tables: CHR-ROM/RAM
		case 0x2000 ... 0x3EFF: return nes_ppu.ciRam[]; //FIXEME:Nametables.
		case 0x3F00 ... 0x3FFF: //FIXEME:Palettes:
		default: return 0;
	}

}


/*write directly to PPU memory 
 * Access PPU memory 
 * @param: uint16_t addr
 *
 */
static void wr(uint16_t addr, uint8_t v)
{
	switch(addr){
		case 0x0000 ... 0x1FFF: Cartridge_chr_access(1, addr, v);break;//pattern tables: CHR-ROM/RAM
		case 0x2000 ... 0x3EFF: nes_ppu.ciRam[]=v; break; //FIXEME:Nametables.
		case 0x3F00 ... 0x3FFF: //FIXEME:Palettes:
		break;		
	}
}



/* Calculate graphics addresses */
static inline uint16_t nt_addr() { return 0x2000 | (nes_ppu.vAddr.r & 0xFFF); } //return current nametable address storing bacground patters tiles. 
static inline uint16_t at_addr() { return 0x23C0 | (nes_ppu.vAddr.nt << 10) | ((nes_ppu.vAddr.cY / 4) << 3) | (nes_ppu.vAddr.cX / 4); }
static inline uint16_t bg_addr() { return (nes_ppu.ctrl.bgTbl * 0x1000) + (nes_ppu.nt * 16) + nes_ppu.vAddr.fY; }


/* Put new data into the shift registers */
inline void reload_shift(){
	nes_ppu.bgShiftL = (nes_ppu.bgShiftL & 0xFF00) | nes_ppu.bgL; //feed left 16-bit register 
	nes_ppu.bgShiftH = (nes_ppu.bgShiftH & 0xFF00) | nes_ppu.bgH; //feed high 16-bit register

	nes_ppu.atLatch_fg = (nes_ppu.atLatch_fg & ~(1 << 0) ) | (at & 1); // set atlatchL flag
	nes_ppu.atLatch_fg = (nes_ppu.atLatch_fg & ~(1 << 1) ) | (at & (1 << 1)); // set atlatchH flag

}

/* Increment the scroll by one pixel */
inline void h_scroll() { if (!rendering()) return; if (nes_ppu.vAddr.cX == 31) nes_ppu.vAddr.r ^= 0x41F; else nes_ppu.vAddr.cX++; }


/*memory access : implements ppu_6502 memory access. 
 * uint8_t mode  : 0:read operation 1:write operation
 * uint16_t addr : address to read/write
 * uint8_t v   	 : value to write in memory 
 */
void uint8_t PPU_access(uint8_t wr, uint16_t addr_idx, uint8_t val )
{
	static uint8_t res;		//result of the operation.
	static uint8_t buffer;	//VRAM read buffer.
	static uint8_t latch;   //DEtect Second reading


	if(wr)
	{
		/*write OP*/
		switch (addr_idx)
		{
			case 0: nes_ppu.ctrl.r = val; tAddr.nt = nes_ppu.ctrl.nt; 	break; //PPUCTRL($2000): Description: PPU control register Access: write. 
			case 1:	nes_ppu.mask.r = val; 	break; //PPUMASK($2001): Description: PPU mask register  Access: write.
			case 3:	nes_ppu.oamAddr = val;	break; // OAMADDR($2003): OAM address port Access: write. it is not really used.
			case 4: nes_ppu.oamMem[oamAddr++]= val; break; //OAMDATA($2004):write
			case 5: 
				//PPUSCROLL ($2005) >> write x2 : Description: PPU scrolling position register Access: write twice
			/*
				bit PPUSTATUS 
				; possibly other code goes here
				 lda cam_position_x
 				 sta PPUSCROLL
 				 lda cam_position_y
 				 sta PPUSCROLL
			*/
				if (!latch) { nes_ppu.fX = val & 7; nes_ppu.tAddr.cX = val >> 3; }      // First write.
				else {nes_ppu.tAddr.fY = val & 7; nes_ppu.tAddr.cY = val >> 3;}			  // Second write.
				latch = !latch;
			break;

			case 6:
				//PPUADDR ($2006)  >> write x2 : Description: PPU address register; Access: write twice
				if(!latch) {nes_ppu.tAddr.h = val & 0x3F;} //first write/ high byte 
				else {nes_ppu.tAddr.l = val; nes_ppu.vAddr.r = nes_ppu.tAddr.r;}//second write/lower byte
				latch = !latch;
			break;
			case 7:
			//PPUDATA($2007):Description: PPU data port; Access: read, write
			wr(nes_ppu.vAddr.addr,val);
				nes_ppu.vAddr.addr += nes_ppu.vAddr.addr += nes_ppu.ctrl.incr ? 32 : 1; //increment vRAM address

			break;


		}


	}else
	{
		/*read OP*/
		switch (addr_idx){
			// PPUSTATUS ($2002): Description: PPU status register Access: read
			case 2: res = (res & 0x1F) | nes_ppu.status.r; nes_ppu.vBlank = 0; latch=0;break;
			case 4: res = oamMem[oamAddr]; break; //OAMDATA($2004)

			case 7:
				if(nes_ppu.vAddr.addr <= 0x3EFF){

					res = buffer;	//read internal buffer.
					buffer = rd (nes_ppu.vAddr.addr); 	//fild internal buffer.

				}else 
					{
						res = buffer = rd(nes_ppu.vAddr.addr);

					}
				nes_ppu.vAddr.addr += nes_ppu.vAddr.addr += nes_ppu.ctrl.incr ?32 : 1; 
				break;
		}
	
	}

return res;

}







void scanline_cycle(Scanline s)
{
	static uint16_t addr;

	if(s == NMI && nes_ppu.dot == 1){
		/*
		* set vblack flag on PPUSTATUS register
		*
		*/
		nes_ppu.status.vBlank = TRUE;


	}else if (s == POST && nes_ppu.dot ==0 ){
		/*here is where we plot the pixel on the LCD*/
	}else if ((s == VISIBLE) || (s == PRE)){

		//sprites
		switch (nes_ppu.dot)
		{
			case 1: clear_oam(); if(s == PRE){nes_ppu.status.sprOvf = nes_ppu.status.sprHit = FALSE;}				break;
			case 257:			break;
			case 321: load_sprites(); break;

		}
		// background
		switch (nes_ppu.dot)
		{

			case 2 ... 255: case 322 ... 337:
			pixel();
			switch (nes_ppu.dot % 8)
			{
				//Nametable
				case 1: addr = nt_addr(); reload_shift(); break;
				case 2: nes_ppu.nt = rd(addr); break;

				//Atribute:
				case 3: addr = at_addr(); break;
				case 4: nes_ppu.at = rd(addr); break;

				// Background (low bits):
				case 5:  addr  = bg_addr(); break;
				case 6: nes_ppu.bgL = rd(addr);
				// Background (high bits):
				case 7: addr += 8;         break;
				case 0:  bgH   = rd(addr); h_scroll(); break;


			}break;

			//not shit reloading 
			case 			 1: addr = nt_addr();if(s== PRE){nes_ppu.status.vBlank = FALSE;}break;
			

			case           340:  nt = rd(addr); if (s == PRE && rendering() && frameOdd) dot++;

		}

	}
	


}
