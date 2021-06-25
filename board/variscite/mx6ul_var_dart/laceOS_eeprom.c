/*
 * Copyright (C) 2021 Salzbrenner media
 *
 * SPDX-License-Identifier: GPL-2.0+
 */
#include <common.h>
#include <i2c.h>
#include "laceOS_eeprom.h"

static inline bool laceos_eeprom_is_valid(const struct lace_eeprom_cfg *p_lace_eeprom_cfg)
{
	return (LACE_MAGIC == p_lace_eeprom_cfg->lace_magic);
}

int laceos_eeprom_read_struct(struct lace_eeprom_cfg *p_lace_eeprom_cfg)
{
	i2c_set_bus_num(LACEOS_EEPROM_I2C_BUS);
	
	if (i2c_probe(LACEOS_EEPROM_I2C_ADDR)) {
		laceeeprom_debug("\nError: Couldn't find EEPROM device\n");
		return -1;
	}

	if (i2c_read(LACEOS_EEPROM_I2C_ADDR, 0, 2,
				(u8 *) p_lace_eeprom_cfg,
				sizeof(struct lace_eeprom_cfg))) {
		laceeeprom_debug("\nError reading data from EEPROM\n");
		return -1;
	}

	if (!laceos_eeprom_is_valid(p_lace_eeprom_cfg)) {
		laceeeprom_debug("\nError: Data on EEPROM is invalid\n");
		return -1;
	}

	return 0;
}
