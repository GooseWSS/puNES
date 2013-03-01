/*
 * mapper_Caltron.c
 *
 *  Created on: 16/set/2011
 *      Author: fhorse
 */

#include <stdlib.h>
#include "mappers.h"
#include "mem_map.h"
#include "save_slot.h"

WORD prg_rom_32k_max, chr_rom_8k_max;

void map_init_Caltron(void) {
	prg_rom_32k_max = (info.prg_rom_16k_count >> 1) - 1;
	chr_rom_8k_max = info.chr_rom_8k_count - 1;

	EXTCL_CPU_WR_MEM(Caltron);

	info.mapper_extend_wr = TRUE;

	if (info.reset >= HARD) {
		caltron.reg = 0;
		map_prg_rom_8k(4, 0, 0);
	}
}
void extcl_cpu_wr_mem_Caltron(WORD address, BYTE value) {
	DBWORD bank;

	if (address < 0x6000) {
		return;
	}

	if ((address >= 0x6000) && (address < 0x6800)) {
		caltron.reg = value = address & 0x00FF;

		control_bank_with_AND(0x07, prg_rom_32k_max)
		map_prg_rom_8k(4, 0, value);
		map_prg_rom_8k_update();

		if (caltron.reg & 0x10) {
			mirroring_H();
		} else {
			mirroring_V();
		}
		return;
	}

	if (address < 0x8000) {
		return;
	}

	if (caltron.reg & 0x04) {
		value = ((caltron.reg >> 1) & 0x0C) | (value & 0x03);
		control_bank(chr_rom_8k_max)
		bank = value << 13;
		chr.bank_1k[0] = &chr.data[bank];
		chr.bank_1k[1] = &chr.data[bank | 0x0400];
		chr.bank_1k[2] = &chr.data[bank | 0x0800];
		chr.bank_1k[3] = &chr.data[bank | 0x0C00];
		chr.bank_1k[4] = &chr.data[bank | 0x1000];
		chr.bank_1k[5] = &chr.data[bank | 0x1400];
		chr.bank_1k[6] = &chr.data[bank | 0x1800];
		chr.bank_1k[7] = &chr.data[bank | 0x1C00];
	}
}
BYTE extcl_save_mapper_Caltron(BYTE mode, BYTE slot, FILE *fp) {
	save_slot_ele(mode, slot, caltron.reg);

	return (EXIT_OK);
}