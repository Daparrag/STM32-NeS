#include "ppu.h"
#include "palette.h"
PPU nes_ppu;
uint32_t pixels[256 * 240];     // Video buffer


static void eval_sprites();
static void clear_oam();
static uint16_t nt_mirror(uint16_t addr );
static void set_mirroring(Mirroring mode);

inline uint8_t rendering () { return (nes_ppu.mask.bg || nes_ppu.mask.spr); }


/*sprite size : return the size of the sprite. 
 	@param : None;
 	@return: uint8_t size of the sprite : 8x8 = 0  16x16 = 1;  
 */
inline uint8_t spr_height() { return nes_ppu.ctrl.sprSz ? 16 : 8; }


void set_mirroring(Mirroring mode) { nes_ppu.mirroring = mode; }



//GET CIRAM Address according to mirror
/*
 * * (0,0)     (256,0)     (511,0)
       +-----------+-----------+
       |           |           |
       |           |           |
       |   $2000   |   $2400   |
       |           |           |
       |           |           |
(0,240)+-----------+-----------+(511,240)
       |           |           |
       |           |           |
       |   $2800   |   $2C00   |
       |           |           |
       |           |           |
       +-----------+-----------+
     (0,479)   (256,479)   (511,479)
 *
 */
static uint16_t nt_mirror(uint16_t addr )
{
	switch(nes_ppu.mirroring){

		case VERTICAL: 	 return addr % 0x800;   						// VERTICAL mirroring: (Nametable 0 = Nametable 1) & (Nametable 2 = Nametable 3)
																		// mapping (0x2000-0x23BF and 0x2800-0x2BBF) to ciRam[0x000-0x3FF]
											    						// and 	  (0x2400-0x27FB and 0x2C00-0x2FBF) to ciRam[0x400-0x7FF]

		case HORIZONTAL: return ((addr >> 1) & 0x400) + (addr % 0x400); //HORIZONTAL mirroring: (Nametable 0 = Nametable 2) & (Nametable 1 = Nametable 3)
																		// mapping (0x2000-0x23BF and 0x2400-0x27FB) to ciRam[0x000-0x3FF]
																		// and 	  (0x2800-0x2BBF and 0x2C00-0x2FBF) to ciRam[0x400-0x7FF]
		/*Fixme: we can try more mirroring in this section*/
		default: 		 return addr - 0x2000;

	}

}


/*read directly from PPU memory 
 * Access PPU memory 
 * @param: uint16_t addr
 *
 * 0x0000 - 0x0FFF Cartridge RAM/ROM pattern TABLE  0.
 * 0x1000 - 0x1FFF Cartridge RAM/ROM pattern TABLE  1.
 * 0x2000 - 0x23BF Nametable 0.
 * 0x23C0 - 0x23FF Attribute Table 0.
 * 0x2400 - 0x27FB Nametable 1.
 * 0x27C0 - 0x27FF Attribute table 1.
 * 0x2800 - 0x2BBF Nametable 2.
 * 0x2BC0 - 0x2BFF Attribute table 2.
 * 0x2C00 - 0x2FBF Nametable 3.
 * 0x2FC0 - 0x2FFF Attribute table 3.
 * 0x3000 - 0x3EFF --Mirroring.
 * 0x3F00 - 0x3F0F Background Palette.
 * 0x3F10 - 0x3F1F Sprite Palette.
 * 3x3F20 - 0x3FFF --Mirroring.
 *
 */



static uint8_t rd (uint16_t addr){
	switch(addr){
		case 0x0000 ... 0x1FFF: return Cartridge_chr_access(0, addr, 0);//pattern tables: CHR-ROM/RAM
		case 0x2000 ... 0x3EFF: return nes_ppu.ciRam[nt_mirror(addr)];  // return the name-table address based on the mirroring
		case 0x3F00 ... 0x3FFF: //Fixme:access to the background and sprites palette:
		if((addr & 0x10) == 0x13) addr = (addr & ~0x10);    //checking for mirrors
			return nes_ppu.cgRam[addr & 0x1F] & (nes_ppu.mask.gray ? 0x30 : 0xFF);

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
		case 0x2000 ... 0x3EFF: nes_ppu.ciRam[nt_mirror(addr)]=v; break; // write to name-table address based on the mirroring
		case 0x3F00 ... 0x3FFF:
		if((addr & 0x10) == 0x13) addr = (addr & ~0x10);    //checking for mirrors
		nes_ppu.cgRam[addr & 0x1F] = v;
		break;		
	}
}



