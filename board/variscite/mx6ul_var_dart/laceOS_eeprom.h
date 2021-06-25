/*
 * Copyright (C) 2021 Salzbrenner media
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef _LACEOS_EEPROM_H_
#define _LACEOS_EEPROM_H_

#define LACE_MAGIC		0x4543414C /* == SWAPPED HEX("LACE") */

#define LACEOS_EEPROM_I2C_BUS		0
#define LACEOS_EEPROM_I2C_ADDR	0x57

#ifdef LACEEEPROM_DEBUG
#define laceeeprom_debug(M, ...) printf("LACEEEPROM DEBUG: " M, ##__VA_ARGS__)
#else
#define laceeeprom_debug(M, ...)
#endif

struct __attribute__((packed)) lace_eeprom_cfg
{
	u32 lace_magic; /* == SWAPPED HEX("LACE") */
	u8 mac1[6];
};

int laceos_eeprom_read_struct(struct lace_eeprom_cfg *p_lace_eeprom_cfg);

#endif /* _LACEOS_EEPROM_H_ */
