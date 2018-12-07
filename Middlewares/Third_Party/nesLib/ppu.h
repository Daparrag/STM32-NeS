#ifndef __ppu_h
#define __ppu_h

#include <stdio.h>
#include <stdint.h>

#ifndef TRUE
#define TRUE 1
#endif


#ifndef FALSE
#define FALSE 0
#endif


typedef struct _Sprite Sprite;
typedef union _Ctrl Ctrl;
typedef union _Mask Mask;
typedef union _Status Status;
typedef union _Addr Addr;
typedef struct _PPU PPU;



typedef enum   { VISIBLE, POST, NMI, PRE } Scanline;
typedef enum   { VERTICAL, HORIZONTAL    } Mirroring;


/* Sprite buffer */
struct _Sprite
{
    uint8_t id;     // Index in OAM.
    uint8_t x;      // X position.
    uint8_t y;      // Y position.
    uint8_t tile;   // Tile index.
    uint8_t attr;   // Attributes.
    uint8_t dataL;  // Tile data (low).
    uint8_t dataH;  // Tile data (high).
};


/* PPUCTRL ($2000) register */
union _Ctrl
{
    struct
    {
        unsigned nt     : 2;  // Nametable ($2000 / $2400 / $2800 / $2C00).
        unsigned incr   : 1;  // Address increment (1 / 32).
        unsigned sprTbl : 1;  // Sprite pattern table ($0000 / $1000).
        unsigned bgTbl  : 1;  // BG pattern table ($0000 / $1000).
        unsigned sprSz  : 1;  // Sprite size (8x8 / 8x16).
        unsigned slave  : 1;  // PPU master/slave.
        unsigned nmi    : 1;  // Enable NMI.
    };
    uint8_t r;
};

/* PPUMASK ($2001) register */
union _Mask
{
    struct
    {
        unsigned gray    : 1;  // Grayscale.
        unsigned bgLeft  : 1;  // Show background in leftmost 8 pixels.
        unsigned sprLeft : 1;  // Show sprite in leftmost 8 pixels.
        unsigned bg      : 1;  // Show background.
        unsigned spr     : 1;  // Show sprites.
        unsigned red     : 1;  // Intensify reds.
        unsigned green   : 1;  // Intensify greens.
        unsigned blue    : 1;  // Intensify blues.
    };
    uint8_t r;
};

/* PPUSTATUS ($2002) register */
union _Status
{
    struct
    {
        unsigned bus    : 5;  // Not significant.
        unsigned sprOvf : 1;  // Sprite overflow.
        unsigned sprHit : 1;  // Sprite 0 Hit.
        unsigned vBlank : 1;  // In VBlank?
    };
    uint8_t r;
};



/* Loopy's VRAM address */
union _Addr
{
    struct
    {
        unsigned cX : 5;  // Coarse X.
        unsigned cY : 5;  // Coarse Y.
        unsigned nt : 2;  // Nametable.
        unsigned fY : 3;  // Fine Y.
    };
    struct
    {
        unsigned l : 8;
        unsigned h : 7;
    };
    	unsigned addr : 14;   // current memory address 
    	unsigned r : 15;       //metatable memory address 
};


struct _PPU
{
	Mirroring mirroring;        // mirroing mode.
	uint8_t ciRam[0x800];      // vRAM for nametables.	
	uint8_t cgRam[0x20];       // vRAM for palettes.
	uint8_t oamMem[0x100]; 	   // vRAM for sprite propoerties 
	Sprite oam[8],secOam[8];   // sprite buffer
	Addr vAddr, tAddr;	 	   // loppy V, T : T = nametables base address (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00);
	uint8_t fX;				   // fine scroll possition X.
	uint8_t oamAddr;		   // OAM address.
	Ctrl  ctrl; 			   // PPUCTRL   (0x2000) register
    Mask  mask 	;			   // PPUMASK   (0x2001) register
    Status status;			   // PPUSTATUS (0x2002) register

/*fixme: background latches*/
    uint8_t nt, at, bgL,bgH;
/*fixme: background shift registers*/
    uint8_t atShiftL, arShiftH;  //2 8-bit shift registers - These contain the palette attributes for the lower 8 pixels of the 16-bit shift register.
    uint16_t bgShiftL, bgShiftH; //2 16-bit shift registers - These contain the bitmap data for two tiles
    uint8_t atLatch_fg;

// rendering counters
    int scanline, dot;
    uint8_t frameOdd;

    //video bufer


};

uint8_t PPU_access (uint8_t wr, uint16_t addr_idx, uint8_t val );
void set_mirrowing(Mirroring mode);
void step();
void reset();

#endif /*_ppu_h*/