/* Calculate graphics addresses */
static inline uint16_t nt_addr() { return 0x2000 | (nes_ppu.vAddr.r & 0xFFF); } //return current nametable address storing background patters tiles from 0x2000-0x2FBF. 
static inline uint16_t at_addr() { return 0x23C0 | (nes_ppu.vAddr.nt << 10) | ((nes_ppu.vAddr.cY / 4) << 3) | (nes_ppu.vAddr.cX / 4); }/*return the corresponding attribute table entry from 0x23C0-0x2FFF*/
static inline uint16_t bg_addr() { return (nes_ppu.ctrl.bgTbl * 0x1000) + (nes_ppu.nt * 16) + nes_ppu.vAddr.fY; } //Fetch the lower order byte of an 8x1 pixel silver of pattern table from (0x0000-0xFF7) or (0x1000-0x1FF7)


/* Put new data into the shift registers */
/*The bitmap data for the next tile is loaded into the upper 8-bits of the shift register */
inline void reload_shift(){
	//each 8 cycles data fetched form cycles 1-256 allocated into internal latches is copy to an appropriate shift register
	nes_ppu.bgShiftL = (nes_ppu.bgShiftL & 0xFF00) | nes_ppu.bgL; //feed left 16-bit register 
	nes_ppu.bgShiftH = (nes_ppu.bgShiftH & 0xFF00) | nes_ppu.bgH; //feed high 16-bit register

	nes_ppu.atLatch_fg = (nes_ppu.atLatch_fg & ~(1 << 0) ) | (nes_ppu.at & 1); // set atlatchL flag
	nes_ppu.atLatch_fg = (nes_ppu.atLatch_fg & ~(1 << 1) ) | (nes_ppu.at & (1 << 1)); // set atlatchH flag

}

/* Increment the scroll by one pixel  horizontal
 *
 * LoopyT does NOTHING... aside from reload LoopyV at certain times.
 * If you want to actually affect where the PPU draws from (ie, change the scrolling)... or change the address
 * so you can write to $2007... LoopyV is what you need to change.
 *
 * */
inline void h_scroll() { if (!rendering()) return; if (nes_ppu.vAddr.cX == 31) nes_ppu.vAddr.r ^= 0x41F; else nes_ppu.vAddr.cX++; }


/* Increment the scroll by one pixel  horizontal or X scroll
 *
 * Fine Y scroll here(fY) has 3 bits (12, 13, 14)
 * when it wraps at 8 then the coarse Y (cY)is incremented, changing the nametable byte to be fetched
 * to the next row of tiles. it is:   0000, 1000, 2000 .... 6000, 7000, 0020, 1020 ... 7020, 0040
 *
 *
 * Since nametables aren't as tall as they are wide,
 * coarse Y scrolling is cut off after 29 ($3A0), rather than 31 ($3E0). It is : 0380, 03A0, 0800, 0820 ... 0B80, 0BA0, 0000, 0020
 *
 * If you manage to set the coarse Y scrolling manually so that it's above 29, it will still wrap at 31
 * however the nametable (bit 11) will not be changed:
 * 03C0, 03E0, 0000, 0020 ... 0380, 03A0, 0800 ... etc
 * */
inline void v_scroll()
{
	if( !rendering() ) return;
	if (nes_ppu.vAddr.fY < 7) nes_ppu.vAddr.fY++;
	else
	{

		nes_ppu.vAddr.fY = 0;

		if(nes_ppu.vAddr.cY == 0x3E0) nes_ppu.vAddr.cY=0; //cY cut at 31 seting manually cY above 29
		else if (nes_ppu.vAddr.cY == 0x3A0){ nes_ppu.vAddr.cY = 0; nes_ppu.vAddr.nt ^= 0b10; }//cY cut at 29;
		else 							 nes_ppu.vAddr.cY++;

	}


}

/* copy scrolling data from loppy T to loopy V */
inline void h_update() { if(!rendering())return; nes_ppu.vAddr.r = (nes_ppu.vAddr.r & ~0x041F) | (nes_ppu.tAddr.r & 0x041F); }
inline void v_update() { if(!rendering())return; nes_ppu.vAddr.r = (nes_ppu.vAddr.r & ~0x7BE0) | (nes_ppu.tAddr.r & 0x7BE0); }


