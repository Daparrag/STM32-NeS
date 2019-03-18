#include "ppu.h"
#include "mapper.h"
#include "mapper1.h"

static Mapper1 mapper1Str;




void MP1_apply()
{
	if(mapper1Str.regs[0] & 0b1000)
	{
		// 0x8000 swappable, 0xC000 fixed to bank 0x0F:
		if (mapper1Str.regs[0] & 0b100)
		{
			map_prg(16,0,mapper1Str.regs[3] & 0xF);
			map_prg(16,1,0xF);


		}
		// 0x8000 fixed to bank 0x00, 0xC000 swappable:
		else{
			 map_prg(16, 0, 0);
			 map_prg(16, 1, mapper1Str.regs[3] & 0xF);

		}
	}
	//32KB PRG:
	else
	{
		map_prg(32, 0, (mapper1Str.regs[3] & 0xF) >> 1);
	}
	// 4KB CHR:
	if (mapper1Str.regs[0] & 0b10000)
	{
		map_chr(4, 0, mapper1Str.regs[1]);
		map_chr(4, 1, mapper1Str.regs[2]);
	}
	// 8KB CHR:
	else
	{
		map_chr(8, 0, mapper1Str.regs[1] >> 1);
	}

	// Set mirroring:
	switch (mapper1Str.regs[0] & 0b11)
	{
		case 2: set_mirroring (VERTICAL); break;
		case 3: set_mirroring (HORIZONTAL); break;
	}
}


uint8_t MP1_write (uint16_t addr, uint8_t v)
{
	// PRG RAM write;
	 if (addr < 0x8000)
		 mapperStr.prgRam[addr - 0x6000] = v;
	 // Mapper register write:
	 else if (addr & 0x8000)
	 {
	 	// Reset:
	 	if((v = 0x80))
	 	{
	 		mapper1Str.writeN   = 0;
	 		mapper1Str.tmpReg   = 0;
	 		mapper1Str.regs[0] |=0x0C;
	 		MP1_apply();
	 	}
	 	else
	 	{
	 		// Write a bit into the temporary register:
	 		mapper1Str.tmpReg   = ((v & 1) << 4) | (mapper1Str.tmpReg >> 1);
	 		if (++mapper1Str.writeN == 5)
	 		{
	 			mapper1Str.regs[(addr >> 13) & 0b11] = mapper1Str.tmpReg;
	 			mapper1Str.writeN = 0;
	 			mapper1Str.tmpReg = 0;
                MP1_apply();
	 		}
	 	}
	 }
	 
	return v;
}


uint8_t MP1_chr_write (uint16_t addr, uint8_t v)
{
	 return mapperStr.chr[addr] = v;
}


void mapper1(uint8_t * rom)
{
	mapper(rom);
	mapper1Str.regs[0] = 0x0C;
	mapper1Str.writeN  = 0;
	mapper1Str.regs[1] = 0;
	mapper1Str.regs[2] = 0;
	mapper1Str.regs[3] = 0;
	mapperStr.map_fun.mapper = mapper1;
	mapperStr.map_fun.write = MP1_write;
	mapperStr.map_fun.chr_write = MP1_chr_write;
	MP1_apply();
}

void mapper1_init()
{
	mapperStr.map_fun.mapper = mapper1;
}
