#ifndef PIEMU_PME_FLASH_H
#define PIEMU_PME_FLASH_H

#include "../core/c33bcu.h"
#include <vector>

#define CFIINFO_OFFSET	0x10		/* CFIクエリー情報の先頭オフセット（ハーフワード単位！） */
struct CFIINFO {
    /* CFI Query Identification String */
    uint16_t query_unique_ascii_string[3];							/* 10-12 */
    uint16_t primary_oem_command_set[2];								/* 13-14 */
    uint16_t address_for_primary_extended_table[2];					/* 15-16 */
    uint16_t alternate_oem_command_set[2];							/* 17-18 */
    uint16_t address_for_alternate_oem_extended_table[2];				/* 19-1a */
    /* System Interface Information */
    uint16_t vdd_min;													/* 1b */
    uint16_t vdd_max;													/* 1c */
    uint16_t vpp_min;													/* 1d */
    uint16_t vpp_max;													/* 1e */
    uint16_t typical_time_out_for_word_program;						/* 1f */
    uint16_t typical_time_out_for_min_size_buffer_program;			/* 20 */
    uint16_t typical_time_out_for_individual_sector_block_erase;		/* 21 */
    uint16_t typical_time_out_for_chip_erase;							/* 22 */
    uint16_t maximum_time_out_for_word_program;						/* 23 */
    uint16_t maximum_time_out_for_buffer_program;						/* 24 */
    uint16_t maximum_time_out_for_individual_sector_block_erase;		/* 25 */
    uint16_t maximum_time_out_for_chip_erase;							/* 26 */
    /* Device Geometry Information */
    uint16_t device_size;												/* 27 */
    uint16_t flash_device_interface_description[2];					/* 28-29 */
    uint16_t maximum_number_of_byte_in_multi_byte_write[2];			/* 2a-2b */
    uint16_t number_of_erase_sector_block_sizes_supported_by_device;	/* 2c */
    uint16_t sector_information[4];									/* 2d-30 */
    uint16_t block_information[4];									/* 31-34 */
};

#define FLASH_NORMAL		0
#define FLASH_ENTER1		1	/* NORMAL -> [5555*2]=??aa */
#define FLASH_ENTER2		2	/* ENTER1 -> [2aaa*2]=??55 */
#define FLASH_ENTER3		3	/* ENTER2 -> [5555*2]=??80 */
#define FLASH_ENTER4		4	/* ENTER3 -> [5555*2]=??aa */
#define FLASH_ENTER5		5	/* ENTER4 -> [2aaa*2]=??55 */
/* CHIP   ERASE */			/* ENTER5 -> [5555*2]=??10 -> EXIT */
/* SECTOR ERASE */			/* ENTER5 -> [??????]=??30 -> EXIT */
/* BLOCK  ERASE */			/* ENTER5 -> [??????]=??50 -> EXIT */
#define FLASH_CFI_QUERY		6	/* ENTER2 -> [5555*2]=??98 */
/* SOFTWARE ID (≒ CFI QUERY) */	/* ENTER2 -> [5555*2]=??90 */
/* EXIT CFI QUERY/SOFTWARE ID */	/* ------------------------------------------> [??????]=??f0 -> EXIT */
/* または -> [5555*2]=??aa -> [2aaa*2]=??55 -> [5555*2]=??f0 -> EXIT */
#define FLASH_WORD_PROGRAM	7	/* ENTER2 -> [5555*2]=??a0 */
/* LOAD WORD ADDRESS/WORD DATA */	/* --------> [??????]=???? -> EXIT */

class PMEFlash: public C33BusDevice
{
private:
    CFIINFO cfiinfo;
    int mem_size;   // # of bits to present mem addr
    std::vector<uint8_t> mem;
    int stat;

public:
    ~PMEFlash() override = default;

    void init(void* context) override;

    template<class T>
    T flash_read(c33word ofs)
    {
        ofs &= mem_size - 1;
        switch(stat)
        {
            case FLASH_NORMAL: {
                MemoryAccessor<std::vector<uint8_t>> acc(mem);
                return acc.mem_read(ofs, sizeof(T));
            }
            case FLASH_CFI_QUERY: {
                ofs -= CFIINFO_OFFSET * sizeof(short);
                if (ofs > sizeof(CFIINFO)) throw std::range_error("flash: CFI offset out of range");
                MemoryAccessor<std::vector<c33byte>> acc((c33byte*)&cfiinfo);
                return acc.mem_read(ofs, sizeof(short));
            }
        }
        throw std::logic_error("flash: read(): invalid status");
    }

    c33byte bus_readB(c33word ofs) override {
        return flash_read<c33byte>(ofs);
    }
    void bus_writeB(c33word ofs, c33byte data) override {
        throw std::logic_error("FLASH: writing other than h-word is not allowed");
    }

    c33hword bus_readH(c33word ofs) override {
        return flash_read<c33hword>(ofs);
    }
    void bus_writeH(c33word ofs, c33hword data) override;

    c33word bus_readW(c33word ofs) override {
        return flash_read<c33word>(ofs);
    }
    void bus_writeW(c33word ofs, c33word data) override {
        throw std::logic_error("FLASH: writing other than h-word is not allowed");
    }
};

#endif //PIEMU_PME_FLASH_H