/* Clear Secondary OAM */
static void clear_oam()
{
	int i=0;

	for(i=0; i < 8; i++ )
	{
		nes_ppu.secOam[i].id 	= 64;
		nes_ppu.secOam[i].y  	= 0xFF;
		nes_ppu.secOam[i].tile	= 0xFF;
		nes_ppu.secOam[i].attr 	= 0xFF;
		nes_ppu.secOam[i].x 	= 0xFF;
		nes_ppu.secOam[i].dataL = 0;
		nes_ppu.secOam[i].dataH	= 0;
	}
}



/*Fill secondary OAM with the sprite infos for the next scanline*/
/* This operation is done by the PPU once each scanline it prepares the set of sprites and 
 * fetches their data to be rendered  on the next sacanline.
	
	1. clear the list of sprites to draw
	2. read through OAM, checking which sprites will be on this scanline. it shous the 
	   first eight it finds that do.
	3. if eight sprites wer found, then check for futher sprites on the scanline 
	   to see if the sprite overflow flag shoud be set
	4. Use the details for the eight or fewer sprites chosen, it determines which pixel each has on the scanline and where
	   to draw them

*/
static void eval_sprites()
{
	uint8_t i=0;
	int n=0;

	for (i=0; i< 64; i ++){ //maximum number of sprites x screen 

		int line = (nes_ppu.scanline == 261 ? -1 : nes_ppu.scanline) - nes_ppu.oamMem[i*4 + 0];

		/*if the sprite is in the scanline, Copy its properties into secundary OAM:*/
		if((line > 0) && (line < spr_height()))
		{
			nes_ppu.secOam[n].id 	= i;
			nes_ppu.secOam[n].y 	= nes_ppu.oamMem[i*4 + 0];
			nes_ppu.secOam[n].tile 	= nes_ppu.oamMem[i*4 + 1];
			nes_ppu.secOam[n].attr 	= nes_ppu.oamMem[i*4 + 2];
			nes_ppu.secOam[n].x 	= nes_ppu.oamMem[i*4 + 3];
			
			if(++n > 8){
				// if  more than 8 sprites appear, those that appear latter in memory 
				// simply would be render.
				nes_ppu.status.sprOvf=  TRUE;
			}


		}


	}

}

/* Load the sprite info into primary OAM and fetch their tile data. */
void load_sprites()
{
	uint16_t addr;
	uint8_t i;


	for(i=0; i < 8; i++){
		nes_ppu.oam[i]=nes_ppu.secOam[i];


	/* Different address modes depending on the sprite height:*/
		if(spr_height()==16)
			addr=((nes_ppu.oam[i].tile & 1) * 0x1000) + ((nes_ppu.oam[i].tile & ~1) * 16);
		else
			addr = ( nes_ppu.ctrl.sprTbl      * 0x1000) + ( nes_ppu.oam[i].tile       * 16);

		unsigned int sprY = (nes_ppu.scanline - nes_ppu.oam[i].y) % spr_height();  // Line inside the sprite.

		if(nes_ppu.oam[i].attr & 0x80) sprY ^= spr_height() - 1; //Vertical flip

		addr +=  sprY + (sprY & 8); // Select the second tile if on 8x16.

		nes_ppu.oam[i].dataL = rd(addr + 0);
        nes_ppu.oam[i].dataH = rd(addr + 8);

	}

}




/* Process a pixel, draw it if it's on screen */
void pixel()
{
	
	
}




/*memory access : implements ppu_6502 memory access. 
 * uint8_t mode  : 0:read operation 1:write operation
 * uint16_t addr : address to read/write
 * uint8_t v   	 : value to write in memory 
 */
uint8_t PPU_access(uint8_t wop, uint16_t addr_idx, uint8_t val)
{
	static uint8_t res;		//result of the operation.
	static uint8_t buffer;	//VRAM read buffer.
	static uint8_t latch;   //Detect Second reading


	if(wop)
	{
		/*write OP*/
		switch (addr_idx)
		{
			case 0: nes_ppu.ctrl.r = val; nes_ppu.tAddr.nt = nes_ppu.ctrl.nt; 	break; //PPUCTRL($2000): Description: PPU control register Access: write.
			case 1:	nes_ppu.mask.r = val; 	break; //PPUMASK($2001): Description: PPU mask register  Access: write.
			case 3:	nes_ppu.oamAddr = val;	break; // OAMADDR($2003): OAM address port Access: write. it is not really used.
			case 4: nes_ppu.oamMem[nes_ppu.oamAddr++]= val; break; //OAMDATA($2004):write
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
			wr(nes_ppu.vAddr.addr , val);
				nes_ppu.vAddr.addr += nes_ppu.ctrl.incr ? 32 : 1; //increment vRAM address
		}


	}
	else
	{
		/*read OP*/
		switch (addr_idx){
			// PPUSTATUS ($2002): Description: PPU status register Access: read
			case 2: res = (res & 0x1F) | nes_ppu.status.r; nes_ppu.status.vBlank = FALSE; latch=0;break;
			case 4: res = nes_ppu.oamMem[nes_ppu.oamAddr]; break; //OAMDATA($2004)

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
	//Scanline: 262 scanlines per frame. each scanline lasts for 341 PPU clock - cycles
	static uint16_t addr;

	if(s == NMI && nes_ppu.dot == 1){
		/*
		* set vblack flag on PPUSTATUS register
		*
		*/
		nes_ppu.status.vBlank = TRUE;

		/*set nmi in the CPU*/
		M6502_set_nmi();


	}else if (s == POST && nes_ppu.dot ==0 ){
		/*here is where we plot the pixel on the LCD*/
		LCD_NES_new_frame(pixels);

	}else if ((s == VISIBLE) || (s == PRE)){

		//sprites 0-341 pixels per scanline
		switch (nes_ppu.dot)
		{
			case 1: clear_oam(); if(s == PRE){nes_ppu.status.sprOvf = nes_ppu.status.sprHit = FALSE;}				break;
			case 257: eval_sprites();	break;
			case 321: load_sprites();   break; /*The tile data for the first two sprites on the next scanline are fetched here
												into the shift register*/

		}
		// background evaluation done 33 times for each scan-line
		switch (nes_ppu.dot)
		{

			case 2 ... 255: case 322 ... 337: 
			pixel();
					switch (nes_ppu.dot % 8)
					{ //cycles 2-255 data for each tile is fetched
					  //cycles 321-336 first two tiles of the next scan-line are fetched and load into the shift registers.	
						
						//Each memory access takes 2 PPU cycles & 4 must be performed per tile = 8 clock cycles x tile 

						//Load nametable
						case 1: addr = nt_addr(); reload_shift(); break;
						case 2: nes_ppu.nt = rd(addr); break;

						//Load Attribute:
						case 3: addr = at_addr(); break;
						case 4: nes_ppu.at = rd(addr); break;

						// Load Background (low bits):
						case 5:  addr  = bg_addr(); break;
						case 6: nes_ppu.bgL = rd(addr);
						// Load Background (high bits):
						case 7: addr += 8;         break;
						case 0:  nes_ppu.bgH   = rd(addr); h_scroll(); break; //end of vertical blanking 
																	  //just before rendering begins 


					}
			break;


			case 			256: pixel(); nes_ppu.bgH=rd(addr);v_scroll(); break; //vertical bump
			case 			257: pixel(); reload_shift(); 	   h_update(); break;	 //update horizontal position
			case 280 ...	304: if(s==PRE)					   v_update(); break;	//update vertical position
			//not shit reloading 
			case 			 1: addr = nt_addr();if(s== PRE){nes_ppu.status.vBlank = FALSE;}break;
			case 321: case 339: addr = nt_addr(); break;			 
			//nametable fetch instead of attribute
			case 		   338:  nes_ppu.nt = rd(addr); break;
			case           340:  nes_ppu.nt = rd(addr); if (s == PRE && rendering() && nes_ppu.frameOdd) nes_ppu.dot++;

		}
		//signal scanline to mapper
		if(nes_ppu.dot == 260 && rendering() ) Cartridge_signal_Sacanline();

	}
	
}


// PPU scanlines cycles:

void step(){

	switch(nes_ppu.scanline)
	{
		case 0 ... 239: scanline_cycle(VISIBLE); break;
		case       240: scanline_cycle(POST);    break;
		case       241: scanline_cycle(NMI);	 break;
		case 	   261: scanline_cycle(PRE);	 break;
	}


	//Update dot and scanline counter

	if(++nes_ppu.dot> 340)
	{
		nes_ppu.scanline %= 341;

		if(++nes_ppu.scanline > 261)
		{
			nes_ppu.scanline =0;
			nes_ppu.frameOdd ^= 1;	
		}

	}

}
